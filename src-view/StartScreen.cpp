#include "src-view/StartScreen.h"

#include "src-view/StartScreenRow.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include <cassert>
#include <set>

StartScreen::StartScreen( QWidget * inParent )
:
	QDialog( inParent ),
	mLayout( new QVBoxLayout() ),
	mPlayerRows(),
	mAddPlayerButton( new QPushButton( "Add player", this ) ),
	mPlayButton( new QPushButton( "Start game", this ) )
{
	connect( mAddPlayerButton, SIGNAL( clicked() ), this, SLOT( addPlayer() ) );

	setLayout( mLayout );

	addPlayer();

	mLayout->addWidget( mAddPlayerButton );
	mLayout->addWidget( mPlayButton );
	mLayout->addStretch();
}

StartScreen::~StartScreen()
{
}

Gui::Color
StartScreen::findUnusedColor() const
{
	std::set< Gui::Color > colors;
	colors.insert( Gui::kRed );
	colors.insert( Gui::kGreen );
	colors.insert( Gui::kBlue );
	colors.insert( Gui::kYellow );
	colors.insert( Gui::kBlack );
	colors.insert( Gui::kGray );
	for ( std::vector< StartScreenRow * >::const_iterator it = mPlayerRows.begin();
		it != mPlayerRows.end();
		++it )
	{
		colors.erase( (*it)->getColor() );
	}
	assert( !colors.empty() );
	return *colors.begin();
}

void
StartScreen::addPlayer()
{
	if ( mPlayerRows.size() < 6 )
	{
		StartScreenRow * row = new StartScreenRow( this );
		Gui::Color color = findUnusedColor();
		row->setColor( color );
		connect( row, SIGNAL( removed() ), this, SLOT( removePlayer() ) );
		connect( row, SIGNAL( colorChanged( Gui::Color ) ), this, SLOT( updateColors( Gui::Color ) ) );
		mLayout->insertWidget( mPlayerRows.size(), row );
		mPlayerRows.push_back( row );
	}
}

void
StartScreen::removePlayer()
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
StartScreen::updateColors( Gui::Color inColor )
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
				Gui::Color color = findUnusedColor();
				(*it)->setColor( color );
			}
		}
	}
}