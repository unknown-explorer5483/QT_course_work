#pragma once
#include <QApplication>
#include "dialog.h"
int main(int argc, char* argv[])
{
    
    QApplication app(argc, argv);
    mainwin* mainwindow = new mainwin();    
    mainwindow->setWindowTitle("Главное окно");    
    mainwindow->show();
    

    return app.exec();
}
