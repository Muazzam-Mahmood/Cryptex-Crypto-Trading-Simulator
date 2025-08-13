#pragma once

#include <QMainWindow>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QLabel>
#include <QMessageBox>
#include <QInputDialog>
#include <QDateTime>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>
#include"audio.h"


class GraphWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit GraphWindow(const QString& username, QWidget* parent = nullptr);
    ~GraphWindow();
    QMessageBox* displayer = new QMessageBox(this); 
    void updateUserBalance(const QString& coinType, int quantity, bool isBuy);
    GraphSound Graph_audio;
    ClickSound click_audio;
    void closeEvent(QCloseEvent* event) override;
private slots:
    void loadNextPoint();
    void onBackClicked();
    void onBuyClicked();
    void onSellClicked(); 
    void onHistoryClicked();
    void onBuyLimitClicked();   
    void onSellLimitClicked();   
    void checkPendingOrders();    
    void showPendingOrders();

private:
    void setupUI();
    void loadUserBalance();
    void startGraphing(const QString& fileName);
    double findMinY() const;
    double findMaxY() const;
    void updateAxes();
    void updateBalance();
    void saveTransaction(const QString& type, double price, double amount);
    bool updateUserBalance(double newBalance);
    void savePendingOrders(); 
    void loadPendingOrders();
    struct LimitOrder {
        QString type;       
        QString crypto;     
        double targetPrice=0; 
        double amount=0;       
        QDateTime timestamp;
        QString status;    
        
    };

    QList<LimitOrder> pendingOrders;
    QString username;
    QString CurrentUsername;
    QString currentCrypto;
    QTimer* timer;
    QFile* priceFile;
    QTextStream* priceStream;
    QLineSeries* priceSeries;
    QScatterSeries* buySeries;
    QScatterSeries* sellSeries;
    QChart* chart;
    QChartView* chartView;
    QLabel* balanceLabel;
    QLabel* positionLabel;
    QValueAxis* axisX;
    QValueAxis* axisY;

    double currentPrice;
    double buyPrice;
    double positionAmount;
    double userBalance;
    bool hasPosition;

    int x;
    const int maxPoints = 100;
};


