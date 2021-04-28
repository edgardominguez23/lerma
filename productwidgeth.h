#ifndef PRODUCTWIDGETH_H
#define PRODUCTWIDGETH_H

#include <QWidget>
#include <QFile>
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QGridLayout>
#include <QPushButton>
#include <QDebug>
#include <QWidget>
#include <QLabel>
#include <vector>

#include "product.h"

using namespace std;

namespace Ui {
class ProductWidget;
}

class ProductWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProductWidget(QString name,QWidget *parent = nullptr);
    ~ProductWidget();

    QString getNameFile() const;
    void setNameFile(const QString &value);

private slots:
    void on_optionCB_currentIndexChanged(int index);

    void on_sortCB_currentIndexChanged(int index);

    void on_searchLE_textEdited(const QString &arg1);

    void on_amoutSB_valueChanged(int arg1);

    void on_addPB_clicked();

private:
    Ui::ProductWidget *ui;

    QFile dbFile;
    QString nameFile;
    QJsonArray dbArrayObjects;
    vector<Product> products;

    void loadAllObjects();
    void loadObjectsByDepartment(int departmentNumber);
    void loadObjectsBySort(vector<Product> &p, int sortNumber);
    void loadObjectsBySearch(const vector<Product> &p);
    void showProducts(const vector<Product> &p);
    void deleteWidgets();
    void deleteProducts();
};

#endif // PRODUCTWIDGETH_H
