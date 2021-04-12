#ifndef PRODUCTWIDGET_H
#define PRODUCTWIDGET_H

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

private:
    Ui::ProductWidget *ui;

    QFile dbFile;
    QString nameFile;
    QJsonArray dbArrayObjects;

    void loadAllObjects();
    void loadObjectsDepartament(int departmentNumber);
    void loadObjectsByDepartment(int departmentNumber);
    void loadObjectsBySort(int sortNumber);
    void loadObjectsBySearch(QString objectName);
    void deleteWidgets();
};

#endif // PRODUCTWIDGET_H
