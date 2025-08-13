#pragma once

#include"ui_SignUp.h"

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include"audio.h"  

class SignupWindow : public QMainWindow {
    Q_OBJECT

public:
    SignupWindow(QWidget* parent = nullptr);
    ClickSound click_audio;
private slots:
    void onSignupButtonClicked();
    void onBackButtonClicked();

private:
    QPushButton* signupButton;
    QPushButton* backButton;
    QLineEdit* usernameInput;
    QLineEdit* passwordInput;
    QLineEdit* emailInput;
    QLabel* usernameLabel;
    QLabel* passwordLabel;
    QLabel* emailLabel;
    Ui::SignUpPass* ui3;
    
};


