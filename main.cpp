#include "GameController.h"
#include "UnitTests.h"
#include "src-view/StartScreen.h"

#include <QApplication>
#include <QFile>
#include <QString>

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
		QFile file( ":/StyleSheet.qss" );
		file.open( QFile::ReadOnly );
		QString styleSheet = QLatin1String( file.readAll() );

		QApplication a(argc, argv);
		a.setStyleSheet( styleSheet );
		
		// GameController gc( "QVVUKXDPJD" );
		StartScreen s;
		s.show();

		return a.exec();
	}
}
