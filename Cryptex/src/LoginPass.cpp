#include"LoginPass.h"
#include"ui_LoginPass.h"
#include"frontpagewindow.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

LoginPass::LoginPass(QWidget* parent)
    : QMainWindow(parent), ui3(new Ui::LoginPass)
{
    ui3->setupUi(this);
    connect(ui3->LoginButton, &QPushButton::clicked, this, &LoginPass::onLoginButtonClicked);
    connect(ui3->BackButton, &QPushButton::clicked, this, &LoginPass::onBackButtonClicked);
}

void LoginPass::onLoginButtonClicked() {
    click_audio.play();
    QString username = ui3->UsernameInput->text();
    QString password = ui3->PassInput->text();
    if (username.contains(",") || password.contains(",") ) {
        QMessageBox::warning(this, "Login Failed", "Invalid symbol used in one or more field. ");
        return;
    }
    if (username.isEmpty() || password.isEmpty()) {
        ui3->textEdit_7->setTextColor(Qt::red);
        ui3->textEdit_7->setText("Kindly fill in all fields");
        return;
    }

    QFile file("users.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Could not open user file.");
        return;
    }

    QTextStream in(&file);
    bool found = false;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        if (parts.size() >= 2 && parts[0] == username && parts[1] == password) {
            found = true;
            break;
        }
    }

    file.close();
    
    if (found) {
        ui3->textEdit_7->setTextColor(Qt::green);
        ui3->textEdit_7->setText("Login SuccessFull !");
        QMessageBox::information(this, "Login", "Welcome " + username + "!");
        LoginPageWindow* dashboard = new LoginPageWindow(username);
        dashboard->showFullScreen();
        this->close();
    }
    else {
        ui3->textEdit_7->setTextColor(Qt::red);
        ui3->textEdit_7->setText("Invalid Credentials. Try Again");
        QMessageBox::information(this, "Login Failed", "One or more info entered was wrong");
    }
}

void LoginPass::onBackButtonClicked() {
    click_audio.play();
    FrontPageWindow* frontpagewindow = new FrontPageWindow();
    frontpagewindow->showFullScreen();
    this->close();
}
