#include "GameController.h"
#include "SupremeController.h"
#include "UnitTests.h"

#include <QApplication>
#include <QFile>
#include <QString>

#include <iostream>

namespace
{
	int const kPlay = 0;
	int const kUnitTests = 1;
	int const kDebugSpecificGame = 2;
}

int main(int argc, char * argv[])
{
	int const gameOption = kPlay;

	if ( gameOption == kPlay )
	{
		QFile file( ":/StyleSheet.qss" );
		file.open( QFile::ReadOnly );
		QString styleSheet = QLatin1String( file.readAll() );

		QApplication a(argc, argv);
		a.setStyleSheet( styleSheet );
		
		SupremeController controller;

		return a.exec();
	}
	else if ( gameOption == kUnitTests )
	{
		tom::mainTests();
		return EXIT_SUCCESS;
	}
	else if ( gameOption == kDebugSpecificGame )
	{
		QFile file( ":/StyleSheet.qss" );
		file.open( QFile::ReadOnly );
		QString styleSheet = QLatin1String( file.readAll() );

		QApplication a(argc, argv);
		a.setStyleSheet( styleSheet );
		
		GameController gc( "QVVUKXDPJD" );

		return a.exec();
	}
}
