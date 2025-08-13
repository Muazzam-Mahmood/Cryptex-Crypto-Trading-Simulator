#include "adminwindow.h"
#include"frontpagewindow.h"
#include <QMessageBox>
#include <QFont>  
#include <QVBoxLayout>  
#include "viewuserswindow.h"
#include "deleteuserwindow.h"
#include <edituserwindow.h>
#include <changerolewindow.h>
#include<qfile.h>
#include"userreportswindow.h"
#include"graphwindow.h"



AdminWindow::AdminWindow(QWidget* parent)
    : QMainWindow(parent),ui5(new Ui::AdminPage)
{   
    ui5->setupUi(this);

    connect(ui5->ViewUserButton, &QPushButton::clicked, this, &AdminWindow::onViewCurrentUsersClicked);
    connect(ui5->ChangInfoButton, &QPushButton::clicked, this, &AdminWindow::onChangeInfoClicked);
    connect(ui5->UserTradingRepoButton, &QPushButton::clicked, this, &AdminWindow::onUsersTradingReportsClicked);
    connect(ui5->PromoteToAdminButton, &QPushButton::clicked, this, &AdminWindow::onChangeRoleClicked);
    connect(ui5->DeleteUserButton, &QPushButton::clicked, this, &AdminWindow::onDeleteUserClicked);
    connect(ui5->CurrentPriceButton, &QPushButton::clicked, this, &AdminWindow::onSeeCurrentPricesClicked);
    connect(ui5->BackButton, &QPushButton::clicked, this, &AdminWindow::onBackButtonClicked);
}

AdminWindow::~AdminWindow() {}

void AdminWindow::onViewCurrentUsersClicked() {
    click_audio.play();
    ViewUsersWindow* viewUsersWin = new ViewUsersWindow();
    viewUsersWin->showFullScreen();
}
void AdminWindow::onChangeInfoClicked()
{
    click_audio.play();

    EditUserWindow* editWindow = new EditUserWindow();
    editWindow->showFullScreen();
}
void AdminWindow::onUsersTradingReportsClicked() {
    click_audio.play();

    UserReportsWindow* reportWindow = new UserReportsWindow();  
    reportWindow->setWindowFlags(Qt::Window);                   
    reportWindow->setWindowTitle("User Trading Reports");
    reportWindow->resize(800, 600);                            
    reportWindow->show();
}
void AdminWindow::onChangeRoleClicked() {
    click_audio.play();
    ChangeRoleWindow* roleWindow = new ChangeRoleWindow();
    roleWindow->show();
}
void AdminWindow::onDeleteUserClicked() {
    click_audio.play();
    DeleteUserWindow* deleteUserWin = new DeleteUserWindow();
    deleteUserWin->show();
}
void AdminWindow::onSeeCurrentPricesClicked() {
    GraphWindow* graphwindow = new GraphWindow("Admin");
    graphwindow->showFullScreen();
    click_audio.play();
    this->close();
}
void AdminWindow::onBackButtonClicked() {
    click_audio.play();
    FrontPageWindow* frontpagewindow = new FrontPageWindow;
    frontpagewindow->showFullScreen();
    this->close();
}
