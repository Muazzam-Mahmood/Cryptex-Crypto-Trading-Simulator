#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include"audio.h"
#include"LoginPageWindow.h"
#include"ui_LoginPass.h"
class LoginPass : public QMainWindow {
    Q_OBJECT

public:
    LoginPass(QWidget* parent = nullptr);
    ClickSound click_audio;
private slots:
    void onLoginButtonClicked();
    void onBackButtonClicked();

private:
    QPushButton* loginButton;
    QPushButton* backButton;
    QLineEdit* usernameInput;
    QLineEdit* passwordInput;
    QLabel* usernameLabel;
    QLabel* passwordLabel;

    Ui::LoginPass* ui3;
};
