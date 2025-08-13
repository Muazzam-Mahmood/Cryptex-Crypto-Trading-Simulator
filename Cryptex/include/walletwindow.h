#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include"audio.h"     


class WalletWindow : public QWidget
{
    Q_OBJECT

public:
    explicit WalletWindow(const QString& username, QWidget* parent = nullptr);
    ClickSound click_audio;
private:
    QLabel* walletLabel;     
    QLabel* balanceLabel;    
    QPushButton* backButton;

    void displayBalance(const QString& username); 
private slots:
    void onBackClicked(); 
};



