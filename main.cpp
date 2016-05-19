#include "Controller/GameController.h"
#include "Controller/GUIPlayer.h"
#include "Controller/Moderator.h"
#include "Controller/RobotPlayer.h"
#include "Controller/SupremeController.h"

#include "Utils/Location.h"
#include "Utils/PrintUtils.h"

#include "UnitTests.h"

#include <QApplication>
#include <QFile>
#include <QMetaType>
#include <QString>

#include <iostream>

Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(Model::TileOnBoard)
Q_DECLARE_METATYPE(Utils::Location)

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
	Main::Type const gameOption = Main::kPlay;

	qRegisterMetaType< std::string >();
	qRegisterMetaType< Model::TileOnBoard >();
	qRegisterMetaType< Utils::Location >();

	// QFile file( ":/StyleSheet.qss" );
	// file.open( QFile::ReadOnly );
	// QString styleSheet = QLatin1String( file.readAll() );

	if ( gameOption == Main::kPlay )
	{
		QApplication a( argc, argv );
		// a.setStyleSheet( styleSheet );
		
		Controller::SupremeController controller;

		// TODO: once the game is finished: display scores and announce winners
		return a.exec();
	}
	else if ( gameOption == Main::kUnitTests )
	{
		tom::mainTests();
		return EXIT_SUCCESS;
	}
	else if ( gameOption == Main::kDebugSpecificGame )
	{
		QApplication a( argc, argv );
		// a.setStyleSheet( styleSheet );
		
		// GameController gc( "BKDAD" );
		Controller::GameController gc( "eFBD" );

		return a.exec();
	}
}
