#include "productwidget.h"
#include "ui_productwidget.h"
#include <QGridLayout>
#include <QPushButton>
#include <QDebug>

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

    loadAllDepartmets();
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

    for (int i(0); i < dbArrayObjects.size(); ++i) {
        QJsonObject obj = dbArrayObjects[i].toObject();
        qDebug() << obj["id"].toString();
        qDebug() << obj["name"].toString();
        qDebug() << obj["price"].toDouble();
    }
    /////////////////////////////////////////////////////////////////////////////////////
    QGridLayout *contGL = new QGridLayout(this);
    for (int i = 0;i < 5 ;i++) {
        for (int j = 0;j < 3 ; j++) {
            QPushButton *btn = new QPushButton("Hello, how are you");

            contGL->addWidget(btn,i,j);
        }
    }
    ui->scrollContents->setLayout(contGL);
}


