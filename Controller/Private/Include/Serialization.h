#ifndef CARCASSONNE_CONTROLLER_SERIALIZATION
#define CARCASSONNE_CONTROLLER_SERIALIZATION

#include "Model/Area.h"
#include "Model/Color.h"
#include "Model/Piece.h"
#include "Model/PlacedPiece.h"
#include "Model/Player.h"
#include "Model/Rotation.h"
#include "Model/Tile.h"
#include "Model/TileOnBoard.h"

#include <QDataStream>

namespace Controller
{
	QDataStream & operator <<
	( QDataStream & inStream, Model::Area::Area inArea );
	
	QDataStream & operator >>
	( QDataStream & inStream, Model::Area::Area & outArea );

	QDataStream & operator <<
	( QDataStream & inStream, Model::Color::Color inColor );

	QDataStream & operator >>
	( QDataStream & inStream, Model::Color::Color & outColor );

	QDataStream & operator <<
	( QDataStream & inStream, Model::Piece::PieceType inPieceType );

	QDataStream & operator >>
	( QDataStream & inStream, Model::Piece::PieceType & outPieceType );

	QDataStream & operator <<
	( QDataStream & inStream, Model::Piece const & inPiece );

	QDataStream & operator >>
	( QDataStream & inStream, Model::Piece & outPiece );

	QDataStream & operator <<
	( QDataStream & inStream, Model::PlacedPiece const & inPiece );

	QDataStream & operator >>
	( QDataStream & inStream, Model::PlacedPiece & outPiece );

	QDataStream & operator <<
	( QDataStream & inStream, Model::Player const & inPlayer );

	QDataStream & operator >>
	( QDataStream & inStream, Model::Player & outPlayer );

	QDataStream & operator <<
	( QDataStream & inStream, Model::Tile const & inTile );

	QDataStream & operator >>
	( QDataStream & inStream, Model::Tile & outTile );

	QDataStream & operator <<
	( QDataStream & inStream, Model::Rotation inRotation );

	QDataStream & operator >>
	( QDataStream & inStream, Model::Rotation & outRotation );

	QDataStream & operator <<
	( QDataStream & inStream, Model::TileOnBoard const & inTile );

	QDataStream & operator >>
	( QDataStream & inStream, Model::TileOnBoard & outTile );
}

#endif