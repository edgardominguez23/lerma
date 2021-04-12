#include "productwidget.h"
#include "ui_productwidget.h"
#include <QPixmap>
#include <QVBoxLayout>

bool firstActivation = false;

ProductWidget::ProductWidget(QString name, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProductWidget)
{
    ui->setupUi(this);

    nameFile = name;
    ui->optionCB->addItem("Todos los departamentos");
    ui->optionCB->addItem("Alimentos y Bebidas");
    ui->optionCB->addItem("Libros");
    ui->optionCB->addItem("Electronicos");
    ui->optionCB->addItem("Hogar y Cocina");
    ui->optionCB->addItem("Deporte y Aire Libre");

    loadAllObjects();
    //loadObjectsDepartament(0);
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

void ProductWidget::loadObjectsDepartament(int departmentNumber)
{
    QGridLayout *contGL = new QGridLayout(this);
    int cont = (departmentNumber > 0) ? (departmentNumber-1)*10 : 0;
    int departmentSize = (departmentNumber > 0) ? departmentNumber*10 : dbArrayObjects.size();

    for (int i = 0;i <= departmentSize / 4 ;i++) {
        for (int j = 0;j < 4 ; j++) {
            if(cont >= departmentSize) break;

            QJsonObject obj = dbArrayObjects[cont].toObject();

            QLabel *img = new QLabel();
            QLabel *name = new QLabel();
            QLabel *price = new QLabel();
            QPixmap pix("C:/Users/Edyal/Documents/seminarioAlgoritmia/Proyecto/lerma/imgs/"+obj["id"].toString()+".jpg");

            img->setPixmap(pix.scaled(200,200,Qt::KeepAspectRatio));
            name->setText(obj["name"].toString());
            name->setAlignment(Qt::AlignCenter);
            name->setWordWrap(true);//Si el texto no abarca el ancho, se realiza un salto de linea
            price->setText("$" + QString::number(obj["price"].toDouble()));

            contGL->addWidget(img,i*4,j,Qt::AlignCenter);
            contGL->addWidget(name,(i*4)+1,j); //El texto sea del ancho necesario
            contGL->addWidget(price,(i*4)+2,j,Qt::AlignCenter);

            cont++;
        }
    }
    ui->scrollContents->setLayout(contGL);
}

void ProductWidget::loadObjectsByDepartment(int departmentNumber)
{
    QGridLayout *contGL = new QGridLayout(this);
    int cont = (departmentNumber > 0) ? (departmentNumber-1)*10 : 0;
    int departmentSize = (departmentNumber > 0) ? departmentNumber*10 : dbArrayObjects.size();

    for (int i(0);i <= departmentSize / 4 ;i++ ) {
        for(int j(0); j < 4; j++){
            if(cont >= departmentSize) break;

            QJsonObject obj = dbArrayObjects[cont].toObject();

            QLabel *img = new QLabel();
            QLabel *name = new QLabel();
            QLabel *price = new QLabel();
            QWidget *contWidget = new QWidget();
            //QGridLayout *contTwoGL = new QGridLayout();
            QVBoxLayout *contTwoGL = new QVBoxLayout();
            QPixmap pix("C:/Users/Edyal/Documents/seminarioAlgoritmia/Proyecto/lerma/imgs/"+obj["id"].toString()+".jpg");

            img->setPixmap(pix.scaled(300,300,Qt::KeepAspectRatio));
            img->setAlignment(Qt::AlignCenter);
            name->setText(obj["name"].toString());
            name->setAlignment(Qt::AlignCenter);
            name->setWordWrap(true);//Si el texto no abarca el ancho, se realiza un salto de linea
            price->setText("$" + QString::number(obj["price"].toDouble()));
            price->setAlignment(Qt::AlignCenter);

            contTwoGL->addWidget(img,Qt::AlignCenter);
            contTwoGL->addWidget(name);
            contTwoGL->addWidget(price);
            contWidget->setLayout(contTwoGL);
            contWidget->setStyleSheet("background-color:red;");

            contGL->addWidget(contWidget,i,j);
            cont++;
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

void ProductWidget::on_optionCB_currentIndexChanged(int index)
{
    if (firstActivation){
        deleteWidgets();
        loadObjectsByDepartment(index);
    }
    firstActivation = true;
}
