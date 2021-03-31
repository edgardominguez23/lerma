#include "productwidget.h"
#include "ui_productwidget.h"
#include <QPixmap>
///////////////////////Prueba
#include <QPushButton>
#include <QVBoxLayout>

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
    //loadAllDepartmets();
}

ProductWidget::~ProductWidget()
{
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

void ProductWidget::loadAllDepartmets()
{
    QGridLayout *contGL = new QGridLayout(this);
    int cont = 0;
    bool bandera = true;

    for (int i = 0;i <= dbArrayObjects.size() / 4 ;i++) {
        for (int j = 0;j < 4 ; j++) {
            if(cont > dbArrayObjects.size() - 1) bandera  = false;

            if(bandera){
                QJsonObject obj = dbArrayObjects[cont].toObject();

                QLabel *img = new QLabel();
                QLabel *name = new QLabel();
                QLabel *price = new QLabel();                
                QPixmap pix("C:/Users/Edyal/Documents/seminarioAlgoritmia/Proyecto/lerma/imgs/"+obj["id"].toString()+".jpg");

                img->setPixmap(pix.scaled(200,200,Qt::KeepAspectRatio));
                name->setText(obj["name"].toString());
                //name->setFixedWidth(167772);
                //name->setStyleSheet("max-width:10.5em; display:block;");
                price->setText(QString::number(obj["price"].toDouble()));

                contGL->addWidget(img,i*4,j,Qt::AlignCenter);
                contGL->addWidget(name,(i*4)+1,j,Qt::AlignCenter);
                contGL->addWidget(price,(i*4)+2,j,Qt::AlignCenter);

                cont++;
            }
        }
        if(!bandera) break;
    }
    ui->scrollContents->setLayout(contGL);
}

void ProductWidget::loadAllObjects()
{
    QJsonObject jsonObj;
    QJsonDocument jsonDoc;
    QByteArray data;

    dbFile.setFileName(nameFile);

    dbFile.open(QIODevice::ReadOnly);
    data = dbFile.readAll();
    dbFile.close();

    jsonDoc = QJsonDocument::fromJson(data);
    jsonObj = jsonDoc.object();
    dbArrayObjects = jsonObj["products"].toArray();
}

void ProductWidget::loadObjectsDepartament(QString id)
{
    QGridLayout *contGL = new QGridLayout(this);
    int cont = 0;
    bool bandera = true;

    for (int i = 0;i <= dbArrayObjects.size() / 4 ;i++) {
        for (int j = 0;j < 4 ; j++) {
            if(cont > dbArrayObjects.size() - 1) bandera  = false;
            QJsonObject obj = dbArrayObjects[cont].toObject();

            QString x = obj["id"].toString();
            qDebug() << x.indexOf(id);


            if(bandera){

                QLabel *img = new QLabel();
                QLabel *name = new QLabel();
                QLabel *price = new QLabel();
                QPixmap pix("C:/Users/Edyal/Documents/seminarioAlgoritmia/Proyecto/lerma/imgs/"+obj["id"].toString()+".jpg");

                img->setPixmap(pix.scaled(200,200,Qt::KeepAspectRatio));
                name->setText(obj["name"].toString());
                //name->setFixedWidth(167772);
                //name->setStyleSheet("max-width:10.5em; display:block;");
                price->setText(QString::number(obj["price"].toDouble()));

                contGL->addWidget(img,i*4,j,Qt::AlignCenter);
                contGL->addWidget(name,(i*4)+1,j,Qt::AlignCenter);
                contGL->addWidget(price,(i*4)+2,j,Qt::AlignCenter);

                cont++;
            }
        }
        if(!bandera) break;
    }
    ui->scrollContents->setLayout(contGL);
}

void ProductWidget::cargarBotones(int index){
    QVBoxLayout *lay = new QVBoxLayout(this);
    for(int i(0); i < index + 1; i++){
        QPushButton *b = new QPushButton("Hello world");
        lay->addWidget(b);
    }
    ui->scrollContents->setLayout(lay);
}

void ProductWidget::deleteBotones()
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
    if(index == 0){
        //loadAllDepartmets();
        cargarBotones(index);
    }else if(index == 1){
        //loadObjectsDepartament("AB");
        //cargarBotones(index);
        deleteBotones();
    }else if(index == 2){
        //loadObjectsDepartament("L");
        cargarBotones(index);
    }else if(index == 3){
        //loadObjectsDepartament("E");
        cargarBotones(index);
    }else if(index == 4){
        //loadObjectsDepartament("HC");
        cargarBotones(index);
    }else{
        //loadObjectsDepartament("D");
        cargarBotones(index);
    }
}
