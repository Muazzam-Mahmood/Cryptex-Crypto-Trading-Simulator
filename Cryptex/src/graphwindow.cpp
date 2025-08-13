#include "graphwindow.h"
#include"LoginPageWindow.h"
#include"adminwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <limits>
#include <QInputDialog>
#include <QMessageBox>
#include <QDateTime>
#include <QTextStream>
#include <QScrollArea>
#include <QTextEdit>  

GraphWindow::GraphWindow(const QString& username, QWidget* parent)
    : QMainWindow(parent), username(username), maxPoints(100)
{
    CurrentUsername = username;
    this->username = username;
    timer = new QTimer(this);
    priceSeries = new QLineSeries();
    buySeries = new QScatterSeries();
    sellSeries = new QScatterSeries();
    chart = new QChart();
    chartView = new QChartView(chart);
    balanceLabel = new QLabel();
    positionLabel = new QLabel("No active position");
    priceFile = nullptr;
    priceStream = nullptr;
    axisX = nullptr;
    axisY = nullptr;
    x = 0;

    currentPrice = 0;
    buyPrice = 0;
    positionAmount = 0;
    userBalance = 0;
    hasPosition = false;
    currentCrypto = "";

    buySeries->setName("Buy Points");
    buySeries->setMarkerSize(14);
    buySeries->setColor(Qt::green);
    buySeries->setBorderColor(Qt::darkGreen);

    sellSeries->setName("Sell Points");
    sellSeries->setMarkerSize(14);
    sellSeries->setColor(Qt::red);
    sellSeries->setBorderColor(Qt::darkRed);

    setupUI();
    loadUserBalance();

    connect(timer, &QTimer::timeout, this, &GraphWindow::loadNextPoint);
}

GraphWindow::~GraphWindow()
{
    if (priceFile) {
        priceFile->close();
        delete priceFile;
    }
    delete priceStream;
    savePendingOrders();
}

void GraphWindow::setupUI()
{
    QWidget* central = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    chart->addSeries(priceSeries);
    chart->addSeries(buySeries);
    chart->addSeries(sellSeries);
    chart->legend()->setVisible(true);

    QFont TitleFont;
    TitleFont.setPointSize(30);
    chart->setTitleFont(TitleFont);

    chart->setTitle("Select a Coin to Trade");

    axisX = new QValueAxis;
    axisX->setTitleText("Time");
    axisX->setLabelFormat("%d");
    axisX->setRange(0, maxPoints);

    axisY = new QValueAxis;
    axisY->setTitleText("Price (USDT)");
    axisY->setRange(0, 100);

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    priceSeries->attachAxis(axisX);
    priceSeries->attachAxis(axisY);
    buySeries->attachAxis(axisX);
    buySeries->attachAxis(axisY);
    sellSeries->attachAxis(axisX);
    sellSeries->attachAxis(axisY);

    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(800, 500);
    mainLayout->addWidget(chartView);

    QHBoxLayout* infoLayout = new QHBoxLayout();
    balanceLabel->setStyleSheet("font-size: 16px; padding: 5px;");
    positionLabel->setStyleSheet("font-size: 16px; padding: 5px;");
    infoLayout->addWidget(balanceLabel);
    infoLayout->addWidget(positionLabel);
    
    mainLayout->addLayout(infoLayout);
    

    QHBoxLayout* cryptoBtnLayout = new QHBoxLayout();
    QPushButton* btcBtn = new QPushButton("Bitcoin");
    QPushButton* ethBtn = new QPushButton("Ethereum");
    QPushButton* solBtn = new QPushButton("Solana");
    cryptoBtnLayout->addWidget(btcBtn);
    cryptoBtnLayout->addWidget(ethBtn);
    cryptoBtnLayout->addWidget(solBtn);

    QHBoxLayout* actionBtnLayout = new QHBoxLayout();

    QPushButton* buyBtn = new QPushButton("Buy");
    QPushButton* sellBtn = new QPushButton("Sell");
    QPushButton* historyBtn = new QPushButton("History");
    QPushButton* backBtn = new QPushButton("Back");
    QPushButton* buyLimitBtn = new QPushButton("Buy Limit");
    QPushButton* sellLimitBtn = new QPushButton("Sell Limit");
    QPushButton* pendingOrdersButton = new QPushButton("Pending Orders");
  

   
    pendingOrdersButton->setStyleSheet("background-color: #607D8B; color: white;");
   
    connect(pendingOrdersButton, &QPushButton::clicked, this, &GraphWindow::showPendingOrders);

    buyLimitBtn->setStyleSheet("background-color: #8BC34A; color: white;");
    sellLimitBtn->setStyleSheet("background-color: #FF9800; color: white;");

    buyBtn->setStyleSheet("background-color: #4CAF50; color: white; padding: 5px 15px;");
    sellBtn->setStyleSheet("background-color: #F44336; color: white; padding: 5px 15px;");
    historyBtn->setStyleSheet("background-color: #2196F3; color: white; padding: 5px 15px;");
    backBtn->setStyleSheet("background-color: #555; color: white; padding: 5px 15px;");
  

    loadPendingOrders();

    if (username != "Admin") {
        actionBtnLayout->addWidget(buyBtn);
        actionBtnLayout->addWidget(sellBtn);
        actionBtnLayout->addStretch();
        actionBtnLayout->addWidget(pendingOrdersButton);
        actionBtnLayout->addWidget(historyBtn);
        actionBtnLayout->addWidget(buyLimitBtn);
        actionBtnLayout->addWidget(sellLimitBtn);
    }


    actionBtnLayout->addWidget(backBtn);
    mainLayout->addLayout(cryptoBtnLayout);
    mainLayout->addLayout(actionBtnLayout);

    connect(btcBtn, &QPushButton::clicked, this, [this]() { startGraphing("btc.txt"); });
    connect(ethBtn, &QPushButton::clicked, this, [this]() { startGraphing("eth.txt"); });
    connect(solBtn, &QPushButton::clicked, this, [this]() { startGraphing("solana.txt"); });
    connect(buyBtn, &QPushButton::clicked, this, &GraphWindow::onBuyClicked);
    connect(sellBtn, &QPushButton::clicked, this, &GraphWindow::onSellClicked);
    connect(historyBtn, &QPushButton::clicked, this, &GraphWindow::onHistoryClicked);
    connect(backBtn, &QPushButton::clicked, this, &GraphWindow::onBackClicked);
    connect(buyLimitBtn, &QPushButton::clicked, this, &GraphWindow::onBuyLimitClicked);
    connect(sellLimitBtn, &QPushButton::clicked, this, &GraphWindow::onSellLimitClicked);

    setCentralWidget(central);
    setWindowTitle("Crypto Trading - " + username);
    resize(1000, 700);
}

void GraphWindow::loadUserBalance()
{

    QFile file("users.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open users file";
        balanceLabel->setText("Balance: Error loading");
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {

        QString line = in.readLine();
        QStringList parts = line.split(",");
        if (parts[0] == username) {
            userBalance = parts[3].toDouble();
            balanceLabel->setText(QString("Balance: %1 USDT").arg(userBalance, 0, 'f', 2));
            qDebug() << "Loaded balance:" << userBalance; 
            break;
        }

    }
    file.close();
}
void GraphWindow::updateBalance()
{
    balanceLabel->setText(QString("Balance: %1 USDT").arg(userBalance, 0, 'f', 2));
    balanceLabel->repaint(); 
    qDebug() << "Updated balance display to:" << userBalance;
}
void GraphWindow::startGraphing(const QString& fileName)
{
    
    Graph_audio.play();
    timer->stop();

    if (priceFile) {
        priceFile->close();
        delete priceFile;
        priceFile = nullptr;
    }

    delete priceStream;
    priceStream = nullptr;

    priceFile = new QFile(fileName);
    if (!priceFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open" << fileName;
        return;
    }

    priceStream = new QTextStream(priceFile);
    priceSeries->clear();
    buySeries->clear();
    sellSeries->clear();
    x = 0;


    currentCrypto = fileName.split(".").first();
    currentCrypto[0] = currentCrypto[0].toUpper();

    
    chart->setTitle("Real-Time " + currentCrypto + " Price");

    axisX->setRange(0, maxPoints);
    axisY->setRange(0, 100);

    timer->start(520);
}

void GraphWindow::loadNextPoint()
{
    if (!priceStream) return;

    if (priceStream->atEnd()) {
        priceFile->seek(0);
        priceStream->seek(0);
    }

    QString line = priceStream->readLine().trimmed();
    bool ok;
    currentPrice = line.toDouble(&ok);
    positionLabel->setText(line);

    if (ok) {
        priceSeries->append(x, currentPrice);


        if (priceSeries->count() > maxPoints) {
            priceSeries->removePoints(0, 1);
        }

        updateAxes();

        checkPendingOrders();
        x++;
       
    }
    else {
        qDebug() << "Invalid data point:" << line;
    }
}

void GraphWindow::updateAxes()
{
    axisX->setRange(x - maxPoints, x);

    double minY = findMinY();
    double maxY = findMaxY();

    double range = maxY - minY;
    if (range < 1.0) {
        minY -= 0.5;
        maxY += 0.5;
    }
    else {
        minY -= range * 0.1;
        maxY += range * 0.1;
    }

    axisY->setRange(minY, maxY);
}

double GraphWindow::findMinY() const
{
    if (priceSeries->count() == 0) return 0;

    double min = std::numeric_limits<double>::max();
    for (const QPointF& point : priceSeries->points()) {
        if (point.y() < min) min = point.y();
    }
    return min;
}

double GraphWindow::findMaxY() const
{
    if (priceSeries->count() == 0) return 100;

    double max = std::numeric_limits<double>::min();
    for (const QPointF& point : priceSeries->points()) {
        if (point.y() > max) max = point.y();
    }
    return max;
}



void GraphWindow::onBuyClicked()
{
    click_audio.play();
    if (currentCrypto.isEmpty()) {
        displayer->setText("Please select a coin to graph first.");
        displayer->exec();
        return;
    }

    bool ok;
    double coinsToBuy = QInputDialog::getDouble(this, "Buy Coins", "Enter number of coins to buy:", 0, 0, 1000000, 4, &ok);
    

    if (!ok || coinsToBuy <= 0)
        return;

    double totalCost = coinsToBuy * currentPrice;
    if (totalCost > userBalance) {
        displayer->setWindowTitle("Insufficient Balance ");
        displayer->setText("Not enough balance to buy these coins.");
        displayer->setStandardButtons(QMessageBox::Ok);
        displayer->exec();
        return;
    }

    userBalance -= totalCost;

    QFile file("users.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        displayer->setText("Failed to open users.txt");
        displayer->exec();
        return;
    }

    QStringList lines;
    QString userLine;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.startsWith(username + ",")) {
            userLine = line;
        }
        lines.append(line);
    }
    file.close();

    if (userLine.isEmpty()) {
        displayer->setText("User not found.");
        displayer->exec();
        return;
    }

    QStringList parts = userLine.split(",");
    if (parts.size() < 7) {
        displayer->setText("Invalid user data.");
        displayer->exec();
        return;
    }

    int coinIndex = 4; 
    if (currentCrypto.contains("eth", Qt::CaseInsensitive)) coinIndex = 5;
    else if (currentCrypto.contains("sol", Qt::CaseInsensitive)) coinIndex = 6;

    double prevCoinAmount = parts[coinIndex].toDouble();
    prevCoinAmount += coinsToBuy;
    parts[coinIndex] = QString::number(prevCoinAmount, 'f', 4);

    parts[3] = QString::number(userBalance, 'f', 2); 

    QString updatedLine = parts.join(",");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        displayer->setText("Failed to write to users.txt");
        displayer->exec();
        return;
    }

    QTextStream out(&file);
    for (QString& line : lines) {
        if (line.startsWith(username + ",")) {
            out << updatedLine << "\n";
        }
        else {
            out << line << "\n";
        }
    }
    file.close();

    saveTransaction("Buy", currentPrice, coinsToBuy);

    buySeries->append(x-1, currentPrice);

    updateBalance();

    displayer->setText("You bought " + QString::number(coinsToBuy, 'f', 4) + " coins for $" + QString::number(totalCost, 'f', 2));
    displayer->exec();
}



void GraphWindow::onSellClicked()
{
    click_audio.play();
    if (currentCrypto.isEmpty()) {
        displayer->setText(" Kindly select a coin first for its graphing.");
        displayer->exec();
        return;
    }

    bool ok;
    double coinsToSell = QInputDialog::getDouble(this, "Sell Coins",
        "Enter number of coins to sell:", 0, 0, 1e8, 4, &ok);
    if (!ok || coinsToSell <= 0)
        return;

    QFile file("users.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        displayer->setText("Failed to open users.txt");
        displayer->exec();
        return;
    }

    QStringList lines;
    QString userLine;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.startsWith(username + ",")) {
            userLine = line;
        }
        lines.append(line);
    }
    file.close();

    if (userLine.isEmpty()) {
        displayer->setText("User not found.");
        displayer->exec();
        return;
    }

    QStringList parts = userLine.split(",");
    if (parts.size() < 7) {
        displayer->setText("Invalid user data.");
        displayer->exec();
        return;
    }

    int coinIndex = 4;
    if (currentCrypto.contains("eth", Qt::CaseInsensitive))
        coinIndex = 5;
    else if (currentCrypto.contains("sol", Qt::CaseInsensitive))
        coinIndex = 6;

    double coinBalance = parts[coinIndex].toDouble();
    if (coinBalance < coinsToSell) {
        displayer->setWindowTitle("Insufficient Assets ");
        displayer->setText(" You don't have enough coins to sell.");
        displayer->setStandardButtons(QMessageBox::Ok);
        displayer->exec();
        return;
    }

    double currentPricePerCoin = currentPrice;
    double usdtReceived = coinsToSell * currentPricePerCoin;

    coinBalance -= coinsToSell;
    userBalance += usdtReceived;

    parts[coinIndex] = QString::number(coinBalance, 'f', 4);
    parts[3] = QString::number(userBalance, 'f', 2);

    QString updatedLine = parts.join(",");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        displayer->setText("Failed to write to users.txt");
        displayer->exec();
        return;
    }

    QTextStream out(&file);
    for (QString& line : lines) {
        if (line.startsWith(username + ",")) {
            out << updatedLine << "\n";
        }
        else {
            out << line << "\n";
        }
    }
    file.close();

    sellSeries->append(x-1, currentPrice);

    updateBalance();

    double currentprice = usdtReceived / coinsToSell;

    displayer->setText(" Sold " + QString::number(coinsToSell, 'f', 4) +
        " coins for $" + QString::number(currentprice*coinsToSell, 'f', 2));
    displayer->exec();

    saveTransaction("Sell", currentprice, coinsToSell);

}


void GraphWindow::updateUserBalance(const QString& coinType, int quantity, bool isBuy) {
    QFile file("users.txt");
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qDebug() << "Could not open users.txt file for reading/writing!";
        return;
    }
    balanceLabel->setText("Balance: $" + QString::number(userBalance, 'f', 2));
    QTextStream in(&file);
    QStringList userData;
    QString line;
    QString updatedFileContent;
    bool userFound = false;

    while (!in.atEnd()) {
        line = in.readLine();
        userData = line.split(",");

        if (userData[0] == username) { 
            userFound = true;

            if (isBuy) {
                if (coinType == "BTC") {
                    userData[4] = QString::number(userData[4].toDouble() + quantity, 'f', 4);  
                }
                else if (coinType == "ETH") {
                    userData[5] = QString::number(userData[5].toDouble() + quantity, 'f', 4);  
                }
                else if (coinType == "SOLANA") {
                    userData[6] = QString::number(userData[6].toDouble() + quantity, 'f', 4); 
                }
            }
            else {
                if (coinType == "BTC") {
                    userData[4] = QString::number(userData[4].toDouble() - quantity, 'f', 4);  
                }
                else if (coinType == "ETH") {
                    userData[5] = QString::number(userData[5].toDouble() - quantity, 'f', 4);  
                }
                else if (coinType == "SOLANA") {
                    userData[6] = QString::number(userData[6].toDouble() - quantity, 'f', 4); 
                }
            }
            if (quantity == -1) {
                userData[4] = QString::number(0, 'f', 4);  // BTC amount
                userData[5] = QString::number(0, 'f', 4);  // ETH amount
                userData[6] = QString::number(0, 'f', 4);  // Solana amount
            }
            updatedFileContent += userData.join(",") + "\n";
        }
        else {
            updatedFileContent += line + "\n";  
        }
    }

    if (userFound) {
        file.seek(0);
        QTextStream out(&file);
        out << updatedFileContent;
    }
    else {
        qDebug() << "User not found in users.txt!";
    }

    file.close();
}



bool GraphWindow::updateUserBalance(double newBalance)
{
    QFile file("users.txt");
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qDebug() << "Failed to open users file for update";
        return false;
    }

    QStringList lines;
    QTextStream in(&file);
    bool userFound = false;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        if (parts.size() >= 4 && parts[0] == username) {
            userBalance = newBalance;
            parts[3] = QString::number(newBalance, 'f', 2);
            line = parts.join(",");
            userFound = true;
        }
        lines << line;
    }

    if (userFound) {
        file.resize(0);
        QTextStream out(&file);
        for (const QString& line : lines) {
            out << line << "\n";
        }
    }

    file.close();
    return userFound;
}

void GraphWindow::saveTransaction(const QString& type, double price, double amount)
{
    QFile file("transactions_" + username + ".txt");
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Failed to open transactions file";
        return;
    }

    QTextStream out(&file);
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << ","
        << currentCrypto << ","
        << type << ","
        << QString::number(amount, 'f', 4) << ","
        << QString::number(price, 'f', 2) << "\n";

    file.close();
}

void GraphWindow::onHistoryClicked()
{
    click_audio.play();
    QString filename = "transactions_" + username + ".txt";
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

    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("Transaction History");
    dialog->resize(400, 400);

    QTextEdit* textEdit = new QTextEdit(dialog);
    textEdit->setReadOnly(true);
    textEdit->setHtml(html);

    QVBoxLayout* layout = new QVBoxLayout(dialog);
    layout->addWidget(textEdit);

    QPushButton* okButton = new QPushButton("OK");
    connect(okButton, &QPushButton::clicked, dialog, &QDialog::accept);
    layout->addWidget(okButton, 0, Qt::AlignCenter);

    dialog->setLayout(layout);
    dialog->exec();
}

void GraphWindow::onBuyLimitClicked() {
    click_audio.play();
    if (currentCrypto.isEmpty()) {
        QMessageBox::warning(this, "Error", "Select a crypto first!");
        return;
    }

    bool ok;
    double price = QInputDialog::getDouble(this, "Buy Limit", "Trigger price:", 0, 0, 1e6, 2, &ok);
    if (!ok) return;

    double amount = QInputDialog::getDouble(this, "Buy Limit", "Amount to buy:", 0, 0, 1e6, 4, &ok);
    if (!ok) return;


    double totalCost = amount * price;
    if (totalCost > userBalance) {
        displayer->setText("Not enough balance to buy the selected amount of coins at the selected price limit");
        displayer->exec();
        return;
    }
    LimitOrder order;
    order.type = "Buy";
    order.crypto = currentCrypto;
    order.targetPrice = price;
    order.amount = amount;
    order.timestamp = QDateTime::currentDateTime();
    order.status = "pending";

    pendingOrders.append(order);

    QMessageBox::information(this, "Success",
        QString("Buy limit order placed: %1 %2 @ %3 USDT").arg(amount).arg(currentCrypto).arg(price));
}

void GraphWindow::onSellLimitClicked() {
    click_audio.play();
    if (currentCrypto.isEmpty()) {
        QMessageBox::warning(this, "Error", "Select a crypto first!");
        return;
    }

    bool ok;
    double price = QInputDialog::getDouble(this, "Sell Limit", "Trigger price:", 0, 0, 1e6, 2, &ok);
    if (!ok) return;
    double amount = QInputDialog::getDouble(this, "Sell Limit", "Amount to sell:", 0, 0, 1e6, 4, &ok);
    if (!ok) return;


    
    if (!ok || amount <= 0)
        return;

    QFile file("users.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        displayer->setText("Failed to open users.txt");
        displayer->exec();
        return;
    }

    QStringList lines;
    QString userLine;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.startsWith(username + ",")) {
            userLine = line;
        }
        lines.append(line);
    }
    file.close();

    if (userLine.isEmpty()) {
        displayer->setText("User not found.");
        displayer->exec();
        return;
    }

    QStringList parts = userLine.split(",");
    if (parts.size() < 7) {
        displayer->setText("Invalid user data.");
        displayer->exec();
        return;
    }

    int coinIndex = 4; // BTC index
    if (currentCrypto.contains("eth", Qt::CaseInsensitive))
        coinIndex = 5;
    else if (currentCrypto.contains("sol", Qt::CaseInsensitive))
        coinIndex = 6;

    double coinBalance = parts[coinIndex].toDouble();
    if (coinBalance < amount) {
        displayer->setText(" You don't have enough coins to sell.");
        displayer->exec();
        return;
    }
    
    LimitOrder order;
    order.type = "Sell";
    order.crypto = currentCrypto;
    order.targetPrice = price;
    order.amount = amount;
    order.timestamp = QDateTime::currentDateTime();
    order.status = "pending";

    pendingOrders.append(order);

    QMessageBox::information(this, "Success",
        QString("Sell limit order placed: %1 %2 @ %3 USDT").arg(amount).arg(currentCrypto).arg(price));
}


void GraphWindow::checkPendingOrders() {
    for (int i = 0; i < pendingOrders.size(); i++) {
        LimitOrder& order = pendingOrders[i];
        if (order.crypto != currentCrypto) continue;

        if (order.type == "Buy" && currentPrice <= order.targetPrice) {
            double cost = order.amount * currentPrice;
            if (userBalance >= cost) {
                userBalance -= cost;
                updateUserBalance(order.crypto, order.amount, true);
                buySeries->append(x, currentPrice);
                saveTransaction("Buy (Limit)", currentPrice, order.amount);
                pendingOrders.removeAt(i);
                i--;
                QMessageBox::information(this, "Limit Order Filled",
                    QString("Bought %1 %2 @ %3 USDT").arg(order.amount).arg(order.crypto).arg(currentPrice));
            }
        }
        else if (order.type == "Sell" && currentPrice >= order.targetPrice) {
            QFile file("users.txt");
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QMessageBox::warning(this, "Error", "Cannot check coin balance!");
                continue;
            }

            QTextStream in(&file);
            QString userLine;
            while (!in.atEnd()) {
                QString line = in.readLine();
                if (line.startsWith(username + ",")) {
                    userLine = line;
                    break;
                }
            }
            file.close();

            if (userLine.isEmpty()) continue;

            QStringList parts = userLine.split(",");
            int coinIndex = 4; 
            if (order.crypto.contains("eth", Qt::CaseInsensitive)) coinIndex = 5;
            else if (order.crypto.contains("sol", Qt::CaseInsensitive)) coinIndex = 6;

            double coinBalance = parts[coinIndex].toDouble();
            if (coinBalance >= order.amount) {
                double revenue = order.amount * currentPrice;
                userBalance += revenue;
                updateUserBalance(order.crypto, order.amount, false);
                sellSeries->append(x, currentPrice);
                saveTransaction("Sell (Limit)", currentPrice, order.amount);
                pendingOrders.removeAt(i);
                i--;
                QMessageBox::information(this, "Limit Order Filled",
                    QString("Sold %1 %2 @ %3 USDT").arg(order.amount).arg(order.crypto).arg(currentPrice));
            }
        }
    }
}
void GraphWindow::savePendingOrders() {
    QFile file("pending_orders_" + username + ".txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;

    QTextStream out(&file);
    for (const LimitOrder& order : pendingOrders) {
        out << order.type << ","
            << order.crypto << ","
            << order.targetPrice << ","
            << order.amount << ","
            << order.timestamp.toString(Qt::ISODate) << ","
            << order.status << "\n";
    }
    file.close();
}


void GraphWindow::loadPendingOrders() {
    QFile file("pending_orders_" + username + ".txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QStringList parts = in.readLine().split(",");
        if (parts.size() == 6) {
            pendingOrders.append({
                parts[0],
                parts[1],
                parts[2].toDouble(),
                parts[3].toDouble(),
                QDateTime::fromString(parts[4], Qt::ISODate),
                parts[5]
                });
        }
    }
    file.close();
}

void GraphWindow::showPendingOrders() {
    QString message = "📋 Pending Orders with Status:\n\n";
    QDateTime now = QDateTime::currentDateTime();

    for (LimitOrder& order : pendingOrders) {
        // Mark expired if older than 1 hour and still pending
        if (order.status == "pending" && order.timestamp.secsTo(now) > 3600) {
            order.status = "expired";
        }

        message += QString("%1 %2 %3 @ %4 USDT [%5]\n")
            .arg(order.type)
            .arg(order.amount)
            .arg(order.crypto)
            .arg(order.targetPrice)
            .arg(order.status);
    }

    QMessageBox::information(this, "Pending Orders", message);
}


void GraphWindow::onBackClicked()
{
    if (username == "Admin") {
        AdminWindow* adminwindow = new AdminWindow();
        adminwindow->showFullScreen();
    }
    else {
        LoginPageWindow* Loginpage = new LoginPageWindow(username);
        Loginpage->showFullScreen();
    }
    Graph_audio.stop();
    click_audio.play();
    timer->stop();
    close();

}

void GraphWindow::closeEvent(QCloseEvent* event) {
    Graph_audio.stop();
    event->accept();
}