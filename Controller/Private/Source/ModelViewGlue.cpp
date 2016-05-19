#include "ModelViewGlue.h"

#include <cassert>

int
Controller::colFromX( int inX )
{
	return ( inX / Gui::kTileWidth - ( inX < 0 ? 1 : 0 ) );
}

int
Controller::rowFromY( int inY )
{
	return ( inY / Gui::kTileHeight - ( inY < 0 ? 1 : 0 ) );
}

int
Controller::xFromCol( int inCol )
{
	return inCol * Gui::kTileWidth;
}

int
Controller::yFromRow( int inRow )
{
	return inRow * Gui::kTileHeight;
}

int
Controller::xFromArea( Model::Area::Area inArea )
{
	switch ( inArea )
	{
	case Model::Area::kLeftTop:
	case Model::Area::kLeft:
	case Model::Area::kLeftBottom:
		return .15 * Gui::kTileWidth;
	case Model::Area::kTopLeft:
	case Model::Area::kBottomLeft:
		return .3 * Gui::kTileWidth;
	case Model::Area::kTop:
	case Model::Area::kCentral:
	case Model::Area::kBottom:
		return .5 * Gui::kTileWidth;
	case Model::Area::kTopRight:
	case Model::Area::kBottomRight:
		return .7 * Gui::kTileWidth;
	case Model::Area::kRightTop:
	case Model::Area::kRight:
	case Model::Area::kRightBottom:
		return .85 * Gui::kTileWidth;
	}
	assert( !"Invalid Area" );
	return .5 * Gui::kTileWidth;
}

int
Controller::yFromArea( Model::Area::Area inArea )
{
	switch ( inArea )
	{
	case Model::Area::kTopLeft:
	case Model::Area::kTop:
	case Model::Area::kTopRight:
		return .15 * Gui::kTileHeight;
	case Model::Area::kLeftTop:
	case Model::Area::kRightTop:
		return .3 * Gui::kTileHeight;
	case Model::Area::kLeft:
	case Model::Area::kCentral:
	case Model::Area::kRight:
		return .5 * Gui::kTileHeight;
	case Model::Area::kLeftBottom:
	case Model::Area::kRightBottom:
		return .7 * Gui::kTileHeight;
	case Model::Area::kBottomLeft:
	case Model::Area::kBottom:
	case Model::Area::kBottomRight:
		return .85 * Gui::kTileHeight;
	}
	assert( !"Invalid Area" );
	return .5 * Gui::kTileHeight;
}

int
Controller::xFromLocation( Utils::Location const & inLocation, Model::Area::Area inArea )
{
	return xFromCol( inLocation.col ) + xFromArea( inArea ) - .5 * Gui::kTileWidth;
}

int
Controller::yFromLocation( Utils::Location const & inLocation, Model::Area::Area inArea )
{
	return yFromRow( inLocation.row ) + yFromArea( inArea ) - .5 * Gui::kTileHeight;
}

Utils::Location
Controller::locationFromXY( int inX, int inY )
{
	return Utils::Location( rowFromY( inY ), colFromX( inX ) );
}

int
Controller::posXFromX( int inX )
{
	int res = ( inX % Gui::kTileWidth );
	if ( res < 0 )
	{
		res += Gui::kTileWidth;
	}
	return res;
}

int
Controller::posYFromY( int inY )
{
	int res = ( inY % Gui::kTileHeight );
	if ( res < 0 )
	{
		res += Gui::kTileHeight;
	}
	return res;
}

Model::Area::Area
Controller::areaFromPos( int inX, int inY )
{
	using namespace Gui;
	int x = posXFromX( inX );
	int y = posYFromY( inY );
	if ( x < kFirstBorder )
	{
		if ( y < kFirstBorder )
		{
			if ( y < x )
				return Model::Area::kTopLeft;
			else
				return Model::Area::kLeftTop;
		}
		else if ( kSecondBorder < y )
		{
			if ( kTileHeight - y < x )
				return Model::Area::kBottomLeft;
			else
				return Model::Area::kLeftBottom;
		}
		else if ( kFirstBorder < y && y < kSecondBorder )
			return Model::Area::kLeft;
	}
	else if ( kSecondBorder < x )
	{
		if ( y < kFirstBorder )
		{
			if ( y < kTileWidth - x )
				return Model::Area::kTopRight;
			else
				return Model::Area::kRightTop;
		}
		else if ( kSecondBorder < y )
		{
			if ( kTileHeight - y < kTileWidth - x )
				return Model::Area::kBottomRight;
			else
				return Model::Area::kRightBottom;
		}
		else if ( kFirstBorder < y && y < kSecondBorder )
			return Model::Area::kRight;
	}
	else if ( kFirstBorder < x && x < kSecondBorder )
	{
		if ( y < kFirstBorder )
			return Model::Area::kTop;
		else if ( kSecondBorder < y )
			return Model::Area::kBottom;
		if ( kFirstBorder < y && y < kSecondBorder )
			return Model::Area::kCentral;
	}
	assert( !"Invalid Area" );
	return Model::Area::kCentral;
}

Model::Color::Color
Controller::modelFromView( View::Color inColor )
{
	switch ( inColor )
	{
	case View::kRed:
		return Model::Color::kRed;
	case View::kGreen:
		return Model::Color::kGreen;
	case View::kBlue:
		return Model::Color::kBlue;
	case View::kYellow:
		return Model::Color::kYellow;
	case View::kBlack:
		return Model::Color::kBlack;
	case View::kGray:
		return Model::Color::kGray;
	}
	assert( !"Invalid Dragging Color" );
	return Model::Color::kRed;
}

View::Color
Controller::viewFromModel( Model::Color::Color inColor )
{
	switch ( inColor )
	{
	case Model::Color::kRed:
		return View::kRed;
	case Model::Color::kGreen:
		return View::kGreen;
	case Model::Color::kBlue:
		return View::kBlue;
	case Model::Color::kYellow:
		return View::kYellow;
	case Model::Color::kBlack:
		return View::kBlack;
	case Model::Color::kGray:
		return View::kGray;
	}
	assert( !"Invalid Color Color" );
	return View::kRed;
}

Model::Piece::PieceType
Controller::modelFromView( View::Meeple::MeepleType inType )
{
	switch ( inType )
	{
	case View::Meeple::kFollower:
		return Model::Piece::kFollower;
	case View::Meeple::kLargeFollower:
		return Model::Piece::kLargeFollower;
	case View::Meeple::kBuilder:
		return Model::Piece::kBuilder;
	case View::Meeple::kPig:
		return Model::Piece::kPig;
	}
	assert( !"Invalide View::Meeple::PieceType" );
	return Model::Piece::kFollower;
}

View::Meeple::MeepleType
Controller::viewFromModel( Model::Piece::PieceType inType )
{
	switch ( inType )
	{
	case Model::Piece::kFollower:
		return View::Meeple::kFollower;
	case Model::Piece::kLargeFollower:
		return View::Meeple::kLargeFollower;
	case Model::Piece::kBuilder:
		return View::Meeple::kBuilder;
	case Model::Piece::kPig:
		return View::Meeple::kPig;
	}
	assert( !"Invalide Model::Piece::PieceType" );
	return View::Meeple::kFollower;
}

Model::Piece
Controller::modelFromView( View::Meeple const & inMeeple )
{
	return Model::Piece( modelFromView( inMeeple.getType() ), modelFromView( inMeeple.getColor() ) );
}

View::Meeple
Controller::viewFromModel( Model::Piece const & inPiece )
{
	return View::Meeple( viewFromModel( inPiece.getType() ), viewFromModel( inPiece.getColor() ) );
}

Model::Rotation
Controller::modelFromView( View::Rotation inRotation )
{
	switch ( inRotation )
	{
	case View::kCw0:
		return Model::kCw0;
	case View::kCw90:
		return Model::kCw90;
	case View::kCw180:
		return Model::kCw180;
	case View::kCw270:
		return Model::kCw270;
	}
	assert( !"Invalid View Rotation" );
	return Model::kCw0;
}

View::Rotation
Controller::viewFromModel( Model::Rotation inRotation )
{
	switch ( inRotation )
	{
	case Model::kCw0:
		return View::kCw0;
	case Model::kCw90:
		return View::kCw90;
	case Model::kCw180:
		return View::kCw180;
	case Model::kCw270:
		return View::kCw270;
	}
	assert( !"Invalid Model Rotation" );
	return View::kCw0;
}