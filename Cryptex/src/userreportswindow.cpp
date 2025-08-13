#include "userreportswindow.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStyle>
#include "adminwindow.h"
#include "profitlosswindow.h"

UserReportsWindow::UserReportsWindow(QWidget* parent)
    : QWidget(parent)
{
    setWindowTitle("User Reports");
    resize(600, 400); 

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QLabel* title = new QLabel("Select a user to view report", this);
    title->setStyleSheet("font-size: 20px; font-weight: bold; padding: 10px;");
    mainLayout->addWidget(title);

    userList = new QListWidget(this);
    userList->setSelectionMode(QAbstractItemView::SingleSelection);
    userList->setStyleSheet("padding: 5px; font-size: 14px;");
    mainLayout->addWidget(userList);

    userList->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QPushButton* okButton = new QPushButton("OK", this);
    QPushButton* backButton = new QPushButton("Back", this);

    okButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #90ee90;"
        "   color: white;"
        "   border-radius: 6px;"
        "   padding: 8px 24px;"      // Enough space for text
        "   font-weight: bold;"
        "   border: none;"
        "   min-width: 100px;"       // Optional: avoids squashing
        "}"

        "QPushButton:hover {"
        "   border: 2px solid #a0a0a0;"
        "}");
    backButton->setStyleSheet("QPushButton {"
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
        "}");

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(okButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(backButton);
    mainLayout->addLayout(buttonLayout);

    connect(backButton, &QPushButton::clicked, this, &UserReportsWindow::OnbackClicked);
    connect(okButton, &QPushButton::clicked, this, &UserReportsWindow::onOkClicked);

    loadUsers();
}

void UserReportsWindow::loadUsers() {
    QFile file("users.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Could not open users.txt");
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        if (!parts.isEmpty()) {
            userList->addItem(parts[0]);
        }
    }
    file.close();
}

void UserReportsWindow::onOkClicked() {
    click_audio.play();
    QListWidgetItem* item = userList->currentItem();
    if (!item) {
        QMessageBox::information(this, "No Selection", "Please select a user from the list.");
        return;
    }

    QString username = item->text();

    ProfitLossWindow* profitWindow = new ProfitLossWindow(username);
    profitWindow->setWindowTitle("Transaction Report - " + username);
    profitWindow->setWindowFlags(Qt::Window); 
    profitWindow->resize(600, 400);
    profitWindow->show();
}


void UserReportsWindow::OnbackClicked() {
    click_audio.play();
    this->close();
}
