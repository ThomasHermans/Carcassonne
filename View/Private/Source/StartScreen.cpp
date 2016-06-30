#include "View/StartScreen.h"

#include "Utils/Random.h"

#include "StartScreenRow.h"

#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include <boost/foreach.hpp>

#include <cassert>
#include <iostream>

namespace
{
	std::size_t kMaxPlayers = 6;
}

View::PlayerInfo::PlayerInfo( std::string const & inName, Color inColor, bool inIsAI )
:
	name( inName ),
	color( inColor ),
	isAI( inIsAI )
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
	connect( mAddPlayerButton, &QPushButton::clicked, [ this ]{ addPlayer(); } );
	mPlayButton->setAutoDefault( true );
	mPlayButton->setDefault( true );
	connect( mPlayButton, &QPushButton::clicked, [ this ]{ playClicked(); } );

	QPushButton * randomOrderButton = new QPushButton( "Randomize player order", this );
	connect( randomOrderButton, &QPushButton::clicked, [ this ]{ randomizePlayerOrder(); } );

	setLayout( mLayout );

	mLayout->addWidget( mAddPlayerButton );
	mLayout->addWidget( randomOrderButton );
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
View::StartScreen::addPlayer( std::string const & inName, Color inColor, bool inAI )
{
	if ( addPlayer() )
	{
		mPlayerRows.back()->setName( inName );
		mPlayerRows.back()->setColor( inColor );
		mPlayerRows.back()->setAI( inAI );
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
	std::set< Color > colors = { kRed, kGreen, kBlue, kYellow, kBlack, kGray };
	for ( std::vector< boost::shared_ptr< StartScreenRow > >::const_iterator it = mPlayerRows.begin();
		it != mPlayerRows.end();
		++it )
	{
		colors.erase( (*it)->getColor() );
	}
	assert( !colors.empty() );
	return *colors.begin();
}

std::vector< View::PlayerInfo >
View::StartScreen::getPlayers() const
{
	std::vector< PlayerInfo > players;
	for ( boost::shared_ptr< StartScreenRow > const & row : mPlayerRows )
	{
		players.push_back( PlayerInfo( row->getName().toStdString(), row->getColor(), row->isAI() ) );
	}
	return players;
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

bool
View::StartScreen::addPlayer()
{
	if ( mPlayerRows.size() < kMaxPlayers )
	{
		StartScreenRow * row = new StartScreenRow( this );
		Color const color = findUnusedColor();
		row->setColor( color );
		connect( row, &StartScreenRow::removed, [ this, row ]{ removePlayer( row ); } );
		connect( row, &StartScreenRow::colorChanged, [ this, row ]{ updateColors( row ); } );
		mLayout->insertWidget( mPlayerRows.size(), row );
		mPlayerRows.push_back( boost::shared_ptr< StartScreenRow >() );
		mPlayerRows.back().reset( row );
		return true;
	}
	else
	{
		return false;
	}
}

void
View::StartScreen::removePlayer( StartScreenRow * inSender )
{
	auto it = std::find_if
	(
		mPlayerRows.begin(), mPlayerRows.end(),
		[ inSender ]( boost::shared_ptr< StartScreenRow > const & inRow ){ return inRow.get() == inSender; }
	);
	mLayout->removeWidget( inSender );
	inSender->deleteLater();
	mPlayerRows.erase( it );
}

void
View::StartScreen::updateColors( StartScreenRow * inSender )
{
	auto it = std::find_if
	(
		mPlayerRows.begin(), mPlayerRows.end(),
		[ inSender ]( boost::shared_ptr< StartScreenRow > const & inRow ){ return inRow.get() == inSender; }
	);

	for ( auto otherRow : mPlayerRows )
	{
		if ( *it != otherRow && otherRow->getColor() == (*it)->getColor() )
		{
			otherRow->setColor( findUnusedColor() );
		}
	}
}

void
View::StartScreen::randomizePlayerOrder()
{
	std::vector< PlayerInfo > players = getPlayers();
	Utils::RandomShuffle( players );
	for ( std::size_t i = 0; i < players.size(); ++i )
	{
		mPlayerRows[ i ]->setName( players[ i ].name );
		mPlayerRows[ i ]->setColor( players[ i ].color );
		mPlayerRows[ i ]->setAI( players[ i ].isAI );
	}
}

void
View::StartScreen::playClicked()
{
	startGame( getSelectedExpansions(), getPlayers() );
}
