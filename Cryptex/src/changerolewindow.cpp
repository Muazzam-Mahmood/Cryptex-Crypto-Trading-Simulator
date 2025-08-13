#include "changerolewindow.h"
#include <QFile>
#include <QTextStream>
#include <QVBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>
#include <QLabel>
#include <QFont>
#include <QFrame>
#include <adminwindow.h>

ChangeRoleWindow::ChangeRoleWindow(QWidget* parent)
    : QWidget(parent)
{
    setWindowTitle("Promote User to Admin");
    setFixedSize(450, 250);  

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QLabel* title = new QLabel(" Promote a User to Admin", this);
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

    promoteButton = new QPushButton("Make Admin", this);
    promoteButton->setMinimumHeight(40);
    promoteButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #27ae60;"   
        "   color: white;"
        "   border-radius: 5px;"
        "   padding: 8px 15px;"
        "   font-weight: bold;"
        "   border: none;"
        "}"
        "QPushButton:hover {"
        "   border: 2px solid #a0a0a0;"
        "}"
    );
    backButton = new QPushButton("Back", this);
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

    mainLayout->addWidget(promoteButton);
    mainLayout->addWidget(backButton);
    mainLayout->addStretch();

    connect(promoteButton, &QPushButton::clicked, this, &ChangeRoleWindow::onPromoteClicked);
    connect(backButton, &QPushButton::clicked, this, &ChangeRoleWindow::onBackClicked);
    loadUsers();
}

ChangeRoleWindow::~ChangeRoleWindow() {}

void ChangeRoleWindow::loadUsers() {
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

void ChangeRoleWindow::onPromoteClicked() {
    click_audio.play();
    int index = userDropdown->currentIndex();
    QString fullLine = userDropdown->itemData(index).toString();
    QStringList parts = fullLine.split(",");
    if (parts.size() < 2) {
        QMessageBox::warning(this, "Error", "Invalid user format.");
        return;
    }
    QString selectedLine = parts[0] + "," + parts[1];  

    if (selectedLine.isEmpty()) {
        QMessageBox::warning(this, "Error", "No user selected.");
        return;
    }

    int confirm = QMessageBox::question(this, "Confirm Promotion",
        "Are you sure you want to make this user an admin?\n" + userDropdown->currentText(),
        QMessageBox::Yes | QMessageBox::No);

    if (confirm != QMessageBox::Yes) return;

    QFile adminFile("admin.txt");
    bool alreadyAdmin = false;


    if (adminFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&adminFile);
        while (!in.atEnd()) {
            if (in.readLine().trimmed() == selectedLine) {
                alreadyAdmin = true;
                break;
            }
        }
        adminFile.close();
    }

    if (alreadyAdmin) {
        QMessageBox::information(this, "Already Admin", "This user is already an admin.");
        return;
    }


    if (adminFile.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&adminFile);
        out << selectedLine << "\n";
        adminFile.close();
        QMessageBox::information(this, "Success", "User promoted to admin successfully.");
    }
    else {
        QMessageBox::warning(this, "Error", "Failed to open admin.txt");
    }
}

void ChangeRoleWindow::onBackClicked() {

    this->close();
}