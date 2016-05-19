#include "AllScoresWidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

#include <cassert>

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
	getStyleSheet( View::Color inColor )
	{
		QString styleSheet( "QLabel { color: __COLOR__; }");
		styleSheet.replace( "__COLOR__", getColor( inColor ) );
		return styleSheet;
	}
}

View::AllScoresWidget::AllScoresWidget( QWidget * inParent )
:
	QWidget( inParent ),
	mLayout( new QVBoxLayout() ),
	mScores()
{
	setContentsMargins( 0, 0, 0, 0 );
	mLayout->setContentsMargins( 0, 0, 0, 0 );
	setLayout( mLayout );
}

View::AllScoresWidget::~AllScoresWidget()
{
}

void
View::AllScoresWidget::addPlayer( std::string const & inName, View::Color inColor )
{
	QHBoxLayout * row = new QHBoxLayout();
	QLabel * name = new QLabel( QString::fromUtf8( inName.c_str() ), this );
	name->setStyleSheet( getStyleSheet( inColor ) );
	row->addWidget( name );
	row->addStretch();
	QLabel * score = new QLabel( QString::number( 0 ), this );
	row->addWidget( score );

	mLayout->addLayout( row );

	mScores[inName] = score;
}

void
View::AllScoresWidget::setScore( std::string const & inName, unsigned inScore )
{
	assert( mScores.find( inName ) != mScores.end() );
	mScores[ inName ]->setText( QString::number( inScore ) );
}
