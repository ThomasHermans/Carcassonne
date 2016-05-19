#include "LocationUtils.h"

#include "View/Typedefs.h"

int
View::getX( Utils::Location const & inLocation )
{
	return inLocation.col * Gui::kTileWidth;
}

int
View::getY( Utils::Location const & inLocation )
{
	return inLocation.row * Gui::kTileHeight;
}