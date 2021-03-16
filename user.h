#ifndef USER_H
#define USER_H
#include <QString>

class User
{
public:
    User();
    QString getName() const;
    void setName(const QString &value);

    QString getEmail() const;
    void setEmail(const QString &value);

    QString getPassword() const;
    void setPassword(const QString &value);

private:
    QString name;
    QString email;
    QString password;
};

#endif // USER_H
