#pragma once

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include"audio.h"
class ChangeRoleWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ChangeRoleWindow(QWidget* parent = nullptr);
    ~ChangeRoleWindow();
    ClickSound  click_audio;
private slots:
    void onPromoteClicked();
    void onBackClicked();
private:
    void loadUsers();
    QComboBox* userDropdown;
    QPushButton* promoteButton;
    QPushButton* backButton;
};

