#ifndef CARCASSONNE_MODEL_NEWGAME_20140329
#define CARCASSONNE_MODEL_NEWGAME_20140329

#include "NewBoard.h"
#include "PlacedProject.h"

#include "Model/Player.h"

#include <boost/optional.hpp>
#include <boost/signals2/signal.hpp>

#include <vector>
#include <set>

namespace Model
{
	class NewGame
	{
	public:
		/**
		 *	There is a new current player.
		 */
		boost::signals2::signal< void ( NewPlayer const & inPlayer ) > currentPlayerChanged;
		/**
		 *	The information about a player has changed.
		 */
		boost::signals2::signal< void ( NewPlayer const & inPlayer ) > playerInfoChanged;
		/**
		 *	A tile was placed on the specified location.
		 */
		boost::signals2::signal< void ( int inRow, int inCol, std::string const & inTileID, Rotation inRotation ) > tilePlaced;
		/**
		 *	A tile was removed from the specified location.
		 */
		boost::signals2::signal< void ( int inRow, int inCol ) > tileRemoved;
		/**
		 *	There is a new next tile.
		 */
		boost::signals2::signal< void ( std::string const & inTileID ) > nextTile;
		/**
		 *	The number of tiles left in the bag has changed.
		 */
		boost::signals2::signal< void ( std::size_t ) > tilesLeft;
		/**
		 *	A piece has been placed on the board.
		 */
		boost::signals2::signal< void ( int inRow, int inCol, PlacedPiece const & inPiece, NewPlayer const & inPlayer ) > piecePlaced;
		/**
		 *	A piece has been removed from the board.
		 */
		boost::signals2::signal< void ( int inRow, int inCol, PlacedPiece const & inPiece, NewPlayer const & inPlayer ) > pieceRemoved;
		/**
		 *	The game has ended. The number of tiles left in the bag is passed.
		 */
		boost::signals2::signal< void ( std::size_t ) > endOfGame;
	public:
		/**
		 *	Construct a game with the given players and the
		 *	tiles for the base game.
		 */
		NewGame( std::vector< NewPlayer > const & inPlayers );

		/**
		 *	Construct a game with the given players and tiles.
		 */
		NewGame
		(
			std::vector< NewPlayer > const & inPlayers,
			std::string const & inTiles
		);

		/**
		 *	Place the start tile on the board.
		 */
		void placeStartTileOnBoard();

		/**
		 *	Place the specified tile on the board on the specified
		 *	location and preferably with the given rotation.
		 *	If the proposed placement is valid except for the given
		 *	rotation, this will rotate the tile until it finds a
		 *	completely valid placement.
		 *	If the proposed placement is not valid, this will
		 *	do nothing.
		 */
		void
		tryToPlaceTile
		(
			int inRow,
			int inCol,
			std::string const & inTileID,
			Rotation inRotation
		);

		/**
		 *	Rotate the specified tile clockwise until we find a new
		 *	valid placement.
		 */
		void rotateTile( int inRow, int inCol );

		/**
		 *	Place a piece if the piece placement is valid.
		 */
		void tryToPlacePiece
		(
			NewPlacedProject const & inPlace,
			Piece::PieceType inType,
			Color::Color inColor
		);

		/**
		 *	End the current turn.
		 */
		void endTurn();

		/**
		 *	Award points for the pieces that are still on the board.
		 */
		void awardEndPoints();

	private:
		void initialize();

		void pickNextTile();

		bool isEmptySpot( int inRow, int inCol ) const;
		bool isCurrentSpot( int inRow, int inCol ) const;

		void returnPieces( std::vector< PlacedPiece > const & inPieces, int inRow, int inCol );

		NewPlayer & getPlayer( Color::Color inColor );

		bool isOccupiedOnCurrentTile( Area::Area inArea ) const;

		void awardEndCloisterPoints();
		void awardEndCityPoints();
		void awardEndRoadPoints();
		void awardEndFieldPoints();

		void awardPoints( std::set< Color::Color > const & inWinningColors, std::size_t inPoints );

		void onFinishedCity( std::vector< NewPlacedCity > const & inCity );
		void onFinishedRoad( std::vector< NewPlacedRoad > const & inRoad );
		void onFinishedCloister( int inRow, int inCol );

	private:
		NewBoard mBoard;
		std::vector< Tile > mBag;
		boost::optional< Tile > mNextTile;
		std::vector< NewPlayer > mPlayers;
		std::size_t mCurrentPlayer;
		std::size_t mPiecesPlacedThisTurn;

		struct PlacedTile
		{
			PlacedTile( TileOnBoard const & inTile, int inRow, int inCol )
			: tile( inTile ), row( inRow ), col( inCol ) {}

			TileOnBoard tile;
			int row;
			int col;
		};

		boost::optional< PlacedTile > mCurrentPlacedTile;
	};
}

#endif