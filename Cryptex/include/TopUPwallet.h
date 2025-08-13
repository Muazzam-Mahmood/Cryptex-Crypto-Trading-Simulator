#pragma once


#include"ui_TopUpPage.h"

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include"audio.h"
class TopUPwallet : public QMainWindow
{
    Q_OBJECT

public:
    explicit TopUPwallet(const QString& username, QWidget* parent = nullptr);
    bool isdemo = false;
    void updateWalletFile(double amountToAdd,int DEMOO=0);
    ClickSound click_audio;
    Transaction transation_audio;
private slots:
    void handleSubmit();
    void handleBack();

private:

    QLineEdit* cardNumberInput;
    QLineEdit* mpinInput;
    QLineEdit* amountInput;
    QPushButton* submitButton;
    QPushButton* backButton;

    QString currentUsername; 
    QString walletFilePath = "users.txt"; 

    Ui::TopUpPage* ui6;
};

 
