#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

QString username;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    openFileAction = new QAction("&Open Database", this);
    connect(openFileAction, SIGNAL(triggered()), this, SLOT(openFile()));
    ui->menubar->addMenu("&File")->addAction(openFileAction);
}

MainWindow::~MainWindow()
{
    saveDB();
    delete ui;
}

void MainWindow::enableLoginPB()
{
    if(ui->usernameLE->text().length() > 0 && ui->passwordLE->text().length() > 0){
        ui->loginPB->setEnabled(true);
    }else{
        ui->loginPB->setEnabled(false);
    }
}

void MainWindow::enableSignInPB()
{
    if(ui->newUserLE->text().length() > 0 && ui->emailLE->text().length() > 0 && ui->newPasswordLE->text().length() > 0){
        ui->signInPB->setEnabled(true);
    }else{
        ui->signInPB->setEnabled(false);
    }
}

void MainWindow::validateUser()
{
    QMessageBox message;
    vector<User>::iterator it;
    QString user = ui->usernameLE->text();
    QString password = ui->passwordLE->text();

    it = find_if(users.begin(), users.end(), [&user, &password](User u) -> bool{
        return u.getName() == user && u.getPassword() == password;
    }
    );

    if(it == users.end()){
        message.setText("Invalid credentials");
        message.setIcon(QMessageBox::Warning);
        message.exec();
    }else{
        //qDebug() << "Nombre" + nameFile;
        message.setText("Welcome to LERMA " + user);
        username = user;
        ui->viewSW->setCurrentIndex(1);
        loadProductsByDepartment(0);
        message.exec();
    }
}

bool MainWindow::validateEmail()
{
    QString email = ui->emailLE->text();
    QString expression = "^[_a-z0-9-]+(\\.[_a-z0-9-]+)*@[a-z0-9-]+(\\.[a-z0-9-]+)*(\\.[a-z]{2,4})$";
    QRegularExpression validate(expression);

    return validate.match(email).hasMatch();
}

bool MainWindow::validateCredential()
{
    QMessageBox message;
    vector<User>::iterator it;
    QString user = ui->newUserLE->text();
    QString email = ui->emailLE->text();

    it = find_if(users.begin(), users.end(), [&user](User u) -> bool{return u.getName() == user;});

    if(it == users.end()){
        it = find_if(users.begin(), users.end(), [&email](User u) -> bool{return u.getEmail() == email;});
        if(it == users.end()){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}

void MainWindow::saveDB()
{
    QJsonObject jsonObj, jsonAddProductsDate, jsonProduct;
    QJsonArray objects;
    QJsonDocument jsonDoc;
    qDebug() << username;
    if(dbArrayAddProducts.size() > 0){
        qDebug() << dbArrayAddProducts.size();
        for(int i(0); i < dbArray.size(); i++){
            QJsonObject obj = dbArray[i].toObject();

            if(username == obj["name"].toString()){            //dbArray[i].toObject()["name"].toString()){
                //QDateTime date;
                QTime time = QTime::currentTime();
                QDateTime date = QDateTime::currentDateTime();
                dbArrayPurchase =  obj["purchase"].toArray();        //dbArray[i].toObject()["purchase"].toArray();
                jsonAddProductsDate[date.toString("dd/MM/yyyy") + " " + time.toString("hh:mm:ss")] = dbArrayAddProducts;
                dbArrayPurchase.append(jsonAddProductsDate);
                //dbArray[i].toObject()["purchase"] = dbArrayPurchase;

                jsonProduct["name"] = obj["name"];
                jsonProduct["email"] = obj["email"];
                jsonProduct["password"] = obj["password"];
                jsonProduct["purchase"] = dbArrayPurchase;
                objects.append(jsonProduct);
            }else{
                jsonProduct["name"] = obj["name"];
                jsonProduct["email"] = obj["email"];
                jsonProduct["password"] = obj["password"];
                jsonProduct["purchase"] = obj["purchase"];
                objects.append(jsonProduct);
            }
        }
        jsonObj["users"] = objects;
    }else{
        jsonObj["users"] = dbArray;
    }

    //jsonObj["users"] = dbArray;
    jsonObj["products"] = dbArrayObject;
    jsonDoc = QJsonDocument(jsonObj);

    dbFile.open(QIODevice::WriteOnly);
    dbFile.write(jsonDoc.toJson());
    dbFile.close();
}

void MainWindow::loadBD()
{
    QJsonObject jsonObj;
    QJsonDocument jsonDoc;
    QByteArray data;

    dbFile.open(QIODevice::ReadOnly);
    data = dbFile.readAll();
    dbFile.close();

    jsonDoc = QJsonDocument::fromJson(data);
    jsonObj = jsonDoc.object();
    dbArray = jsonObj["users"].toArray();
    dbArrayObject = jsonObj["products"].toArray();

    for (int i(0); i < dbArray.size(); ++i) {
        User u;
        QJsonObject obj = dbArray[i].toObject();
        u.setName(obj["name"].toString());
        u.setEmail(obj["email"].toString());
        u.setPassword(obj["password"].toString());

        users.push_back(u);
    }
}

void MainWindow::loadProductsByDepartment(int n)
{
    products.clear();
    int cont = (n > 0) ? (n-1)*10 : 0;
    int departmentSize = (n > 0) ? n*10 : dbArrayObject.size();
    ui->sortingCB->setCurrentIndex(0);
    ui->searchLE->clear();

    for(int i(cont); i < departmentSize; i++){
        Product p;
        QJsonObject x = dbArrayObject[i].toObject();
        p.setId(x["id"].toString());
        p.setName(x["name"].toString());
        p.setPrice(x["price"].toDouble());

        products.push_back(p);
    }
    showProducts(products);
}

void MainWindow::showProducts(vector<Product> &p)
{
    deleteProducts();
    QGridLayout *productsGrid = new QGridLayout();
    for(size_t i(0); i < p.size(); i++){
        ProductWidget *product = new ProductWidget("C:/Users/Edyal/Documents/seminarioAlgoritmia/Proyecto/lerma/imgs/"+p[i].getId() + ".jpg",p[i].getId(),p[i].getName(),p[i].getPrice(),ui->productsSA);
        connect(product, SIGNAL(addItem(QString, int)), this, SLOT(addToChart(QString, int)));
        productsGrid->addWidget(product,i/4,i%4);
    }
    ui->productsArea->setLayout(productsGrid);
}

void MainWindow::loadObjectsBySort(vector<Product> &p, int n)
{
    if(n == 1){
        sort(p.begin(), p.end(),[](const Product &a, const Product &b){return a.getPrice() < b.getPrice();});
    }else if(n == 2){
        sort(p.begin(), p.end(),[](const Product &a, const Product &b){return a.getPrice() > b.getPrice();});
    }

    (ui->searchLE->text().size()) ? loadObjectsBySearch(p) : showProducts(p);
}

void MainWindow::loadObjectsBySearch(const vector<Product> &p)
{
    int cont = 0;
    QGridLayout *productsGrid = new QGridLayout();
    for(size_t i(0); i < p.size(); i++){
        if(ui->searchLE->text().size() <= p[i].getName().size()){
            if(p[i].getName().contains(ui->searchLE->text(), Qt::CaseInsensitive)){
                ProductWidget *product = new ProductWidget("C:/Users/Edyal/Documents/seminarioAlgoritmia/Proyecto/lerma/imgs/"+p[i].getId() + ".jpg",p[i].getId(),p[i].getName(),p[i].getPrice(),ui->productsSA);
                connect(product, SIGNAL(addItem(QString, int)), this, SLOT(addToChart(QString, int)));
                productsGrid->addWidget(product,cont/4,cont%4);
                cont++;
            }
        }
    }
    if(cont <= 4){
        if(cont < 4 && cont > 0){
            int marginW = ui->productsSA->width() / 4;
            int margin = (ui->productsSA->width() - marginW) / cont;
            productsGrid->setContentsMargins(margin * 0.4,200,margin * 0.4,250);
            productsGrid->setSpacing(margin * 0.4);
        }else{
            productsGrid->setContentsMargins(0,200,0,250);
        }
    }else{
        productsGrid->setContentsMargins(0,0,0,0);
    }

    ui->productsArea->setLayout(productsGrid);
}

void MainWindow::deleteProducts()
{
    if(ui->productsArea->layout() != NULL){
        QLayoutItem* item;
        while((item = ui->productsArea->layout()->takeAt(0)) != NULL){
            delete item->widget();
            delete item;
        }
        delete ui->productsArea->layout();
    }
}

void MainWindow::on_usernameLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enableLoginPB();
}

void MainWindow::on_passwordLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enableLoginPB();
}

void MainWindow::on_newUserLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enableSignInPB();
}

void MainWindow::on_emailLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enableSignInPB();
}

void MainWindow::on_newPasswordLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enableSignInPB();
}

void MainWindow::on_signInPB_clicked()
{
    QMessageBox messageNew, message;
    bool validateUE = validateCredential();
    bool validateE = validateEmail();

    if(validateE && validateUE){
        User u;
        QJsonObject jsonOBJ;

        u.setName(ui->newUserLE->text());
        u.setEmail(ui->emailLE->text());
        u.setPassword(ui->newPasswordLE->text());

        users.push_back(u);

        message.setText("New user created");
        message.exec();

        ui->newUserLE->clear();
        ui->emailLE->clear();
        ui->newPasswordLE->clear();

        jsonOBJ["name"] = u.getName();
        jsonOBJ["email"] = u.getEmail();
        jsonOBJ["password"] = u.getPassword();
        dbArray.append(jsonOBJ);
    }
    else{
        if(validateE == false){
            messageNew.setText("Invalid email");
            messageNew.setIcon(QMessageBox::Warning);
            messageNew.exec();
            ui->emailLE->clear();
        }else{
            if(validateUE == false){
                messageNew.setText("Username or email is already registered, write another");
                messageNew.setIcon(QMessageBox::Warning);
                messageNew.exec();
                ui->newUserLE->clear();
                ui->emailLE->clear();
            }
        }
    }
}

void MainWindow::on_loginPB_clicked()
{
    validateUser();
    ui->usernameLE->clear();
    ui->passwordLE->clear();
}

void MainWindow::openFile()
{
    QString name;
    name = QFileDialog::getOpenFileName(this,
                                        "Select Database",
                                        "",
                                        "JSON files (*.json)");
    if(name.length() > 0){
        dbFile.setFileName(name);
        ui->loginGB->setEnabled(true);
        ui->signInGB->setEnabled(true);
        //nameFile = name;
        loadBD();
    }
}

void MainWindow::on_departmentCB_currentIndexChanged(int index)
{
    deleteProducts();
    loadProductsByDepartment(index);
}

void MainWindow::on_searchLE_textEdited(const QString &arg1)
{
    deleteProducts();
    Q_UNUSED(arg1);
    loadObjectsBySearch(products);
}

void MainWindow::on_sortingCB_currentIndexChanged(int index)
{
    deleteProducts();
    loadObjectsBySort(products,index);
}

void MainWindow::addToChart(QString item, int amount)
{
    //qDebug() << "Id: " << item;
    //qDebug() << "Cantidad: " << amount;
    //QTime q = QTime::currentTime();
    //QDateTime qt = QDateTime::currentDateTime();
    //qDebug() << qt.toString("dd/MM/yyyy");
    //qDebug() << q.toString("hh: mm : ss");
    //qDebug() << q.toString()
    QJsonObject jsonOBJ;
    jsonOBJ["id"] = item;
    jsonOBJ["units"] = amount;
    dbArrayAddProducts.append(jsonOBJ);
}
