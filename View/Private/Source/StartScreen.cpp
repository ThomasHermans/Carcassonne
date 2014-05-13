#include "View/StartScreen.h"

#include "StartScreenRow.h"

#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include <boost/foreach.hpp>

#include <cassert>

namespace
{
	std::size_t kMaxPlayers = 6;
}

View::PlayerInfo::PlayerInfo( std::string const & inName, Color inColor )
:
	name( inName ),
	color( inColor )
{
}

View::StartScreen::StartScreen()
:
	QDialog(),
	mLayout( new QVBoxLayout() ),
	mPlayerRows(),
	mAddPlayerButton( new QPushButton( "Add player", this ) ),
	mBaseGameBox( new QCheckBox( "Base Game", this ) ),
	mTheExpansionBox( new QCheckBox( "The Expansion", this ) ),
	mPlayButton( new QPushButton( "Start game", this ) )
{
	connect( mAddPlayerButton, SIGNAL( clicked() ), this, SLOT( addPlayer() ) );
	mPlayButton->setAutoDefault( true );
	mPlayButton->setDefault( true );
	connect( mPlayButton, SIGNAL( clicked() ), this, SLOT( playClicked() ) );

	setLayout( mLayout );

	mLayout->addWidget( mAddPlayerButton );
	mBaseGameBox->setChecked( true );
	mBaseGameBox->setEnabled( false );
	mLayout->addWidget( mBaseGameBox );
	mLayout->addWidget( mTheExpansionBox );
	mLayout->addWidget( mPlayButton );
	mLayout->addStretch();
}

View::StartScreen::~StartScreen()
{
}

bool
View::StartScreen::addPlayer( std::string const & inName, Color inColor )
{
	if ( addPlayer() )
	{
		mPlayerRows.back().setName( inName );
		mPlayerRows.back().setColor( inColor );
		return true;
	}
	else
	{
		return false;
	}
}

void
View::StartScreen::selectExpansions( std::set< Utils::Expansion::Type > const & inExpansions )
{
	mTheExpansionBox->setChecked( inExpansions.count( Utils::Expansion::kTheExpansion ) > 0 );
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
	for ( boost::ptr_vector< StartScreenRow >::const_iterator it = mPlayerRows.begin();
		it != mPlayerRows.end();
		++it )
	{
		colors.erase( it->getColor() );
	}
	assert( !colors.empty() );
	return *colors.begin();
}

bool
View::StartScreen::addPlayer()
{
	if ( mPlayerRows.size() < kMaxPlayers )
	{
		StartScreenRow * row = new StartScreenRow( this );
		Color const color = findUnusedColor();
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
		for ( boost::ptr_vector< StartScreenRow >::iterator it = mPlayerRows.begin();
			it != mPlayerRows.end();
			++it )
		{
			if ( senderRow == &(*it) )
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
		BOOST_FOREACH( StartScreenRow & row, mPlayerRows )
		{
			if ( senderRow != &row && row.getColor() == inColor )
			{
				Color const color = findUnusedColor();
				row.setColor( color );
			}
		}
	}
}

void
View::StartScreen::playClicked()
{
	startGame( getSelectedExpansions(), getPlayers() );
}

std::set< Utils::Expansion::Type >
View::StartScreen::getSelectedExpansions() const
{
	std::set< Utils::Expansion::Type > expansions;
	if ( mBaseGameBox->isChecked() )
	{
		expansions.insert( Utils::Expansion::kBaseGame );
	}
	if ( mTheExpansionBox->isChecked() )
	{
		expansions.insert( Utils::Expansion::kTheExpansion );
	}
	return expansions;
}

std::vector< View::PlayerInfo >
View::StartScreen::getPlayers() const
{
	std::vector< PlayerInfo > players;
	BOOST_FOREACH( StartScreenRow const & row, mPlayerRows )
	{
		players.push_back( PlayerInfo( row.getName().toStdString(), row.getColor() ) );
	}
	return players;
}