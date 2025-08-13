#include "TopUPwallet.h"
#include"LoginPageWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFont>

TopUPwallet::TopUPwallet( const QString& username, QWidget* parent)
    : QMainWindow(parent),ui6(new Ui::TopUpPage)
{
    ui6->setupUi(this);
    currentUsername = username;
    connect(ui6->SignUpButton, &QPushButton::clicked, this, &TopUPwallet::handleSubmit);
    connect(ui6->BackButton, &QPushButton::clicked, this, &TopUPwallet::handleBack);
}





void TopUPwallet::handleSubmit()
{
    QString cardNumber = ui6->CreditNumInput->text().trimmed();
    QString mpin = ui6->MPinInput->text().trimmed();
    QString amountStr = ui6->TopUpInput->text().trimmed();
    bool ok;
    double amount = amountStr.toDouble(&ok);

    if (cardNumber.isEmpty() || mpin.isEmpty() || amountStr.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please fill all fields!");
        return;
    }
    
    if (cardNumber.length() != 8 || cardNumber.toLongLong() == 0) {
        QMessageBox::warning(this, "Invalid Card Number", "Please enter a valid 8-digit number.");
        return;
    }
    
    if (mpin.length() != 6|| mpin.toLongLong() == 0) {
        QMessageBox::warning(this, "Invalid MPIN", "The MPIN must have 6-Digits!");
        return;
    }

    if (amountStr.length()>7) {
        QMessageBox::warning(this, "Too large Top-up Amount", "Whoa , Chill out bro !\n We dont want the FBR get behind us ;) ");
        return;
    }

    if (!ok || amount <= 0) {
        QMessageBox::warning(this, "Error", "Please enter a valid positive amount!");
        return;
    }
    updateWalletFile(amount);
}
void TopUPwallet::updateWalletFile(double amountToAdd,int DEMOO)
{   
    bool isdemo = false;
    if (currentUsername == "DEMO_ACCOUNT") {
        isdemo = true;
    }

    QFile file("users.txt"); 
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Unable to open users file!");
        return;
    }

    QStringList updatedLines;
    QTextStream in(&file);
    bool userFound = false;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();

        QStringList parts = line.split(",");

        if (parts.size() == 7) {
            QString username = parts[0].trimmed(); 
            double oldBalance = parts[3].toDouble();  



            if (username.compare(currentUsername.trimmed(), Qt::CaseInsensitive) == 0) {
                if (DEMOO==1) {
                    double newBalance = 10000000; //for demo account
                    parts[3] = QString::number(newBalance, 'f', 2); 
                    line = parts.join(","); 
                    userFound = true;
                }
                else {
                    double newBalance = oldBalance + amountToAdd; 
                    parts[3] = QString::number(newBalance, 'f', 2);  
                    line = parts.join(","); 
                    userFound = true;
                }
            }
        }

        updatedLines.append(line);  
    }

    file.close(); 
    if (!userFound) {
        QMessageBox::warning(this, "Error", "User could not be found");
        return;
    }

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QMessageBox::warning(this, "Error", "Unable to open users file for writing!");
        return;
    }

    QTextStream out(&file);
    for (const QString& line : updatedLines) {
        out << line << "\n"; 
    }

    file.close();

    if (currentUsername != "DEMO_ACCOUNT") {
        transation_audio.play();
        QMessageBox::information(this, "Success", "Top-Up Successful!\nAdded Amount: $" + QString::number(amountToAdd, 'f', 2));
    }

}


void TopUPwallet::handleBack()
{
    click_audio.play();
    LoginPageWindow* LoginPage = new LoginPageWindow(currentUsername);
    LoginPage->showFullScreen();
    this->close(); 
}
