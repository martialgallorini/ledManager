/*
Project initiated by Martial GALLORINI
This software is free to modify and redistribute.
Please enclose source code when redistributing.
Feel free to contribute.
*/

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
