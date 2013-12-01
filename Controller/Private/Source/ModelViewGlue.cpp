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
Controller::xFromArea( Area::Area inArea )
{
	switch ( inArea )
	{
	case Area::kLeftTop:
	case Area::kLeft:
	case Area::kLeftBottom:
		return .15 * Gui::kTileWidth;
	case Area::kTopLeft:
	case Area::kBottomLeft:
		return .3 * Gui::kTileWidth;
	case Area::kTop:
	case Area::kCentral:
	case Area::kBottom:
		return .5 * Gui::kTileWidth;
	case Area::kTopRight:
	case Area::kBottomRight:
		return .7 * Gui::kTileWidth;
	case Area::kRightTop:
	case Area::kRight:
	case Area::kRightBottom:
		return .85 * Gui::kTileWidth;
	}
	assert( !"Invalid Area" );
	return .5 * Gui::kTileWidth;
}

int
Controller::yFromArea( Area::Area inArea )
{
	switch ( inArea )
	{
	case Area::kTopLeft:
	case Area::kTop:
	case Area::kTopRight:
		return .15 * Gui::kTileHeight;
	case Area::kLeftTop:
	case Area::kRightTop:
		return .3 * Gui::kTileHeight;
	case Area::kLeft:
	case Area::kCentral:
	case Area::kRight:
		return .5 * Gui::kTileHeight;
	case Area::kLeftBottom:
	case Area::kRightBottom:
		return .7 * Gui::kTileHeight;
	case Area::kBottomLeft:
	case Area::kBottom:
	case Area::kBottomRight:
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

Area::Area
Controller::areaFromPos( int inX, int inY )
{
	using namespace Gui;
	if ( inX < kFirstBorder )
	{
		if ( inY < kFirstBorder )
		{
			if ( inY < inX )
				return Area::kTopLeft;
			else
				return Area::kLeftTop;
		}
		else if ( kSecondBorder < inY )
		{
			if ( kTileHeight - inY < inX )
				return Area::kBottomLeft;
			else
				return Area::kLeftBottom;
		}
		else if ( kFirstBorder < inY && inY < kSecondBorder )
			return Area::kLeft;
	}
	else if ( kSecondBorder < inX )
	{
		if ( inY < kFirstBorder )
		{
			if ( inY < kTileWidth - inX )
				return Area::kTopRight;
			else
				return Area::kRightTop;
		}
		else if ( kSecondBorder < inY )
		{
			if ( kTileHeight - inY < kTileWidth - inX )
				return Area::kBottomRight;
			else
				return Area::kRightBottom;
		}
		else if ( kFirstBorder < inY && inY < kSecondBorder )
			return Area::kRight;
	}
	else if ( kFirstBorder < inX && inX < kSecondBorder )
	{
		if ( inY < kFirstBorder )
			return Area::kTop;
		else if ( kSecondBorder < inY )
			return Area::kBottom;
		if ( kFirstBorder < inY && inY < kSecondBorder )
			return Area::kCentral;
	}
	assert( !"Invalid Area" );
	return Area::kCentral;
}

Color::Color
Controller::modelFromView( View::Color inColor )
{
	switch ( inColor )
	{
	case View::kRed:
		return Color::kRed;
	case View::kGreen:
		return Color::kGreen;
	case View::kBlue:
		return Color::kBlue;
	case View::kYellow:
		return Color::kYellow;
	case View::kBlack:
		return Color::kBlack;
	case View::kGray:
		return Color::kGray;
	}
	assert( !"Invalid Dragging Color" );
	return Color::kRed;
}

View::Color
Controller::viewFromModel( Color::Color inColor )
{
	switch ( inColor )
	{
	case Color::kRed:
		return View::kRed;
	case Color::kGreen:
		return View::kGreen;
	case Color::kBlue:
		return View::kBlue;
	case Color::kYellow:
		return View::kYellow;
	case Color::kBlack:
		return View::kBlack;
	case Color::kGray:
		return View::kGray;
	}
	assert( !"Invalid Color Color" );
	return View::kRed;
}

Piece::PieceType
Controller::modelFromView( View::Piece inPiece )
{
	switch ( inPiece )
	{
	case View::kFollower:
		return Piece::kFollower;
	case View::kLargeFollower:
		return Piece::kLargeFollower;
	case View::kBuilder:
		return Piece::kBuilder;
	case View::kPig:
		return Piece::kPig;
	}
	assert( !"Invalid Dragging Piece" );
	return Piece::kFollower;
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