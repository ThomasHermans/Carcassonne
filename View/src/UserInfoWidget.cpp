#include "View/src/UserInfoWidget.h"

#include "View/src/DragMeepleLabel.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

#include <cassert>

namespace
{
	QString
	GetColor( Dragging::Color inColor )
	{
		switch ( inColor )
		{
			case Dragging::kRed:
				return "red";
			case Dragging::kGreen:
				return "green";
			case Dragging::kBlue:
				return "blue";
			case Dragging::kYellow:
				return "yellow";
			case Dragging::kBlack:
				return "black";
			case Dragging::kGray:
				return "gray";
		}
		assert( !"Invalid color" );
		return "red";
	}

	QString
	GetBackgroundColor( Dragging::Color inColor )
	{
		switch ( inColor )
		{
			case Dragging::kRed:
			case Dragging::kGreen:
			case Dragging::kBlue:
			case Dragging::kBlack:
			case Dragging::kGray:
				return "none";
			case Dragging::kYellow:
				return "darkGray";
		}
		assert( !"Invalid color" );
		return "none";
	}

	QString
	GetStyleSheet( Dragging::Color inColor )
	{
		QString styleSheet( "QLabel { color: __COLOR__; } QWidget { background-color: __BGCOLOR__; }");
		styleSheet.replace( "__COLOR__", GetColor( inColor ) );
		styleSheet.replace( "__BGCOLOR__", GetBackgroundColor( inColor ) );
		return styleSheet;
	}
}

UserInfoWidget::UserInfoWidget
(
	std::string const & inName,
	Dragging::Color inColor,
	unsigned inNumberOfFollowers,
	QWidget * inParent
)
:
	QWidget( inParent ),
	mNameLabel(),
	mScoreLabel(),
	mDragFollowersLabel()
{
	setObjectName( "UserInfoWidget" );
	setContentsMargins( 5, 5, 5, 5 );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setContentsMargins( 0, 0, 0, 0 );
	layout->setSpacing( 0 );

	QHBoxLayout * rowLayout = new QHBoxLayout();
	rowLayout->setContentsMargins( 0, 0, 0, 0 );
	rowLayout->setSpacing( 0 );

	mNameLabel = new QLabel( QString::fromStdString( inName ), this );
	mNameLabel->setObjectName( "mNameLabel" );
	rowLayout->addWidget( mNameLabel );

	rowLayout->addStretch();

	mScoreLabel = new QLabel( QString::number( 0 ), this );
	mScoreLabel->setObjectName( "mScoreLabel" );
	rowLayout->addWidget( mScoreLabel );

	layout->addLayout( rowLayout );

	mDragFollowersLabel = new DragMeepleLabel( Dragging::kFollower, inNumberOfFollowers, inColor, this );
	mDragFollowersLabel->setObjectName( "mDragFollowersLabel" );
	layout->addWidget( mDragFollowersLabel );

	setLayout( layout );
	setFixedSize( sizeHint() );

	setStyleSheet( GetStyleSheet( inColor ) );
}

UserInfoWidget::~UserInfoWidget()
{
}

void
UserInfoWidget::setScore( unsigned inScore )
{
	mScoreLabel->setText( QString::number( inScore ) );
}

void
UserInfoWidget::setNumberOfFollowers( unsigned inNumberOfFollowers )
{
	mDragFollowersLabel->setNr( inNumberOfFollowers );
}
