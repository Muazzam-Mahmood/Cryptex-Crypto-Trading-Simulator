#pragma once
#include <QWidget>
#include <QPushButton>
#include<QtMultimedia>
#include"audio.h"
#include <QMainWindow>
#include "ui_frontpage.h"  // This gets auto-generated from frontpage.ui

class FrontPageWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FrontPageWindow(QWidget* parent = nullptr);
    ClickSound click_audio;
    IntroSound intro_audio;
    QPushButton* adminButton;
    QPushButton* signupButton;
    QPushButton* loginButton;
    QPushButton* demobutton;
    void onAdminButtonClicked();
    void onSignupButtonClicked();
    void onLoginButtonClicked();
    void onDemoButtonClicked();
private slots:

private:
    void setupUI();
    Ui::MainWindow* ui;

};


