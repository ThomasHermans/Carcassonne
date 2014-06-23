#include "Serialization.h"

#include "Model/CreateTilesAndPieces.h"

#include <QMap>
#include <QString>

#include <map>

namespace
{
	QString
	fromStd( std::string const & inString )
	{
		return QString::fromUtf8( inString.c_str() );
	}

	std::string
	toStd( QString const & inString )
	{
		return std::string( inString.toUtf8().constData() );
	}

	QMap< quint32, quint32 >
	fromPiecesMap( std::map< Model::Piece::PieceType, std::size_t > const & inPieces )
	{
		QMap< quint32, quint32 > result;
		for ( std::map< Model::Piece::PieceType, std::size_t >::const_iterator it = inPieces.begin();
			it != inPieces.end();
			++it )
		{
			result.insert( quint32( it->first ), quint32( it->second ) );
		}
		return result;
	}

	std::map< Model::Piece::PieceType, std::size_t >
	toPiecesMap( QMap< quint32, quint32 > const & inMap )
	{
		std::map< Model::Piece::PieceType, std::size_t > result;
		for ( QMap< quint32, quint32 >::const_iterator it = inMap.begin();
			it != inMap.end();
			++it )
		{
			result[ Model::Piece::PieceType( it.key() ) ] = std::size_t( it.value() );
		}
		return result;
	}
}

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
( QDataStream & inStream, Model::Piece::PieceType inPieceType )
{
	inStream << quint32( inPieceType );
	return inStream;
}

QDataStream &
Controller::operator >>
( QDataStream & inStream, Model::Piece::PieceType & outPieceType )
{
	quint32 pieceType;
	inStream >> pieceType;
	outPieceType = Model::Piece::PieceType( pieceType );
	return inStream;
}

QDataStream &
Controller::operator <<
( QDataStream & inStream, Model::Piece const & inPiece )
{
	inStream << inPiece.getType() << quint32( inPiece.getColor() );
	return inStream;
}

QDataStream &
Controller::operator >>
( QDataStream & inStream, Model::Piece & outPiece )
{
	Model::Piece::PieceType pieceType;
	Model::Color::Color color;
	inStream >> pieceType >> color;
	outPiece = Model::Piece( pieceType, color );
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

QDataStream &
Controller::operator <<
( QDataStream & inStream, Model::Player const & inPlayer )
{
	inStream << fromStd( inPlayer.getName() );
	inStream << inPlayer.getColor();
	inStream << quint32( inPlayer.getScore() );
	inStream << fromPiecesMap( inPlayer.getPieces() );
	return inStream;
}

QDataStream &
Controller::operator >>
( QDataStream & inStream, Model::Player & outPlayer )
{
	QString name;
	Model::Color::Color color;
	quint32 score;
	QMap< quint32, quint32 > pieces;
	inStream >> name >> color >> score >> pieces;
	outPlayer = Model::Player( toStd( name ), color, std::size_t( score ), toPiecesMap( pieces ) );
	return inStream;
}

QDataStream &
Controller::operator <<
( QDataStream & inStream, Model::Tile const & inTile )
{
	inStream << fromStd( inTile.getID() );
	return inStream;
}

QDataStream &
Controller::operator >>
( QDataStream & inStream, Model::Tile & outTile )
{
	QString id;
	inStream >> id;
	outTile = Model::createTiles( toStd( id ) ).front();
	return inStream;
}