#include "ProfitLossWindow.h"
#include <QTextEdit>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QMap>
#include <QQueue>
#include <QMessageBox>

ProfitLossWindow::ProfitLossWindow(const QString& username, QWidget* parent)
    : QDialog(parent) {
    setWindowTitle("📊 Profit/Loss Report");
    resize(900, 650);

    textEdit = new QTextEdit(this);
    textEdit->setReadOnly(true);
    textEdit->setStyleSheet(R"(
        QTextEdit {
            background-color: #121212;
            color: #eeeeee;
            font-family: Consolas, monospace;
            font-size: 15px;
            padding: 16px;
            border-radius: 10px;
        }
    )");

    
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(textEdit);

    generateReport(username);

}

void ProfitLossWindow::generateReport(const QString& username) {
    QFile file("transactions_" + username + ".txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        textEdit->setText("<span style='color:red;'>❌ Error: User has not made any Transactions yet </span>");
        return;
    }

    QTextStream in(&file);
    QMap<QString, QQueue<QPair<double, double>>> buyQueues;  
    QString html = "<p style='text-align:center; color:#00ffff; font-size:24px;'>📈 Profit / Loss Report</p>";

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) {
            continue;
        }

        QStringList parts = line.split(',');
        if (parts.size() != 5) {
            continue;
        }

        QString timestamp = parts[0].trimmed();
        QString crypto = parts[1].trimmed();
        QString type = parts[2].trimmed().toUpper();
        double quantity = parts[3].toDouble();
        double price = parts[4].toDouble();

        if (type == "BUY") {
            buyQueues[crypto].enqueue(qMakePair(price, quantity));
        }
        else if (type == "SELL") {
            double remainingQty = quantity;

            while (remainingQty > 0 && !buyQueues[crypto].isEmpty()) {
                QPair<double, double> buyInfo = buyQueues[crypto].dequeue();
                double buyPrice = buyInfo.first;
                double buyQty = buyInfo.second;
                double matchedQty = qMin(remainingQty, buyQty);

                double buyCost = matchedQty * buyPrice;
                double sellRevenue = matchedQty * price;
                double profit = sellRevenue - buyCost;
                double percent = (profit / buyCost) * 100.0;

                QString color = profit >= 0 ? "#00ff66" : "#ff5555";
                if (profit > 0) {
                    no_of_profits++;
                }
                else if (profit < 0) {
                    no_of_losses++;
                }
                QString emoji = profit >= 0 ? "✅" : "❌";
                QString sign = profit >= 0 ? "+" : "";

                html += QString(
                    "<p style='color:%1;'>"
                    "[%2] Buy @ %3 → Sell @ %4 → %5%6 USDT (%7%8%) %9"
                    "</p>")
                    .arg(color)
                    .arg(crypto)
                    .arg(QString::number(buyPrice, 'f', 2))
                    .arg(QString::number(price, 'f', 2))
                    .arg(sign)
                    .arg(QString::number(profit, 'f', 2))
                    .arg(sign)
                    .arg(QString::number(percent, 'f', 2))
                    .arg(emoji);

                remainingQty -= matchedQty;

                if (buyQty > matchedQty) {
                    buyQueues[crypto].prepend(qMakePair(buyPrice, buyQty - matchedQty));
                }
            }
        }
    }

    if (html.trimmed().isEmpty()) {
        html = "<p style='color:gray;'>No matched buy/sell trades to report.</p>";
    }

    textEdit->setHtml(html);
}
