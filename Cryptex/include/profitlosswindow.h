#pragma once

#include <QDialog>

class QTextEdit;

class ProfitLossWindow : public QDialog {
    Q_OBJECT
public:
    explicit ProfitLossWindow(const QString& username, QWidget* parent = nullptr);
    int no_of_profits = 0;
    int no_of_losses = 0;
    void generateReport(const QString& username);
private:
    QTextEdit* textEdit;
    QString timestamp;
    QString crypto;
    QString type;
    double quantity;
    double price;
};
