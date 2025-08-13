#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>  
#include"audio.h"
#include"ui_AdminPage.h"

class AdminWindow : public QMainWindow
{
    Q_OBJECT

public:
    AdminWindow(QWidget* parent = nullptr);
    ~AdminWindow();

private slots:
    void onViewCurrentUsersClicked();
    void onChangeInfoClicked();
    void onUsersTradingReportsClicked();
    void onChangeRoleClicked();
    void onDeleteUserClicked();
    void onSeeCurrentPricesClicked();
    void onBackButtonClicked();  
    

private:
    QPushButton* viewUsersButton;
    QPushButton* changeInfoButton;
    QPushButton* tradingReportsButton;
    QPushButton* changeRoleButton;
    QPushButton* deleteUserButton;
    QPushButton* currentPricesButton;
    QPushButton* backButton;  
    
    QLabel* welcomeLabel;    
    QLabel* adminNameLabel;  

    ClickSound click_audio;
    
    Ui::AdminPage* ui5;
    
};