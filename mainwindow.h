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
#include <iostream>

#include "productwidget.h"
#include "product.h"
#include "graph.h"

#include <queue>
#include <QRandomGenerator>

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

    void on_departmentCB_currentIndexChanged(int index);

    void on_searchLE_textEdited(const QString &arg1);

    void on_sortingCB_currentIndexChanged(int index);

    void addToChart(QString item, int amount);

private:
    Ui::MainWindow *ui;

    vector<User> users;
    vector<Product> products, productsCopia;

    QAction* openFileAction;
    QFile dbFile;
    QJsonArray dbArray, dbArrayObject, dbArrayAddProducts, dbArrayPurchase;
    Graph<string> graph;
    priority_queue<pair<int,string> > priorityProducts;

    void enableLoginPB();
    void enableSignInPB();
    void validateUser();
    bool validateEmail();
    bool validateCredential();
    void saveDB();
    void loadBD();
    void loadProductsByDepartment(int n);
    void showProducts(vector<Product> &p);
    void loadObjectsBySort(vector<Product> &p, int n);
    void loadObjectsBySearch(const vector<Product> &p);
    void deleteProducts();
    void deleteProductsPriority();
    void createGraph();

    QString generateRandomProduct(int min, int max);
    ProductWidget* searchProduct(QString id);
    void showPriorityProducts(QString id);
    void generateRandomProductInQueue(int n);
};
#endif // MAINWINDOW_H
