#ifndef CARCASSONNE_MODEL_NEWBOARD_20140313
#define CARCASSONNE_MODEL_NEWBOARD_20140313

#include "TileOnBoard.h"

#include <boost/optional.hpp>

#include <vector>

namespace Model
{
	/**
	 *	A NewBoard holds the information about which TileOnBoards
	 *	are located where. For the public API, the start tile is
	 *	always located on (0,0).
	 */
	class NewBoard
	{
	public:
		NewBoard();

		/**
		 *	Count the total amount of tiles on the board.
		 */
		std::size_t getNrOfTiles() const;

		/**
		 *	Is there a tile at the specified position?
		 */
		bool isTile( int inRow, int inCol ) const;

		/**
		 *	Get a copy of the tile at the specified position,
		 *	or boost::none if the tile does not exist yet.
		 */
		boost::optional< TileOnBoard > getTile( int inRow, int inCol ) const;

		/**
		 *	Get a reference to the tile at the specified position.
		 */
		boost::optional< TileOnBoard > & getTile( int inRow, int inCol );

		/**
		 *	Place a start tile on the board.
		 */
		bool placeStartTile( TileOnBoard const & inTile );

		/**
		 *	Can the specified tile validly be placed at the specified location?
		 */
		bool isValidTilePlacement( TileOnBoard const & inTile, int inRow, int inCol ) const;

	private:
		std::size_t getIndex( int inRow, int inCol ) const;
		void ensureTile( int inRow, int inCol );

		void addRowsTop( int inNrRows );
		void addRowsBottom( int inNrRows );
		void addColsLeft( int inNrCols );
		void addColsRight( int inNrCols );

		bool placeTile( TileOnBoard const & inTile, int inRow, int inCol );

	private:
		typedef std::vector< boost::optional< TileOnBoard > > Tiles;
		Tiles mTiles;
		int mStartRow;
		int mStartCol;
		int mNrRows;
		int mNrCols;
	};
}

#endif