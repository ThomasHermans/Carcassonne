#include "GameController.h"
#include "UnitTests.h"

#include <QtGui/QApplication>

#include <iostream>

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
