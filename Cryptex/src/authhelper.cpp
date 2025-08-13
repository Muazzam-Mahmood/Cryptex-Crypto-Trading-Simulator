#include "authhelper.h"
#include <QFile>
#include <QTextStream>
#include<iostream>
using namespace std;

bool isAdminValid(const QString& id, const QString& pass) {
    QFile file("admin.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        QStringList parts = line.split(",");
        if (parts.size() == 2) {
            if (parts[0].trimmed() == id && parts[1].trimmed() == pass) {
                return true;
            }
        }
    }
    return false;
}
