#include "productwidget.h"
#include "ui_productwidget.h"
#include <QPixmap>
#include <QVBoxLayout>
#include <algorithm>

bool firstActivation = false;

ProductWidget::ProductWidget(QString name, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProductWidget)
{
    ui->setupUi(this);

    nameFile = name;

    loadAllObjects();
    loadObjectsByDepartment(0);
}

ProductWidget::~ProductWidget()
{
    deleteWidgets();
    delete ui;
}

QString ProductWidget::getNameFile() const
{
    return nameFile;
}

void ProductWidget::setNameFile(const QString &value)
{
    nameFile = value;
}

void ProductWidget::loadAllObjects()
{
    QJsonObject jsonObj;
    QJsonDocument jsonDoc;
    QByteArray data;

    dbFile.setFileName(getNameFile());

    dbFile.open(QIODevice::ReadOnly);
    data = dbFile.readAll();
    dbFile.close();

    jsonDoc = QJsonDocument::fromJson(data);
    jsonObj = jsonDoc.object();
    dbArrayObjects = jsonObj["products"].toArray();
}

void ProductWidget::loadObjectsByDepartment(int departmentNumber)
{
    deleteProducts();
    int cont = (departmentNumber > 0) ? (departmentNumber-1)*10 : 0;
    int departmentSize = (departmentNumber > 0) ? departmentNumber*10 : dbArrayObjects.size();
    ui->sortCB->setCurrentIndex(0);
    ui->searchLE->clear();

    for(int i(cont); i < departmentSize; i++){
        Product p;
        QJsonObject x = dbArrayObjects[i].toObject();
        p.setId(x["id"].toString());
        p.setName(x["name"].toString());
        p.setPrice(x["price"].toDouble());

        products.push_back(p);
    }
    showProducts(products);
}

void ProductWidget::loadObjectsBySort(vector<Product> &p, int sortNumber)
{

    if(sortNumber == 1){
        sort(p.begin(), p.end(),[](const Product &a, const Product &b){return a.getPrice() < b.getPrice();});
    }else if(sortNumber == 2){
        sort(p.begin(), p.end(),[](const Product &a, const Product &b){return a.getPrice() > b.getPrice();});
    }

    (ui->searchLE->text().size()) ? loadObjectsBySearch(p) : showProducts(p);
    //showProducts(p);
}

void ProductWidget::loadObjectsBySearch(const vector<Product> &p)
{
    int cont = 0;
    QString text = ui->searchLE->text();
    QGridLayout *contGL = new QGridLayout(this);

    for(size_t i(0); i < p.size(); i++){
        if(text.size() <= p[i].getName().size()){
            if(p[i].getName().contains(text, Qt::CaseInsensitive)){

                QLabel *img = new QLabel();
                QLabel *name = new QLabel();
                QLabel *price = new QLabel();
                QWidget *contWidget = new QWidget();
                //QGridLayout *contTwoGL = new QGridLayout();
                QVBoxLayout *contTwoGL = new QVBoxLayout();
                QPixmap pix("C:/Users/Edyal/Documents/seminarioAlgoritmia/Proyecto/lerma/imgs/"+p[i].getId()+".jpg");

                img->setPixmap(pix.scaled(300,300,Qt::KeepAspectRatio));
                img->setAlignment(Qt::AlignCenter);
                name->setText(p[i].getName());
                name->setAlignment(Qt::AlignCenter);
                name->setWordWrap(true);//Si el texto no abarca el ancho, se realiza un salto de linea
                price->setText("$" + QString::number(p[i].getPrice()));
                price->setAlignment(Qt::AlignCenter);

                contTwoGL->addWidget(img,Qt::AlignCenter);
                contTwoGL->addWidget(name);
                contTwoGL->addWidget(price);
                //contTwoGL->setContentsMargins(0,150,0,250);
                contWidget->setLayout(contTwoGL);
                //contWidget->setStyleSheet("background-color:red;");

                int posX = cont / 4, posY = cont % 4;
                //qDebug() << "posX" << posX << "posY" << posY;
                contGL->addWidget(contWidget,posX,posY);
                cont++;
            }
        }
    }

    (cont <= 4) ? contGL->setContentsMargins(0,200,0,250) : contGL->setContentsMargins(0,0,0,0);
    ui->scrollContents->setLayout(contGL);
}

void ProductWidget::showProducts(const vector<Product> &p)
{
    deleteWidgets();
    QGridLayout *contGL = new QGridLayout(this);
    size_t contador = 0;

    for (size_t i(0);i <= p.size() / 4 ;i++ ) {
        for(int j(0); j < 4; j++){
            if(contador >= p.size()) break;

            QLabel *img = new QLabel();
            QLabel *name = new QLabel();
            QLabel *price = new QLabel();
            QWidget *contWidget = new QWidget();
            //QGridLayout *contTwoGL = new QGridLayout();
            QVBoxLayout *contTwoGL = new QVBoxLayout();
            QPixmap pix("C:/Users/Edyal/Documents/seminarioAlgoritmia/Proyecto/lerma/imgs/"+p[contador].getId()+".jpg");

            img->setPixmap(pix.scaled(300,300,Qt::KeepAspectRatio));
            img->setAlignment(Qt::AlignCenter);
            name->setText(p[contador].getName());
            name->setAlignment(Qt::AlignCenter);
            name->setWordWrap(true);//Si el texto no abarca el ancho, se realiza un salto de linea
            price->setText("$" + QString::number(p[contador].getPrice()));
            price->setAlignment(Qt::AlignCenter);

            contTwoGL->addWidget(img,Qt::AlignCenter);
            contTwoGL->addWidget(name);
            contTwoGL->addWidget(price);
            contWidget->setLayout(contTwoGL);
            contGL->addWidget(contWidget,i,j);
            contador++;
        }
    }
    ui->scrollContents->setLayout(contGL);
}

void ProductWidget::deleteWidgets()
{
    if(ui->scrollContents->layout() != NULL){
        QLayoutItem* item;
        while((item = ui->scrollContents->layout()->takeAt(0)) != NULL){
            delete item->widget();
            delete item;
        }
        delete ui->scrollContents->layout();
    }
}

void ProductWidget::deleteProducts()
{
    products.clear();
}

void ProductWidget::on_optionCB_currentIndexChanged(int index)
{
    deleteWidgets();
    loadObjectsByDepartment(index);
}

void ProductWidget::on_sortCB_currentIndexChanged(int index)
{
    deleteWidgets();
    loadObjectsBySort(products,index);
}

void ProductWidget::on_searchLE_textEdited(const QString &arg1)
{
    Q_UNUSED(arg1);
    deleteWidgets();
    loadObjectsBySearch(products);
}
