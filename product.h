#ifndef PRODUCT_H
#define PRODUCT_H
#include <QString>

class Product
{
public:
    Product();
    QString getId() const;
    void setId(const QString &value);

    QString getName() const;
    void setName(const QString &value);

    double getPrice() const;
    void setPrice(double value);

private:
    QString id;
    QString name;
    double price;
};

#endif // PRODUCT_H
