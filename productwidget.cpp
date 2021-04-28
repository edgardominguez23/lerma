#include "productwidget.h"
#include "ui_productwidget.h"
#include <QPixmap>
#include <QDebug>

ProductWidget::ProductWidget(QString imageFile, QString id, QString name, double price, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProductWidget), productId(id)
{
    ui->setupUi(this);
    QPixmap pix(imageFile);
    ui->nameL->setText(name);
    ui->nameL->setAlignment(Qt::AlignCenter);
    ui->imgL->setPixmap(pix.scaled(300,300,Qt::KeepAspectRatio));
    ui->imgL->setAlignment(Qt::AlignCenter);
    ui->priceL->setText(QString::number(price));
    ui->priceL->setAlignment(Qt::AlignCenter);
}

ProductWidget::~ProductWidget()
{
    delete ui;
}

void ProductWidget::on_addPB_clicked()
{
    qDebug() << "Id secgundo: " << productId;

    emit addItem(productId,ui->amountSB->value());
    ui->amountSB->setValue(0);
}

void ProductWidget::on_amountSB_valueChanged(int arg1)
{
    (arg1 > 0) ? ui->addPB->setEnabled(true) : ui->addPB->setEnabled(false);
}
