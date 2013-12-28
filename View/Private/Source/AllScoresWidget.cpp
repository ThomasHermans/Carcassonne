#include "AllScoresWidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

#include <cassert>

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
View::AllScoresWidget::addPlayer( std::string const & inName )
{
	QHBoxLayout * row = new QHBoxLayout();
	QLabel * name = new QLabel( QString::fromUtf8( inName.c_str() ), this );
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