#pragma once

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include"audio.h"
class EditUserWindow : public QWidget
{
    Q_OBJECT

public:
    explicit EditUserWindow(QWidget* parent = nullptr);
    void loadUserData();
    ClickSound slick_audio;
private slots:
    void handleCellDoubleClick(int row, int column);
    void saveUserData();
    void closeWindow();

private:
    void setupUI();
    bool saveToFile();

    QTableWidget* userTable;
    QPushButton* backButton;
    QPushButton* saveButton;
    QString dataFilePath = "users.txt";
};

