#include "Controller/GameController.h"
#include "Controller/SupremeController.h"
#include "UnitTests.h"

#include <QApplication>
#include <QFile>
#include <QString>

#include <iostream>

namespace Main
{
	enum Type
	{
		kPlay,
		kUnitTests,
		kDebugSpecificGame
	};
}

int main(int argc, char * argv[])
{
	Main::Type const gameOption = Main::kUnitTests;

	if ( gameOption == Main::kPlay )
	{
		QFile file( ":/StyleSheet.qss" );
		file.open( QFile::ReadOnly );
		QString styleSheet = QLatin1String( file.readAll() );

		QApplication a(argc, argv);
		a.setStyleSheet( styleSheet );
		
		Controller::SupremeController controller;

		return a.exec();
	}
	else if ( gameOption == Main::kUnitTests )
	{
		tom::mainTests();
		return EXIT_SUCCESS;
	}
	else if ( gameOption == Main::kDebugSpecificGame )
	{
		QFile file( ":/StyleSheet.qss" );
		file.open( QFile::ReadOnly );
		QString styleSheet = QLatin1String( file.readAll() );

		QApplication a(argc, argv);
		a.setStyleSheet( styleSheet );
		
		// GameController gc( "BKDAD" );
		Controller::GameController gc( "EEED" );

		return a.exec();
	}
}
