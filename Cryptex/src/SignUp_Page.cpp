#include"SignUp_page.h"
#include"ui_SignUp.h"
#include"frontpagewindow.h"
#include"LoginPass.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFont>

SignupWindow::SignupWindow(QWidget* parent)
    : QMainWindow(parent), ui3(new Ui::SignUpPass)
{
    ui3->setupUi(this);

    connect(ui3->SignUpButton, &QPushButton::clicked, this, &SignupWindow::onSignupButtonClicked);
    connect(ui3->BackButton, &QPushButton::clicked, this, &SignupWindow::onBackButtonClicked);
}

void SignupWindow::onSignupButtonClicked() {
    click_audio.play();

    QString username = ui3->UsernameInput->text();
    QString password = ui3->PassInput->text();
    QString email = ui3->EmailInput->text();
    if (username.contains(",") || password.contains(",") || email.contains(",")) {
        QMessageBox::warning(this, "Signup Failed", "Invalid symbol used in one or more field. ");
        return;
    }
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Signup Failed", "Username and Password cannot be empty.");
        return;
    }
    if (!email.contains("@")) {
        QMessageBox::warning(this, "Signup Failed", "Entered Email in Invalid ! ");
        return;
    }
    if (password.length() < 4) {
        QMessageBox::warning(this, "Signup Failed", "WE RESPECT YOUR PRIVACY! \n For a Secure account, password must have atleast a length of 4");
        return;
    }


    QFile file("users.txt");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Failed to open user file.");
        return;
    }

    bool exists = false;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        if (parts.size() >= 1 && parts[0] == username) {
            exists = true;
            break;
        }
    }

    file.close();

    if (exists) {
        QMessageBox::warning(this, "Signup Failed", "OOPS, Username is already in use !");
        return;
    }


    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << username << "," << password << "," << email << "," << "0" << "," << "0" << "," << "0" << "," << "0" << "\n";
        file.close();

        QMessageBox msgBox;
        msgBox.setText("Account Created Successfully!");
        QFont boldFont;
        boldFont.setBold(true);
        boldFont.setPointSize(12);
        msgBox.setFont(boldFont);
        msgBox.exec();

        LoginPass* loginWindow = new LoginPass();
        loginWindow->showFullScreen();
        this->close();
    }
    else {
        QMessageBox::critical(this, "Error", "Failed to open user file.");
    }
}

void SignupWindow::onBackButtonClicked() {
    click_audio.play();
    FrontPageWindow* frontpagewindow = new FrontPageWindow();
    frontpagewindow->showFullScreen();
    this->close();
}

