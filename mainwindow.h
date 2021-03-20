#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <vector>
#include "user.h"
#include <QRegularExpression>
#include <QAction>
#include <QFile>
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include "productwidget.h"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_usernameLE_textChanged(const QString &arg1);

    void on_passwordLE_textChanged(const QString &arg1);

    void on_newUserLE_textChanged(const QString &arg1);

    void on_emailLE_textChanged(const QString &arg1);

    void on_newPasswordLE_textChanged(const QString &arg1);

    void on_signInPB_clicked();

    void on_loginPB_clicked();

    void openFile();

private:
    Ui::MainWindow *ui;

    //ProductWidget *uiProducts;
    ProductWidget *uiProduct;

    vector<User> users;
    QAction* openFileAction;
    QFile dbFile;
    QJsonArray dbArray, dbArrayObject;

    void enableLoginPB();
    void enableSignInPB();
    void validateUser();
    bool validateEmail();
    bool validateCredential();
    void saveDB();
    void loadBD();
};
#endif // MAINWINDOW_H
