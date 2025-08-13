#pragma once

#include <QWidget>
#include <QPushButton>
#include <QTableWidget>
#include <QScrollArea>

class ViewUsersWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ViewUsersWindow(QWidget* parent = nullptr);
    ~ViewUsersWindow();

private slots:
    void onBackButtonClicked();

private:
    void loadUsers();

    QPushButton* backButton;
    QTableWidget* usersTable;
    QScrollArea* scrollArea;
};

