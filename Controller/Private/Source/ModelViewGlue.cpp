#include "ModelViewGlue.h"

#include <cassert>

unsigned
Controller::colFromX( int inX, unsigned inStartCol )
{
	return ( inX / Gui::kTileWidth + inStartCol - ( inX < 0 ? 1 : 0 ) );
}

unsigned
Controller::rowFromY( int inY, unsigned inStartRow )
{
	return ( inY / Gui::kTileHeight + inStartRow - ( inY < 0 ? 1 : 0 ) );
}

int
Controller::xFromCol( unsigned inCol, unsigned inStartCol )
{
	return ( ((int)inCol - (int)inStartCol) * Gui::kTileWidth );
}

int
Controller::yFromRow( unsigned inRow, unsigned inStartRow )
{
	return ( ((int)inRow - (int)inStartRow) * Gui::kTileHeight );
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
	if ( inX < kFirstBorder )
	{
		if ( inY < kFirstBorder )
		{
			if ( inY < inX )
				return Model::Area::kTopLeft;
			else
				return Model::Area::kLeftTop;
		}
		else if ( kSecondBorder < inY )
		{
			if ( kTileHeight - inY < inX )
				return Model::Area::kBottomLeft;
			else
				return Model::Area::kLeftBottom;
		}
		else if ( kFirstBorder < inY && inY < kSecondBorder )
			return Model::Area::kLeft;
	}
	else if ( kSecondBorder < inX )
	{
		if ( inY < kFirstBorder )
		{
			if ( inY < kTileWidth - inX )
				return Model::Area::kTopRight;
			else
				return Model::Area::kRightTop;
		}
		else if ( kSecondBorder < inY )
		{
			if ( kTileHeight - inY < kTileWidth - inX )
				return Model::Area::kBottomRight;
			else
				return Model::Area::kRightBottom;
		}
		else if ( kFirstBorder < inY && inY < kSecondBorder )
			return Model::Area::kRight;
	}
	else if ( kFirstBorder < inX && inX < kSecondBorder )
	{
		if ( inY < kFirstBorder )
			return Model::Area::kTop;
		else if ( kSecondBorder < inY )
			return Model::Area::kBottom;
		if ( kFirstBorder < inY && inY < kSecondBorder )
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
Controller::modelFromView( View::Piece inPiece )
{
	switch ( inPiece )
	{
	case View::kFollower:
		return Model::Piece::kFollower;
	case View::kLargeFollower:
		return Model::Piece::kLargeFollower;
	case View::kBuilder:
		return Model::Piece::kBuilder;
	case View::kPig:
		return Model::Piece::kPig;
	}
	assert( !"Invalid Dragging Piece" );
	return Model::Piece::kFollower;
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