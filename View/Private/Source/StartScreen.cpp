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
	std::set< Color > colors;
	colors.insert( kRed );
	colors.insert( kGreen );
	colors.insert( kBlue );
	colors.insert( kYellow );
	colors.insert( kBlack );
	colors.insert( kGray );
	for ( std::vector< boost::shared_ptr< StartScreenRow > >::const_iterator it = mPlayerRows.begin();
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
	if ( mPlayerRows.size() < kMaxPlayers )
	{
		StartScreenRow * row = new StartScreenRow( this );
		Color const color = findUnusedColor();
		row->setColor( color );
		connect( row, SIGNAL(removed()), this, SLOT(removePlayer()) );
		connect( row, SIGNAL(colorChanged(Color)), this, SLOT(updateColors(Color)) );
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
View::StartScreen::removePlayer()
{
	StartScreenRow * senderRow = qobject_cast< StartScreenRow * >( QObject::sender() );
	if ( senderRow )
	{
		for ( std::vector< boost::shared_ptr< StartScreenRow > >::iterator it = mPlayerRows.begin();
			it != mPlayerRows.end();
			++it )
		{
			if ( senderRow == it->get() )
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
		for ( std::vector< boost::shared_ptr< StartScreenRow > >::iterator it = mPlayerRows.begin();
			it != mPlayerRows.end();
			++it )
		{
			if ( senderRow != it->get() && (*it)->getColor() == inColor )
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
	for ( boost::shared_ptr< StartScreenRow > const & row : mPlayerRows )
	{
		players.push_back( PlayerInfo( row->getName().toStdString(), row->getColor(), row->isAI() ) );
	}
	return players;
}
