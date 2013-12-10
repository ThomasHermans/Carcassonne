#include "Settings.h"

QSettings &
Controller::getSettings()
{
	static QSettings sSettings( "TeamDinoRaptor", "Carcasonne" );

	return sSettings;
}