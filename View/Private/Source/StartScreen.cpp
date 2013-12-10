#include "View/StartScreen.h"

#include "StartScreenRow.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include <cassert>
#include <set>

View::StartScreen::StartScreen( QWidget * inParent )
:
	QDialog( inParent ),
	mLayout( new QVBoxLayout() ),
	mPlayerRows(),
	mAddPlayerButton( new QPushButton( "Add player", this ) ),
	mPlayButton( new QPushButton( "Start game", this ) )
{
	connect( mAddPlayerButton, SIGNAL( clicked() ), this, SLOT( addPlayer() ) );
	mPlayButton->setAutoDefault( true );
	mPlayButton->setDefault( true );
	connect( mPlayButton, SIGNAL( clicked() ), this, SLOT( playClicked() ) );

	setLayout( mLayout );

	mLayout->addWidget( mAddPlayerButton );
	mLayout->addWidget( mPlayButton );
	mLayout->addStretch();
}

bool
View::StartScreen::addPlayer( QString const & inName, Color inColor )
{
	if ( addPlayer() )
	{
		mPlayerRows.back()->setName( inName );
		mPlayerRows.back()->setColor( inColor );
		return true;
	}
	else
	{
		return false;
	}
}

View::Color
View::StartScreen::findUnusedColor() const
{
	std::set< Color > colors;
	colors.insert( kRed );
	colors.insert( kGreen );
	colors.insert( kBlue );
	colors.insert( kYellow );
	colors.insert( kBlack );
	colors.insert( kGray );
	for ( std::vector< StartScreenRow * >::const_iterator it = mPlayerRows.begin();
		it != mPlayerRows.end();
		++it )
	{
		colors.erase( (*it)->getColor() );
	}
	assert( !colors.empty() );
	return *colors.begin();
}

bool
View::StartScreen::addPlayer()
{
	if ( mPlayerRows.size() < 6 )
	{
		StartScreenRow * row = new StartScreenRow( this );
		Color color = findUnusedColor();
		row->setColor( color );
		connect( row, SIGNAL( removed() ), this, SLOT( removePlayer() ) );
		connect( row, SIGNAL( colorChanged( Color ) ), this, SLOT( updateColors( Color ) ) );
		mLayout->insertWidget( mPlayerRows.size(), row );
		mPlayerRows.push_back( row );
		return true;
	}
	else
	{
		return false;
	}
}

void
View::StartScreen::removePlayer()
{
	StartScreenRow * senderRow = qobject_cast< StartScreenRow * >( QObject::sender() );
	if ( senderRow )
	{
		for ( std::vector< StartScreenRow * >::iterator it = mPlayerRows.begin();
			it != mPlayerRows.end();
			++it )
		{
			if ( senderRow == *it )
			{
				mLayout->removeWidget( senderRow );
				senderRow->deleteLater();
				mPlayerRows.erase( it );
				break;
			}
		}
	}
}

void
View::StartScreen::updateColors( Color inColor )
{
	StartScreenRow * senderRow = qobject_cast< StartScreenRow * >( QObject::sender() );
	if ( senderRow )
	{
		for ( std::vector< StartScreenRow * >::iterator it = mPlayerRows.begin();
			it != mPlayerRows.end();
			++it )
		{
			if ( senderRow != *it && (*it)->getColor() == inColor )
			{
				Color const color = findUnusedColor();
				(*it)->setColor( color );
			}
		}
	}
}

void
View::StartScreen::playClicked()
{
	std::map< Color, std::string > players;
	for ( std::vector< StartScreenRow * >::const_iterator it = mPlayerRows.begin();
		it != mPlayerRows.end();
		++it )
	{
		players[ (*it)->getColor() ] = (*it)->getName().toUtf8().constData();
	}
	emit startGame( players );
}
