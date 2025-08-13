#include "LoginPageWindow.h"
#include"frontpagewindow.h"
#include <QFont>
#include"TopUPwallet.h"
#include"walletwindow.h"
#include <graphwindow.h>
#include <profitlosswindow.h>
#include <QVBoxLayout>
#include <QTextEdit>     
#include <QScrollArea>    


LoginPageWindow::LoginPageWindow(const QString& username, QWidget* parent)
    : QMainWindow(parent), ui2(new Ui::LoginPage)
{   
    ui2->setupUi(this);
    usernamme = username;
    bool isdemo = false;
    
    

    QFile file("users.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Unable to open users.txt");
        return;
    }

    QTextStream in(&file);
    bool userFound = false;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        QStringList parts = line.split(",");

        if (parts.size() == 7) {
            QString storedUsername = parts[0].trimmed();

            if (storedUsername.compare(username, Qt::CaseInsensitive) == 0) {
                QString btc = parts[4].trimmed();
                QString eth = parts[5].trimmed();
                QString sol = parts[6].trimmed();
                double balance = parts[3].toDouble();
                ui2->User_Name_label->setText("Welcome back " + username);
                ui2->BitCoin_Label->setText(  btc + " BTC");
                ui2->ETH_Label->setText( eth + " ETH");
                ui2->SOL_Button->setText(sol + " SOL");
                ui2->BalanceLabel->setText("$" + QString::number(balance, 'f', 2));
                userFound = true;
                break;
            }
        }
    }
    if (username == "DEMO_ACCOUNT") {
        
        isdemo = true;
        ui2->TopUpButton->hide();
        ui2->User_Name_label->setText("Demo Account Initiated");
    }
    if (!userFound) {
        ui2->BitCoin_Label->setText("User not found");
        ui2->ETH_Label->clear();
        ui2->SOL_Button->clear();
    }

    file.close();

    connect(ui2->TopUpButton, &QPushButton::clicked, this, &LoginPageWindow::onTopUpWalletClicked);
    connect(ui2->TransHistoryButton, &QPushButton::clicked, this, &LoginPageWindow::onHistoryClicked);
    connect(ui2->ProfitLossButton, &QPushButton::clicked, this, &LoginPageWindow::onProfitLossClicked);
    connect(ui2->TradeButton, &QPushButton::clicked, this, &LoginPageWindow::onTradeClicked);
    connect(ui2->BackButton, &QPushButton::clicked, this, &LoginPageWindow::onBackClicked);
}

void LoginPageWindow::onTopUpWalletClicked() {
    click_audio.play();
    TopUPwallet* TopUPWindow = new TopUPwallet(usernamme);
    TopUPWindow->showFullScreen(); 
    this->close();
}

void LoginPageWindow::onHistoryClicked() {
    click_audio.play();


    QString filename = "transactions_" + usernamme + ".txt";
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::information(this, "History", "No transaction history found.");
        return;
    }

    QString html = "<html><body>";
    html += "<h2 style='text-align:center;'>📜 Your Transaction History</h2>";
    html += "<table border='1' cellspacing='0' cellpadding='6' style='width:100%;'>";
    html += "<tr><th>Time</th><th>Crypto</th><th>Type</th><th>Amount</th><th>Price</th></tr>";

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        if (parts.size() >= 5) {
            html += QString("<tr><td>%1</td><td>%2</td><td>%3</td><td>%4</td><td>%5</td></tr>")
                .arg(parts[0])
                .arg(parts[1])
                .arg(parts[2])
                .arg(parts[3])
                .arg(parts[4]);
        }
    }
    file.close();
    html += "</table></body></html>";

    // Create dialog
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("Transaction History");
    dialog->resize(400, 400);

    // Create scrollable text view
    QTextEdit* textEdit = new QTextEdit(dialog);
    textEdit->setReadOnly(true);
    textEdit->setHtml(html);

    // Layout and OK button
    QVBoxLayout* layout = new QVBoxLayout(dialog);
    layout->addWidget(textEdit);

    QPushButton* okButton = new QPushButton("OK");
    connect(okButton, &QPushButton::clicked, dialog, &QDialog::accept);
    layout->addWidget(okButton, 0, Qt::AlignCenter);

    dialog->setLayout(layout);
    dialog->exec();

}

void LoginPageWindow::onProfitLossClicked() {
    click_audio.play();
    ProfitLossWindow* plWindow = new ProfitLossWindow(usernamme, this);
    plWindow->exec();
}

void LoginPageWindow::onTradeClicked() {
    click_audio.play();
    GraphWindow* graphWindow = new GraphWindow(usernamme);  
    graphWindow->showFullScreen();
    this->close();
}

void LoginPageWindow::onBackClicked() {
    click_audio.play();

    FrontPageWindow* frontpagewindow = new FrontPageWindow();
    frontpagewindow->showFullScreen();
    this->close();
    this->deleteLater();
}
