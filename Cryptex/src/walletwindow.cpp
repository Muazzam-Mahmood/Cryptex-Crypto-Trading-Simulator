#include "WalletWindow.h"
#include <QLabel>
#include <QPushButton>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFont>


WalletWindow::WalletWindow(const QString& username, QWidget* parent)
    : QWidget(parent)
{
    setFixedSize(1280, 1024);
    setWindowTitle("Wallet Window");

    walletLabel = new QLabel("Your Wallet Balance: ", this);
    walletLabel->setGeometry(470, 300, 400, 40);
    walletLabel->setFont(QFont("Arial", 24, QFont::Bold));


    balanceLabel = new QLabel(this);
    balanceLabel->setGeometry(400, 400, 500, 80); 
    balanceLabel->setFont(QFont("Arial", 24, QFont::Bold));
    balanceLabel->setAlignment(Qt::AlignCenter);  

    backButton = new QPushButton("Back", this);
    backButton->setGeometry(580, 500, 120, 40);
    backButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #2980b9;"  
        "   color: white;"
        "   border-radius: 6px;"       
        "   padding: 8px 15px;"
        "   font-weight: bold;"
        "   border: none;"
        "}"
        "QPushButton:hover {"           
        " border: 2px solid #a0a0a0;"  
        "}"
    );

    connect(backButton, &QPushButton::clicked, this, &WalletWindow::onBackClicked);
    displayBalance(username);
}

void WalletWindow::displayBalance(const QString& username)
{
    QFile file("users.txt"); 
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Unable to open users file!");
        return;
    }

    QTextStream in(&file);
    bool userFound = false;
    double balance = 0.0;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        QStringList parts = line.split(",");

        if (parts.size() == 7) {
            QString storedUsername = parts[0].trimmed();
            if (storedUsername.compare(username, Qt::CaseInsensitive) == 0) {
                balance = parts[3].toDouble();
                userFound = true;
                break;
            }
        }
    }

    file.close();
    
    if (userFound) {
        balanceLabel->setText("Balance: $" + QString::number(balance, 'f', 2));
    }
    else {
        balanceLabel->setText("User not found");
    }

}
void WalletWindow::onBackClicked()
{
    click_audio.play();
    close();  
}
