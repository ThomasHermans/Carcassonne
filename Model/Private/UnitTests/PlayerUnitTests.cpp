#include <test_o_matic.hpp>

#include "Model/Player.h"

using namespace Model;

TEST( "NewPlayer: constructor, getters and initial values" )
{
	NewPlayer const player( "Thomas", Color::kGreen );
	CHECK( player.getName() == "Thomas" );
	CHECK( player.getColor() == Color::kGreen );
	CHECK( player.getScore() == 0 );
	CHECK( player.getNumberOfFreePieces() == 6 );
	CHECK( player.hasFreePieces() );
}

TEST( "NewPlayer: get and return a piece and check free pieces" )
{
	NewPlayer player( "Thomas", Color::kGreen );
	Piece const first = player.getPieceToPlace();
	Piece const second = player.getPieceToPlace();
	CHECK( player.getNumberOfFreePieces() == 4 );
	player.returnPiece( first );
	CHECK( player.getNumberOfFreePieces() == 5 );
}

TEST( "NewPlayer: award points" )
{
	NewPlayer player( "Thomas", Color::kGreen );
	player.awardPoints( 5 );
	player.awardPoints( 4 );
	player.awardPoints( 19 );
	CHECK( player.getScore() == 28 );
}