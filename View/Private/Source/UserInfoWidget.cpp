#include "UserInfoWidget.h"

#include "DragMeepleLabel.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

#include <cassert>

namespace
{
	QString
	GetColor( View::Color inColor )
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
	GetBackgroundColor( View::Color inColor )
	{
		switch ( inColor )
		{
			case View::kRed:
			case View::kGreen:
			case View::kBlue:
			case View::kBlack:
			case View::kGray:
				return "none";
			case View::kYellow:
				return "darkGray";
		}
		assert( !"Invalid color" );
		return "none";
	}

	QString
	GetStyleSheet( View::Color inColor )
	{
		QString styleSheet( "QLabel { color: __COLOR__; } QWidget { background-color: __BGCOLOR__; }");
		styleSheet.replace( "__COLOR__", GetColor( inColor ) );
		styleSheet.replace( "__BGCOLOR__", GetBackgroundColor( inColor ) );
		return styleSheet;
	}
}

View::UserInfoWidget::UserInfoWidget
(
	std::string const & inName,
	View::Color inColor,
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

	mDragFollowersLabel = new DragMeepleLabel( View::kFollower, inNumberOfFollowers, inColor, this );
	mDragFollowersLabel->setObjectName( "mDragFollowersLabel" );
	layout->addWidget( mDragFollowersLabel );

	setLayout( layout );
	setFixedSize( sizeHint() );

	setStyleSheet( GetStyleSheet( inColor ) );
}

View::UserInfoWidget::~UserInfoWidget()
{
}

void
View::UserInfoWidget::setScore( unsigned inScore )
{
	mScoreLabel->setText( QString::number( inScore ) );
}

void
View::UserInfoWidget::setNumberOfFollowers( unsigned inNumberOfFollowers )
{
	mDragFollowersLabel->setNr( inNumberOfFollowers );
}
