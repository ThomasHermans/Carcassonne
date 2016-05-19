#include <test_o_matic.hpp>

#include "Model/ModelPlayer.h"

#include "Utils/Typedefs.h"

using namespace Model;

namespace
{
	std::set< Utils::Expansion::Type >
	getExpansions()
	{
		std::set< Utils::Expansion::Type > expansions;
		expansions.insert( Utils::Expansion::kBaseGame );
		return expansions;
	}
}

TEST( "Player: constructor, getters and initial values" )
{
	Player const player( "Thomas", Color::kGreen, getExpansions() );
	CHECK( player.getName() == "Thomas" );
	CHECK( player.getColor() == Color::kGreen );
	CHECK( player.getScore() == 0 );
	CHECK( player.getNumberOfFreePieces( Piece::kFollower ) == 6 );
}

TEST( "Player: get and return a piece and check free pieces" )
{
	Player player( "Thomas", Color::kGreen, getExpansions() );
	boost::optional< Piece > const firstPiece = player.getPieceToPlace( Piece::kFollower );
	boost::optional< Piece > const secondPiece = player.getPieceToPlace( Piece::kFollower );
	assert( firstPiece );
	assert( secondPiece );
	CHECK( player.getNumberOfFreePieces( Piece::kFollower ) == 4 );
	player.returnPiece( *firstPiece );
	CHECK( player.getNumberOfFreePieces( Piece::kFollower ) == 5 );
}

TEST( "Player: award points" )
{
	Player player( "Thomas", Color::kGreen, getExpansions() );
	player.awardPoints( 5 );
	player.awardPoints( 4 );
	player.awardPoints( 19 );
	CHECK( player.getScore() == 28 );
}