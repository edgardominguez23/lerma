#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::enableLoginPB()
{
    if(ui->usernameLE->text().length() > 0 && ui->passwordLE->text().length() > 0){
        ui->loginPB->setEnabled(true);
    }else{
        ui->loginPB->setEnabled(false);
    }
}

void MainWindow::enableSignInPB()
{
    if(ui->newUserLE->text().length() > 0 && ui->emailLE->text().length() > 0 && ui->newPasswordLE->text().length() > 0){
        ui->signInPB->setEnabled(true);
    }else{
        ui->signInPB->setEnabled(false);
    }
}

void MainWindow::validateUser()
{
    QMessageBox message;
    vector<User>::iterator it;
    QString user = ui->usernameLE->text();
    QString password = ui->passwordLE->text();

    it = find_if(users.begin(), users.end(), [&user, &password](User u) -> bool{
        return u.getName() == user && u.getPassword() == password;
    }
    );

    if(it == users.end()){
        message.setText("Invalid credentials");
        message.setIcon(QMessageBox::Warning);
        message.exec();
    }else{
        message.setText("Welcome to LERMA " + user);
        ui->viewSW->setCurrentIndex(1);
        message.exec();
    }
}

bool MainWindow::validateEmail()
{
    QString email = ui->emailLE->text();
    QString expression = "^[_a-z0-9-]+(\\.[_a-z0-9-]+)*@[a-z0-9-]+(\\.[a-z0-9-]+)*(\\.[a-z]{2,4})$";
    QRegularExpression validate(expression);

    return validate.match(email).hasMatch();
}



void MainWindow::on_usernameLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enableLoginPB();
}

void MainWindow::on_passwordLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enableLoginPB();
}

void MainWindow::on_newUserLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enableSignInPB();
}

void MainWindow::on_emailLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enableSignInPB();
}

void MainWindow::on_newPasswordLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enableSignInPB();
}

void MainWindow::on_signInPB_clicked()
{
    QMessageBox message;
    User u;

    if(validateEmail()){
        u.setName(ui->newUserLE->text());
        u.setEmail(ui->emailLE->text());
        u.setPassword(ui->newPasswordLE->text());

        users.push_back(u);

        message.setText("New user created");
        message.exec();

        ui->newUserLE->clear();
        ui->emailLE->clear();
        ui->newPasswordLE->clear();
    }else{
        message.setText("Email is invalid");
        message.setIcon(QMessageBox::Warning);
        message.exec();
        ui->emailLE->clear();
    }
}

void MainWindow::on_loginPB_clicked()
{
    validateUser();
    ui->usernameLE->clear();
    ui->passwordLE->clear();
}
