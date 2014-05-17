#ifndef CARCASSONNE_CONTROLLER_SERIALIZATION
#define CARCASSONNE_CONTROLLER_SERIALIZATION

#include "Model/Area.h"
#include "Model/Color.h"
#include "Model/Piece.h"
#include "Model/PlacedPiece.h"

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
	( QDataStream & inStream, Model::Piece const & inPiece );

	QDataStream & operator >>
	( QDataStream & inStream, Model::Piece & outPiece );

	QDataStream & operator <<
	( QDataStream & inStream, Model::PlacedPiece const & inPiece );

	QDataStream & operator >>
	( QDataStream & inStream, Model::PlacedPiece & outPiece );
}

#endif