#ifndef CONTROLLER_MODELVIEWGLUE_THHR_20131121
#define CONTROLLER_MODELVIEWGLUE_THHR_20131121

#include "Model/Area.h"
#include "Model/Color.h"
#include "Model/Piece.h"
#include "Model/Rotation.h"

#include "View/Typedefs.h"

#include <QColor>

namespace Controller
{
	unsigned
	colFromX( int inX, unsigned inStartCol );

	unsigned
	rowFromY( int inY, unsigned inStartRow );

	int
	xFromCol( unsigned inCol, unsigned inStartCol );

	int
	yFromRow( unsigned inRow, unsigned inStartRow );

	int
	xFromArea( Model::Area::Area inArea );

	int
	yFromArea( Model::Area::Area inArea );

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
	modelFromView( View::Piece inPiece );

	Model::Rotation
	modelFromView( View::Rotation inRotation );

	View::Rotation
	viewFromModel( Model::Rotation inRotation );
}

#endif