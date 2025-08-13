#pragma once

#include <QWidget>
#include"audio.h"
#include<QPushButton>
#include<QLabel>
#include<QComboBox>

class DeleteUserWindow : public QWidget {
    Q_OBJECT

public:
    explicit DeleteUserWindow(QWidget* parent = nullptr);
    ~DeleteUserWindow();
    ClickSound click_audio;

private slots:
    void onDeleteUserClicked();

private:
    QComboBox* userDropdown;
    QPushButton* deleteButton;
    QPushButton* backButton;
    QLabel* infoLabel;

    void loadUsers();
    void deleteUserFromFile(const QString& username);
};
