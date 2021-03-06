#include "MeepleUtils.h"

#include "QtGlue.h"

#include "View/Meeple.h"

#include <QBitmap>
#include <QPainter>
#include <QPen>

#include <cassert>

namespace
{
	QBitmap
	getFollowerMask( bool inFull )
	{
		QPixmap const original = QPixmap( inFull ? ":/Follower_own.png" : ":/Follower_own_empty.png" );
		QPixmap const scaled = original.scaled( Gui::kMeepleWidth, Gui::kMeepleHeight );
		return scaled.createMaskFromColor( Qt::transparent );
	}

	QBitmap
	getLargeFollowerMask( bool inFull )
	{
		QPixmap const original = QPixmap( inFull ? ":/LargeFollower_own.png" : ":/LargeFollower_own_empty.png" );
		QPixmap const scaled = original.scaled( Gui::kMeepleWidth, Gui::kMeepleHeight );
		return scaled.createMaskFromColor( Qt::transparent );
	}

	QBitmap
	getMask( View::Meeple::MeepleType inType, bool inFull )
	{
		switch ( inType )
		{
			case View::Meeple::kFollower:
				return getFollowerMask( inFull );
			case View::Meeple::kLargeFollower:
				return getLargeFollowerMask( inFull );
		}
		assert( !"Invalide View::Meeple type" );
		return getFollowerMask( inFull );
	}

	QPixmap
	getMeeplePixmap( View::Meeple const & inMeeple, bool inFull )
	{
		QBitmap const mask = getMask( inMeeple.getType(), inFull );
		QPixmap result( mask.size() );
		result.fill( toQColor( inMeeple.getColor() ) );
		result.setMask( mask );
		return result;
	}
}

QPainterPath
View::getMeeplePath( int inX, int inY, int inWidth, int inHeight )
{
	QPointF const top(				inX + 0.5 * inWidth,	inY + 0		);
	QPointF const topRShoulder1(	inX + 0.7 * inWidth,	inY + 0		);
	QPointF const rightShoulder(	inX + 0.65 * inWidth,	inY + 0.25 * inHeight );
	QPointF const rightHand(		inX + inWidth,			inY + 0.4 * inHeight );
	QPointF const rightAboveHand(	inX + inWidth,			inY + 0.3 * inHeight );
	QPointF const rightBelowHand(	inX + inWidth,			inY + 0.5 * inHeight );
	QPointF const rightArmpit(		inX + 0.7 * inWidth,	inY + 0.55 * inHeight );
	QPointF const rightAboveToe(	inX + 0.9 * inWidth,	inY + 0.8 * inHeight	);
	QPointF const rightToe(			inX + 0.9 * inWidth,	inY + inHeight		);
	QPointF const rightHeel(		inX + 0.7 * inWidth,	inY + inHeight		);
	QPointF const crotch(			inX + 0.5 * inWidth,	inY + 0.75 * inHeight );
	QPointF const leftHeel(			inX + 0.3 * inWidth,	inY + inHeight		);
	QPointF const leftToe(			inX + 0.1 * inWidth,	inY + inHeight		);
	QPointF const leftAboveToe(		inX + 0.1 * inWidth,	inY + 0.8 * inHeight	);
	QPointF const leftArmpit(		inX + 0.3 * inWidth,	inY + 0.55 * inHeight );
	QPointF const leftBelowHand(	inX + 0,				inY + 0.5 * inHeight );
	QPointF const leftHand(			inX + 0,				inY + 0.4 * inHeight );
	QPointF const leftAboveHand(	inX + 0,				inY + 0.3 * inHeight );
	QPointF const leftShoulder(		inX + 0.35 * inWidth,	inY + 0.25 * inHeight );
	QPointF const topLShoulder1(	inX + 0.3 * inWidth,	inY + 0		);
	QPainterPath path;
	path.moveTo( top );
	path.cubicTo( topRShoulder1, rightShoulder, rightShoulder );
	path.cubicTo( rightShoulder, rightAboveHand, rightHand );
	path.cubicTo( rightBelowHand, rightArmpit, rightArmpit );
	path.cubicTo( rightArmpit, rightAboveToe, rightToe );
	path.lineTo( rightHeel );
	path.cubicTo( crotch, crotch, leftHeel );
	path.lineTo( leftToe );
	path.cubicTo( leftAboveToe, leftArmpit, leftArmpit );
	path.cubicTo( leftArmpit, leftBelowHand, leftHand );
	path.cubicTo( leftAboveHand, leftShoulder, leftShoulder );
	path.cubicTo( leftShoulder, topLShoulder1, top );
	return path;
}

QPixmap
View::getMeeplePixmap( Meeple const & inMeeple )
{
	return ::getMeeplePixmap( inMeeple, true );
}

QPixmap
View::getRemovedMeeplePixmap( Meeple const & inMeeple )
{
	return ::getMeeplePixmap( inMeeple, false );
}