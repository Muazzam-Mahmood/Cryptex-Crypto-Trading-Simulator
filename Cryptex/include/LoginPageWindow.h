#pragma once


#include "TopUPwallet.h"
#include <QWidget> 
#include <QPushButton>
#include <QLabel>
#include <QDialog>
#include"ui_LoginPage.h"

class LoginPageWindow : public QMainWindow {
    Q_OBJECT

public:
    LoginPageWindow(const QString& username, QWidget* parent = nullptr);
    QString usernamme;
    ClickSound click_audio;

private slots:
    void onTopUpWalletClicked();
    //void onCheckBalanceClicked();
    //void onCheckAssetsClicked();
    void onHistoryClicked();
    void onProfitLossClicked();
    void onTradeClicked();
    void onBackClicked();

private:
    QLabel* welcomeLabel;
    QPushButton* topUpWalletButton;
    QPushButton* checkBalanceButton;
    QPushButton* checkAssetsButton;
    QPushButton* historyButton;
    QPushButton* profitLossButton;
    QPushButton* tradeButton;
    QPushButton* backButton;


    Ui::LoginPage* ui2;

    
};

