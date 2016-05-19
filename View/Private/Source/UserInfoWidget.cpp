#include "UserInfoWidget.h"

#include "DragMeepleLabel.h"

#include "View/Meeple.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

#include <cassert>
#include <iostream>

namespace
{
	QString
	getColor( View::Color inColor )
	{
		switch ( inColor )
		{
			case View::kRed:
				return "red";
			case View::kGreen:
				return "green";
			case View::kBlue:
				return "blue";
			case View::kYellow:
				return "yellow";
			case View::kBlack:
				return "black";
			case View::kGray:
				return "gray";
		}
		assert( !"Invalid color" );
		return "red";
	}

	QString
	getBackgroundColor( View::Color inColor )
	{
		switch ( inColor )
		{
			case View::kRed:
			case View::kGreen:
			case View::kBlue:
			case View::kBlack:
			case View::kGray:
				return "yellow";
			case View::kYellow:
				return "darkGray";
		}
		assert( !"Invalid color" );
		return "none";
	}

	QString
	getStyleSheet( View::Color inColor )
	{
		QString styleSheet( "QLabel { color: __COLOR__; } QWidget { background-color: __BGCOLOR__; }");
		styleSheet.replace( "__COLOR__", getColor( inColor ) );
		styleSheet.replace( "__BGCOLOR__", getBackgroundColor( inColor ) );
		return styleSheet;
	}
}

View::UserInfoWidget::UserInfoWidget
(
	std::string const & inName,
	Color inColor,
	QWidget * inParent
)
:
	QWidget( inParent ),
	mColor( inColor ),
	mMainLayout( new QVBoxLayout( this ) ),
	mNameLabel( new QLabel( QString::fromStdString( inName ), this ) ),
	mScoreLabel( new QLabel( QString::number( 0 ), this ) ),
	mDragLabels()
{
	setObjectName( "UserInfoWidget" );
	setContentsMargins( 5, 5, 5, 5 );
	setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );

	mMainLayout->setContentsMargins( 0, 0, 0, 0 );
	mMainLayout->setSpacing( 0 );

	QHBoxLayout * nameAndScoreLayout = new QHBoxLayout();
	nameAndScoreLayout->setContentsMargins( 0, 0, 0, 0 );
	nameAndScoreLayout->setSpacing( 0 );
	nameAndScoreLayout->addWidget( mNameLabel, 1 );
	nameAndScoreLayout->addWidget( mScoreLabel, 0 );

	mMainLayout->addLayout( nameAndScoreLayout );

	setStyleSheet( getStyleSheet( inColor ) );
}

void
View::UserInfoWidget::setScore( std::size_t inScore )
{
	mScoreLabel->setText( QString::number( inScore ) );
}

void
View::UserInfoWidget::setSupply( Meeple::MeepleType inType, std::size_t inAmount )
{
	if ( mDragLabels.find( inType ) == mDragLabels.end() && inAmount > 0 )
	{
		DragMeepleLabel * dragLabel = new DragMeepleLabel( Meeple( inType, mColor ), this );
		mMainLayout->addWidget( dragLabel );
		mDragLabels.emplace( inType, dragLabel );
	}
	if ( mDragLabels.find( inType ) != mDragLabels.end() )
	{
		mDragLabels.find( inType )->second->setAmount( inAmount );
	}
}