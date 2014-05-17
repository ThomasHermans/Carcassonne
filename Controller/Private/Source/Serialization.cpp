#include "Serialization.h"

QDataStream &
Controller::operator <<
( QDataStream & inStream, Model::Area::Area inArea )
{
	inStream << quint32( inArea );
	return inStream;
}

QDataStream &
Controller::operator >>
( QDataStream & inStream, Model::Area::Area & outArea )
{
	quint32 area;
	inStream >> area;
	outArea = Model::Area::Area( area );
	return inStream;
}

QDataStream &
Controller::operator <<
( QDataStream & inStream, Model::Color::Color inColor )
{
	inStream << quint32( inColor );
	return inStream;
}

QDataStream &
Controller::operator >>
( QDataStream & inStream, Model::Color::Color & outColor )
{
	quint32 color;
	inStream >> color;
	outColor = Model::Color::Color( color );
	return inStream;
}

QDataStream &
Controller::operator <<
( QDataStream & inStream, Model::Piece const & inPiece )
{
	inStream << quint32( inPiece.getType() ) << quint32( inPiece.getColor() );
	return inStream;
}

QDataStream &
Controller::operator >>
( QDataStream & inStream, Model::Piece & outPiece )
{
	quint32 pieceType;
	Model::Color::Color color;
	inStream >> pieceType >> color;
	outPiece = Model::Piece( Model::Piece::PieceType( pieceType ), color );
	return inStream;
}

QDataStream &
Controller::operator <<
( QDataStream & inStream, Model::PlacedPiece const & inPiece )
{
	inStream << inPiece.getPiece() << inPiece.getArea();
	return inStream;
}

QDataStream &
Controller::operator >>
( QDataStream & inStream, Model::PlacedPiece & outPiece )
{
	Model::Piece piece;
	Model::Area::Area area;
	inStream >> piece >> area;
	outPiece = Model::PlacedPiece( piece, area );
	return inStream;
}