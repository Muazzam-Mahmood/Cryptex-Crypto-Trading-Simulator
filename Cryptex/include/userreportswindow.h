#pragma once

#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QListWidgetItem>
#include <QPushButton>
#include <QLabel>
#include"audio.h"    
class UserReportsWindow : public QWidget {
    Q_OBJECT

public:
    explicit UserReportsWindow(QWidget* parent = nullptr);
    void OnbackClicked();
    ClickSound click_audio;
private slots:
    void onOkClicked();

private:
    void loadUsers();

    QListWidget* userList;
    QPushButton* okButton;
    QPushButton* backButton;
    QLabel* titleLabel;
};

 
