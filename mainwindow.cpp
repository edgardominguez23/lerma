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

        productsCopia = products;
        showPriorityProducts("");
        //loadProductsPriority("");

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
    //qDebug() << username;
    if(dbArrayAddProducts.size() > 0){
        //qDebug() << dbArrayAddProducts.size();
        for(int i(0); i < dbArray.size(); i++){
            QJsonObject obj = dbArray[i].toObject();
            if(username == obj["name"].toString()){
                QTime time = QTime::currentTime();
                QDateTime date = QDateTime::currentDateTime();
                dbArrayPurchase =  obj["purchase"].toArray();
                jsonAddProductsDate[date.toString("dd/MM/yyyy") + " " + time.toString("hh:mm:ss")] = dbArrayAddProducts;
                dbArrayPurchase.append(jsonAddProductsDate);

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

void MainWindow::showProductsPriority(vector<pair<Product,int>> &p)
{
    deleteProductsPriority();
    QGridLayout *productsGrid = new QGridLayout();
    for(size_t i(0); i < p.size(); i++){
        ProductWidget *product = new ProductWidget("C:/Users/Edyal/Documents/seminarioAlgoritmia/Proyecto/lerma/imgs/"+p[i].first.getId() + ".jpg",p[i].first.getId(),p[i].first.getName(),p[i].first.getPrice(),ui->productsPSA);
        connect(product, SIGNAL(addItem(QString, int)), this, SLOT(addToChart(QString, int)));
        productsGrid->addWidget(product,i/5,i%5);
    }
    ui->productsPriority->setLayout(productsGrid);
}

void MainWindow::loadProductsPriority(QString id)
{
    vector<pair<Product,int>> p;

    if(!id.size()){
        generateRandomProductInQueue(5);

        while(!priorityProducts.empty()){
            p.push_back(make_pair(searchProductPriority(QString::fromStdString(priorityProducts.top().second)),priorityProducts.top().first));
            priorityProducts.pop();
        }
    }else{
        if(graph.contains(id.toUtf8().constData())){
            map<string, int> c = graph.getNeighbors(id.toUtf8().constData());

            for (auto it = c.begin(); it != c.end(); it++) {
                priorityProducts.push(make_pair((*it).second,(*it).first));
            }
        }

        p.push_back(make_pair(searchProductPriority(QString::fromStdString(priorityProducts.top().second)),priorityProducts.top().first));
        priorityProducts.pop();

        for (int j(0);j < 4; j++) {
            string n1 = priorityProducts.top().second;
            int cont = 0;

            for(size_t k(0); k < p.size(); k++){
                if(n1 == p[k].first.getId().toUtf8().constData()){
                    cont++;
                }
            }

            if(cont > 0){
                priorityProducts.pop();
            }else{
                p.push_back(make_pair(searchProductPriority(QString::fromStdString(priorityProducts.top().second)),priorityProducts.top().first));
                priorityProducts.pop();
            }
        }
    }
    deleteProductsPriority();
    showProductsPriority(p);

    while(!priorityProducts.empty()){
        priorityProducts.pop();
    }

    for (size_t i(0);i < p.size(); i++) {
        priorityProducts.push(make_pair(p[i].second,p[i].first.getId().toUtf8().constData()));
    }
    p.clear();
}

Product MainWindow::searchProductPriority(QString id)
{
    auto it = find_if(productsCopia.begin(),productsCopia.end(),[&id](Product p) -> bool{return p.getId() == id;});

    return *it;
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

void MainWindow::deleteProductsPriority()
{
    if(ui->productsPriority->layout() != NULL){
        QLayoutItem* item;
        while((item = ui->productsPriority->layout()->takeAt(0)) != NULL){
            delete item->widget();
            delete item;
        }
        delete ui->productsPriority->layout();
    }
}

void MainWindow::createGraph()
{
    QJsonArray purchases;
    for(int i(0); i < dbArray.size(); ++i){
        QJsonObject obj = dbArray[i].toObject();
        purchases = obj["purchase"].toArray();

        if(!purchases.empty()){
            for(int j(0); j < purchases.size(); j++){
                QJsonObject datePurchases = purchases[j].toObject();
                QStringList date = datePurchases.keys();
                QString key = date[0];
                QJsonArray buys = datePurchases[key].toArray();

                if(buys.size() > 1){
                    for(int k(0); k < buys.size()-1; k++){
                        QJsonObject productOne = buys[k].toObject();
                        string pOne = productOne["id"].toString().toUtf8().constData();
                        for(int l(k+1); l < buys.size(); l++){
                            if(k != l){
                                QJsonObject productTwo = buys[l].toObject();
                                string pTwo = productTwo["id"].toString().toUtf8().constData();
                                if(graph.isEdge(pOne,pTwo)){
                                    int cost = graph.getCost(pOne,pTwo);

                                    graph.createEdge(pOne,pTwo,cost + 1);
                                }else{
                                    graph.createEdge(pOne,pTwo,1);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    graph.printData();
}

QString MainWindow::generateRandomProduct(int min, int max)
{
    uniform_int_distribution<int> distribution(min,max);
    int x = distribution(*QRandomGenerator::global());

    return products[x].getId();
}

ProductWidget *MainWindow::searchProduct(QString id) //Busca producto y retorna el productWidget correspondiente
{
    auto it = find_if(productsCopia.begin(),productsCopia.end(),[&id](Product p) -> bool{return p.getId() == id;});
    Product *p = &(*it);
    return new ProductWidget("C:/Users/Edyal/Documents/seminarioAlgoritmia/Proyecto/lerma/imgs/"+p->getId() + ".jpg",p->getId(),p->getName(),p->getPrice(),ui->productsPSA);
}

void MainWindow::showPriorityProducts(QString id)
{
    queue<pair<int,string> > copia;

    if(!id.size()){
        deleteProductsPriority();
        QGridLayout *productsGrid = new QGridLayout();
        generateRandomProductInQueue(5);

        //Llenar la cola copia
        while(!priorityProducts.empty()){
            copia.push(make_pair(priorityProducts.top().first,priorityProducts.top().second));
            priorityProducts.pop();
        }

        //Llenar la cola de prioridad
        int cont = 0;
        while(!copia.empty()){
            //deleteProductsPriority();
            ProductWidget *product = searchProduct(QString::fromStdString(copia.front().second));
            connect(product, SIGNAL(addItem(QString, int)), this, SLOT(addToChart(QString, int)));
            productsGrid->addWidget(product,cont / 5, cont % 5);
            cont++;

            priorityProducts.push(make_pair(copia.front().first,copia.front().second));
            copia.pop();
        }
        ui->productsPriority->setLayout(productsGrid);

    }else{
        //QGridLayout *productsGrid = new QGridLayout();
        if(graph.contains(id.toUtf8().constData())){
            deleteProductsPriority();
            QGridLayout *productsGrid = new QGridLayout();
            map<string, int> c = graph.getNeighbors(id.toUtf8().constData());

            for (auto it = c.begin(); it != c.end(); it++) {
                priorityProducts.push(make_pair((*it).second,(*it).first));
            }

            for (int i(0);i < 5;i++) {
                if(copia.empty()){
                    copia.push(make_pair(priorityProducts.top().first,priorityProducts.top().second));
                    priorityProducts.pop();
                }

                if(copia.size() == 5) break;
                string backId2 = copia.front().second;
                string topId2 = priorityProducts.top().second;
                int cont = 0;
                size_t sizeCopia = copia.size();
                for(size_t j(0); j < sizeCopia; j++){
                    if(topId2 == backId2){
                        cont++;
                    }
                    copia.push(make_pair(copia.front().first, copia.front().second));
                    copia.pop();
                    backId2 = copia.front().second;
                }
                if(cont == 0){
                    copia.push(make_pair(priorityProducts.top().first, priorityProducts.top().second));
                    priorityProducts.pop();
                }else{
                    priorityProducts.pop();
                }
            }
            //Vaciar cola principal
            while(!priorityProducts.empty()){
                priorityProducts.pop();
            }

            //Llenar la cola de prioridad
            int cont = 0;
            while(!copia.empty()){
                //deleteProductsPriority();
                ProductWidget *product = searchProduct(QString::fromStdString(copia.front().second));
                connect(product, SIGNAL(addItem(QString, int)), this, SLOT(addToChart(QString, int)));
                productsGrid->addWidget(product,cont / 5, cont % 5);
                cont++;

                priorityProducts.push(make_pair(copia.front().first,copia.front().second));
                copia.pop();
            }
            ui->productsPriority->setLayout(productsGrid);
        }
    }
}

void MainWindow::generateRandomProductInQueue(int n)
{
    for(int i(0); i < n; i++){
        string randomId = generateRandomProduct(i * 10,((i+1)*10)-1).toUtf8().constData();

        priorityProducts.push(make_pair(0,randomId));
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
        createGraph();
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
    QJsonObject jsonOBJ;
    jsonOBJ["id"] = item;
    jsonOBJ["units"] = amount;
    dbArrayAddProducts.append(jsonOBJ);

    showPriorityProducts(item);
    //loadProductsPriority(item);
}
