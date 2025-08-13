#include "adminloginwindow.h"
#include "adminwindow.h"
#include "authhelper.h"
#include"frontpagewindow.h"
#include <QMessageBox>

AdminLoginWindow::AdminLoginWindow(QWidget* parent)
    : QMainWindow(parent),ui4(new Ui::AdminPass)
{
    ui4->setupUi(this);

    connect(ui4->LoginButton, &QPushButton::clicked, this, &AdminLoginWindow::onLoginButtonClicked);
    connect(ui4->BackButton, &QPushButton::clicked, this, &AdminLoginWindow::onBackButtonClicked);
}

void AdminLoginWindow::onLoginButtonClicked() {

    click_audio.play();

    QString adminId = ui4->UsernameInput->text();
    QString adminPass = ui4->PassInput->text();
    if (adminId.contains(",") || adminPass.contains(",")) {
        QMessageBox::warning(this, "Admin Login Failed", "Invalid symbol used in one or more field. ");
    }
    if (isAdminValid(adminId, adminPass)) {
        AdminWindow* adminWindow = new AdminWindow();
        adminWindow->showFullScreen();
        this->close();
    }
    else {
        QMessageBox::warning(this, "Login Failed", "Invalid Admin credentials.");
    }
}

void AdminLoginWindow::onBackButtonClicked() {

    click_audio.play();
    
    FrontPageWindow* frontpagewindow = new FrontPageWindow();
    frontpagewindow->showFullScreen();

    this->close();
}


