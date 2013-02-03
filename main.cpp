#include <QtGui/QApplication>
#include "mainwindow.h"

#include <iostream>

#include "GameController.h"
#include "UnitTests.h"

int main(int argc, char * argv[])
{
    bool testing = false;
    if (testing)
    {
        tom::mainTests();
        return EXIT_SUCCESS;
    }
    else
    {
        QApplication a(argc, argv);
        GameController gc;

        return a.exec();
    }
}
