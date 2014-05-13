#include "LocationUtils.h"

#include "View/Typedefs.h"

int
View::getX( Utils::Location const & inLocation )
{
	return inLocation.second * Gui::kTileWidth;
}

int
View::getY( Utils::Location const & inLocation )
{
	return inLocation.first * Gui::kTileHeight;
}