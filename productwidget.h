#ifndef PRODUCTWIDGET_H
#define PRODUCTWIDGET_H

#include <QWidget>
#include <QString>
using namespace std;
namespace Ui {
class ProductWidget;
}

class ProductWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProductWidget(QString imageFile, QString id, QString name, double price, QWidget *parent = nullptr);
    ~ProductWidget();

private slots:
    void on_addPB_clicked();

    void on_amountSB_valueChanged(int arg1);

signals:
    void addItem(QString id, int amount);

private:
    Ui::ProductWidget *ui;
    const QString productId;
};

#endif // PRODUCTWIDGET_H
