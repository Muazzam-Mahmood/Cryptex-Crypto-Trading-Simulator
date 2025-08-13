#include <QApplication>
#include<iostream>
#include"frontpagewindow.h"
using namespace std;

int main(int argc, char* argv[])
{   
    cout << "Pogram runnin !!!!!!!!!!!!!!!!!";
    QApplication app(argc, argv);


    FrontPageWindow window1;
    window1.showFullScreen();

    return app.exec();
}
