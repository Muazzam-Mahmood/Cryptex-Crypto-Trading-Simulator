#include "frontpagewindow.h"
#include"SignUp_page.h"
#include"LoginPass.h"
#include "adminwindow.h"
#include"TopUPwallet.h"
#include"graphwindow.h"
#include "authhelper.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QLineEdit>
#include <QSpacerItem>
#include <QLabel>
#include<QtMultimedia>
#include <QAudioOutput>
#include <QPainter>
#include <QPixmap>
#include <adminloginwindow.h>
#include <QMainWindow>

FrontPageWindow::FrontPageWindow(QWidget* parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    static bool run = true;
    if (run) {
        intro_audio.play();
        run = false;
    }
    ui->setupUi(this);  // This links the UI

    connect(ui->AdminButton, &QPushButton::clicked, this, &FrontPageWindow::onAdminButtonClicked);
    connect(ui->SignupButton, &QPushButton::clicked, this, &FrontPageWindow::onSignupButtonClicked);
    connect(ui->LoginButton, &QPushButton::clicked, this, &FrontPageWindow::onLoginButtonClicked);
    connect(ui->DemoButton, &QPushButton::clicked, this, &FrontPageWindow::onDemoButtonClicked);
    connect(ui->qqqq, &QPushButton::clicked, this, &FrontPageWindow::close);

}

void FrontPageWindow::onAdminButtonClicked() {

    click_audio.play();
    AdminLoginWindow* adminLogin = new AdminLoginWindow();
    adminLogin->showFullScreen();
    this->close();


}

void FrontPageWindow::onSignupButtonClicked() {
    click_audio.play();
    SignupWindow* SignUpWindow = new SignupWindow;
    SignUpWindow->showFullScreen();
    this->close();
}

void FrontPageWindow::onLoginButtonClicked() {
    click_audio.play();
    LoginPass* LoginPasss = new LoginPass();
    LoginPasss->showFullScreen();
    this->close();
}

void FrontPageWindow::onDemoButtonClicked() {
    click_audio.play();
    remove("transactions_DEMO_ACCOUNT.txt");//Removes the already present txt of the recent demo account
    QMessageBox::information(this, "Demo Simulator Initiated ", "Trial Funds have been Credited to Trial Account !");
    
    LoginPageWindow* logindemo = new LoginPageWindow("DEMO_ACCOUNT");
    {
        TopUPwallet INITIAL("DEMO_ACCOUNT");//iss ka sirf ye kaam hai kae account balance reset krdae------
        INITIAL.updateWalletFile(0, 1);//iss mai second parameter jb 1 hota tou balance 100000 pr hardcode hojata (zero krne ki zaroorat nhi, hamesha 10000 hi hoga startup pr)
        GraphWindow INITIALL("DEMO_ACCOUNT");//coins ko reset krne kae liye,kyu kae coins ki access sirf idher sae aaskti
        INITIALL.updateUserBalance("NONE", -1, true);//jb second parameter -1 ho tou saare coins zero pr hardcode
    }
    

    this->close();
    
    logindemo->showFullScreen();

}

