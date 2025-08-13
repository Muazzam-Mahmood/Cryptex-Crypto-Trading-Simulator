#pragma once

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "audio.h"
#include"ui_AdminPass.h"

class AdminLoginWindow : public QMainWindow {
    Q_OBJECT

public:
    AdminLoginWindow(QWidget* parent = nullptr);

private slots:
    void onLoginButtonClicked();
    void onBackButtonClicked();

private:
    QLabel* adminIdLabel;
    QLabel* adminPasswordLabel;
    QLineEdit* adminIdInput;
    QLineEdit* adminPasswordInput;

    QPushButton* loginButton;
    QPushButton* backButton;

    ClickSound click_audio;

    Ui::AdminPass* ui4;
};

