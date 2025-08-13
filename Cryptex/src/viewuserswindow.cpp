#include "viewuserswindow.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QTableWidgetItem>
#include <QScrollArea>
#include <QPushButton>
#include <QHeaderView>

ViewUsersWindow::ViewUsersWindow(QWidget* parent)
    : QWidget(parent)
{
    setWindowTitle("View Users");

    QVBoxLayout* layout = new QVBoxLayout(this);
    
    layout->setContentsMargins(10, 10, 10, 10); 

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    usersTable = new QTableWidget(this);
    usersTable->setColumnCount(6);
    usersTable->setHorizontalHeaderLabels({ "Username", "Email","Balance $","BTC","ETH","SOL" });

    QFont headerFont = usersTable->horizontalHeader()->font();
    headerFont.setBold(true);

    usersTable->horizontalHeader()->setFont(headerFont);
    usersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    usersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    usersTable->setAlternatingRowColors(true);

    usersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // Stretch columns
    usersTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); // Auto-adjust row height
    usersTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    scrollArea->setWidget(usersTable);

    layout->addWidget(scrollArea, 1);  // Takes maximum space

    backButton = new QPushButton("Back", this);
    backButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #2980b9;"
        "   color: white;"
        "   border-radius: 6px;"
        "   padding: 8px 24px;"      // Enough space for text
        "   font-weight: bold;"
        "   border: none;"
        "   min-width: 100px;"       // Optional: avoids squashing
        "}"

        "QPushButton:hover {"
        "   border: 2px solid #a0a0a0;"
        "}"
    );
    
    connect(backButton, &QPushButton::clicked, this, &ViewUsersWindow::onBackButtonClicked);

    layout->addWidget(backButton, 0, Qt::AlignRight); 
    loadUsers();
}

ViewUsersWindow::~ViewUsersWindow() {}

void ViewUsersWindow::loadUsers() {
    
    QFile file("users.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Unable to open users.txt");
        return;
    }

    QTextStream in(&file);
    int userIndex = 1;
    while (!in.atEnd()) {

        QString line = in.readLine().trimmed();
        QStringList parts = line.split(",");

        if (parts.size() >= 3) {
            QString username = parts[0];
            QString email = parts[2];
            QString Amount = parts[3];
            QString BTC = parts[4];
            QString ETH = parts[5];
            QString SOL = parts[6];
            int row = usersTable->rowCount();
            usersTable->insertRow(row);

            usersTable->setItem(row, 0, new QTableWidgetItem(username));
            usersTable->setItem(row, 1, new QTableWidgetItem(email));
            usersTable->setItem(row, 2, new QTableWidgetItem(Amount));
            usersTable->setItem(row, 3, new QTableWidgetItem(BTC));
            usersTable->setItem(row, 4, new QTableWidgetItem(ETH));
            usersTable->setItem(row, 5, new QTableWidgetItem(SOL));

            userIndex++;
        }
    }
    file.close();
}

void ViewUsersWindow::onBackButtonClicked() {
    this->close();
}