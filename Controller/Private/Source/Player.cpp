#include "Controller/Player.h"

Utils::PlayerID const Controller::kInvalidID = 0;

namespace
{
	Utils::PlayerID
	getNextID()
	{
		static Utils::PlayerID sCurrentID = Controller::kInvalidID;
		return ++sCurrentID;
	}
}

Controller::TilePlacement::TilePlacement
(
	Utils::Location const & inLocation,
	Model::Rotation const & inRotation
)
:
	location( inLocation ),
	rotation( inRotation )
{}

Controller::Player::Player
(
	std::string const & inName,
	Model::Color::Color inColor,
	std::map< Model::Piece::PieceType, std::size_t > const & inMeepleSupply
)
:
	mName( inName ),
	mPlayerID( getNextID() ),
	mColor( inColor ),
	mScore( 0 ),
	mMeepleSupply( inMeepleSupply )
{
}

Controller::Player::~Player()
{}

std::string const &
Controller::Player::getName() const
{
	return mName;
}

Utils::PlayerID
Controller::Player::getID() const
{
	return mPlayerID;
}

Model::Color::Color
Controller::Player::getColor() const
{
	return mColor;
}

std::size_t
Controller::Player::getScore() const
{
	return mScore;
}

void
Controller::Player::awardPoints( std::size_t inPoints )
{
	mScore += inPoints;
	scoreChanged( mScore );
}

bool
Controller::Player::hasPiece( Model::Piece::PieceType inPieceType ) const
{
	return getSupply( inPieceType ) > 0;
}

std::size_t
Controller::Player::getSupply( Model::Piece::PieceType inPieceType ) const
{
	auto const it = mMeepleSupply.find( inPieceType );
	return it != mMeepleSupply.end() ? it->second : 0;
}

void
Controller::Player::takePiece( Model::Piece::PieceType inPieceType )
{
	assert( hasPiece( inPieceType ) );
	auto const it = mMeepleSupply.find( inPieceType );
	--(it->second);
	meepleSupplyChanged();
}

void
Controller::Player::returnPiece( Model::Piece const & inPiece )
{
	auto it = mMeepleSupply.find( inPiece.getType() );
	if ( it != mMeepleSupply.end() )
	{
		++(it->second);
	}
	else
	{
		mMeepleSupply.emplace( inPiece.getType(), 0 );
	}
	meepleSupplyChanged();
}

void
Controller::Player::setWindow( std::shared_ptr< View::GameWindow > /*inWindow*/ )
{
}

void
Controller::Player::setNumberOfPlayers( std::size_t /*inNumberOfPlayers*/ )
{
}