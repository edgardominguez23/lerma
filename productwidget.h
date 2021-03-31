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

private:
    Ui::ProductWidget *ui;

    QFile dbFile;
    QString nameFile;
    QJsonArray dbArrayObjects;

    void loadAllDepartmets();
    void loadAllObjects();
    void loadObjectsDepartament(QString id);
    ////Prueba
    void cargarBotones(int index);
    void deleteBotones();
};

#endif // PRODUCTWIDGET_H
