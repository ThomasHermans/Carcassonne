#include "src-view/UserInfoWidget.h"

#include "src-view/DragMeepleLabel.h"

#include <QLabel>
#include <QVBoxLayout>

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
			default:
				return "red";
		}
	}

	QString
	GetStyleSheet( Dragging::Color inColor )
	{
		QString styleSheet( "QLabel { color: __COLOR__; }");
		styleSheet.replace( "__COLOR__", GetColor( inColor ) );
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
	setContentsMargins( 0, 0, 0, 0 );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setContentsMargins( 0, 0, 0, 0 );
	layout->setSpacing( 0 );

	mNameLabel = new QLabel( QString::fromStdString( inName ), this );
	mNameLabel->setObjectName( "mNameLabel" );
	layout->addWidget( mNameLabel );

	mScoreLabel = new QLabel( QString::number( 0 ), this );
	mScoreLabel->setObjectName( "mScoreLabel" );
	layout->addWidget( mScoreLabel );

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