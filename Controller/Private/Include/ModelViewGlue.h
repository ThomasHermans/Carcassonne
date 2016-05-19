#ifndef CONTROLLER_MODELVIEWGLUE_THHR_20131121
#define CONTROLLER_MODELVIEWGLUE_THHR_20131121

#include "Model/Area.h"
#include "Model/Color.h"
#include "Model/Piece.h"
#include "Model/Rotation.h"

#include "View/Meeple.h"
#include "View/Typedefs.h"

#include "Utils/Location.h"

namespace Controller
{
	int
	colFromX( int inX );

	int
	rowFromY( int inY );

	int
	xFromCol( int inCol );

	int
	yFromRow( int inRow );

	int
	xFromArea( Model::Area::Area inArea );

	int
	yFromArea( Model::Area::Area inArea );

	int
	xFromLocation( Utils::Location const & inLocation, Model::Area::Area inArea );

	int
	yFromLocation( Utils::Location const & inLocation, Model::Area::Area inArea );

	Utils::Location
	locationFromXY( int inX, int inY );

	int
	posXFromX( int inX );

	int
	posYFromY( int inY );

	Model::Area::Area
	areaFromPos( int inX, int inY );

	Model::Color::Color
	modelFromView( View::Color inColor );

	View::Color
	viewFromModel( Model::Color::Color inColor );

	Model::Piece::PieceType
	modelFromView( View::Meeple::MeepleType inType );

	View::Meeple::MeepleType
	viewFromModel( Model::Piece::PieceType inType );

	Model::Piece
	modelFromView( View::Meeple const & inPiece );

	View::Meeple
	viewFromModel( Model::Piece const & inPiece );

	Model::Rotation
	modelFromView( View::Rotation inRotation );

	View::Rotation
	viewFromModel( Model::Rotation inRotation );
}

#endif