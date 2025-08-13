#include "edituserwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QScrollArea>
#include <QFont>

EditUserWindow::EditUserWindow(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
    loadUserData();
}

void EditUserWindow::setupUI()
{
    setWindowTitle("Edit Users");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    userTable = new QTableWidget(this);
    userTable->setColumnCount(8);
    userTable->setHorizontalHeaderLabels({ "ID", "Username", "Password", "Email","Balance $","BTC","ETH","SOL" });

    QFont headerFont = userTable->horizontalHeader()->font();
    headerFont.setBold(true);
    userTable->horizontalHeader()->setFont(headerFont);

    userTable->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
    userTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    userTable->setAlternatingRowColors(true);
    userTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    userTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    scrollArea->setWidget(userTable);
    mainLayout->addWidget(scrollArea, 1);

    QHBoxLayout* buttonLayout = new QHBoxLayout();

    backButton = new QPushButton("Back", this);
    saveButton = new QPushButton("Save Changes", this);

    QString buttonStyle =
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
        "}";

    backButton->setStyleSheet(buttonStyle);

    saveButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #4CAF50;"
        "   color: white;"
        "   border-radius: 6px;"
        "   padding: 8px 24px;"      // Enough space for text
        "   font-weight: bold;"
        "   border: none;"
        "   min-width: 100px;"
        "   min-height: 20px"// Optional: avoids squashing
        "}"
        "QPushButton:hover {"
        "   border: 2px solid #a0a0a0;"
        "}"
    );

    buttonLayout->addWidget(backButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(saveButton);
    mainLayout->addLayout(buttonLayout);

    connect(backButton, &QPushButton::clicked, this, &EditUserWindow::closeWindow);
    connect(saveButton, &QPushButton::clicked, this, &EditUserWindow::saveUserData);
    connect(userTable, &QTableWidget::cellDoubleClicked, this, &EditUserWindow::handleCellDoubleClick);
}

void EditUserWindow::loadUserData()
{
    QFile file("users.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "The User Data file could not be opened");
        return;
    }

    userTable->setRowCount(0);
    QTextStream in(&file);
    int row = 0;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split(",");
        if (parts.size() >= 7) {
            userTable->insertRow(row);
            userTable->setItem(row, 0, new QTableWidgetItem(QString::number(row + 1)));
            userTable->setItem(row, 1, new QTableWidgetItem(parts[0].trimmed()));
            userTable->setItem(row, 2, new QTableWidgetItem(parts[1].trimmed()));
            userTable->setItem(row, 3, new QTableWidgetItem(parts[2].trimmed()));
            userTable->setItem(row, 4, new QTableWidgetItem(parts[3].trimmed()));
            userTable->setItem(row, 5, new QTableWidgetItem(parts[4].trimmed()));
            userTable->setItem(row, 6, new QTableWidgetItem(parts[5].trimmed()));
            userTable->setItem(row, 7, new QTableWidgetItem(parts[6].trimmed()));
            row++;
        }
        userTable->setColumnHidden(0, true);
        userTable->setColumnHidden(4, true);
        userTable->setColumnHidden(5, true);
        userTable->setColumnHidden(6, true);
        userTable->setColumnHidden(7, true);

    }
    file.close();
}

void EditUserWindow::handleCellDoubleClick(int row, int column)
{
    if (column == 0) return;
    userTable->editItem(userTable->item(row, column));
}

void EditUserWindow::saveUserData()
{
    if (saveToFile()) {
        QMessageBox::information(this, "Success", "Changes saved successfully");
    }
    else {
        QMessageBox::warning(this, "Error", "Failed to save changes");
    }
}

bool EditUserWindow::saveToFile()
{
    QFile file(dataFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);
    for (int row = 0; row < userTable->rowCount(); ++row) {
        QString line = QString("%1,%2,%3,%4,%5,%6,%7")
            .arg(userTable->item(row, 1)->text())
            .arg(userTable->item(row, 2)->text())
            .arg(userTable->item(row, 3)->text())
            .arg(userTable->item(row, 4)->text())
            .arg(userTable->item(row, 5)->text())
            .arg(userTable->item(row, 6)->text())
            .arg(userTable->item(row, 7)->text());
        out << line << "\n";
    }
    file.close();
    return true;
}

void EditUserWindow::closeWindow()
{
    this->close();
}