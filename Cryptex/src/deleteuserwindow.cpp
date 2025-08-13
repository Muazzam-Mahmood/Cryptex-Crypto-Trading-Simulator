#include "deleteuserwindow.h"
#include <QFile>
#include <QTextStream>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFont>
#include <QFrame>
#include <QSpacerItem>

DeleteUserWindow::DeleteUserWindow(QWidget* parent)
    : QWidget(parent)
{
    setWindowTitle("Delete User");
    setFixedSize(450, 250);  

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QLabel* title = new QLabel(" Delete a User", this);
    QFont titleFont = title->font();

    titleFont.setPointSize(18);
    titleFont.setBold(true);
    
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
    
    mainLayout->addWidget(title);

    QFrame* line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(line);

    QLabel* label = new QLabel("Select a user:", this);
    QFont labelFont = label->font();
    labelFont.setPointSize(12);
    labelFont.setBold(true);
    label->setFont(labelFont);
    mainLayout->addWidget(label);

    userDropdown = new QComboBox(this);
    userDropdown->setMinimumHeight(35);
    userDropdown->setStyleSheet("font-size: 14px; padding: 5px;");
    mainLayout->addWidget(userDropdown);

    deleteButton = new QPushButton("Delete User", this);
    deleteButton->setMinimumHeight(40);
    deleteButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #e74c3c;"  
        "   color: white;"
        "   border-radius: 5px;"      
        "   padding: 8px 15px;"
        "   font-weight: bold;"
        "   border: none;"
        "}"
        "QPushButton:hover {"         
        " border: 2px solid #a0a0a0;"  
        "}"
    );

    mainLayout->addWidget(deleteButton);
    mainLayout->addStretch();
    connect(deleteButton, &QPushButton::clicked, this, &DeleteUserWindow::onDeleteUserClicked);
    loadUsers();
}

DeleteUserWindow::~DeleteUserWindow() {}

void DeleteUserWindow::loadUsers() {
    QFile file("users.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Unable to open users.txt");
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        QStringList parts = line.split(",");
        if (parts.size() >= 3) {
            QString displayText = parts[0] + " | " + parts[2]; 
            userDropdown->addItem(displayText, line);         
        }
    }
    file.close();
}


void DeleteUserWindow::onDeleteUserClicked() {
    int index = userDropdown->currentIndex();
    QString selectedLine = userDropdown->itemData(index).toString();

    if (selectedLine.isEmpty()) {
        QMessageBox::warning(this, "Error", "No user selected.");
        return;
    }

    int confirm = QMessageBox::question(this, "Confirm Delete",
        "Are you sure you want to delete this user?\n" + selectedLine,
        QMessageBox::Yes | QMessageBox::No);

    if (confirm == QMessageBox::Yes) {
        deleteUserFromFile(selectedLine);
        QMessageBox::information(this, "Deleted", "User deleted successfully.");
        userDropdown->clear();
        loadUsers();
    }
}


void DeleteUserWindow::deleteUserFromFile(const QString& fullLine) {
    QString username = fullLine.section(',', 0, 0).trimmed();
    QFile userFile("users.txt");
    QStringList userLines;

    if (userFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&userFile);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (line != fullLine) {
                userLines.append(line);
            }
        }
        userFile.close();

        if (userFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
            QTextStream out(&userFile);
            for (const QString& line : userLines)
                out << line << "\n";
            userFile.close();
        }
    }


    QFile adminFile("admin.txt");
    QStringList adminLines;

    if (adminFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&adminFile);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            QString adminUsername = line.section(',', 0, 0).trimmed();
            if (adminUsername != username) {
                adminLines.append(line);
            }
        }
        adminFile.close();

        if (adminFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
            QTextStream out(&adminFile);
            for (const QString& line : adminLines)
                out << line << "\n";
            adminFile.close();
        }
    }
}

