#ifndef CARCASSONNE_MODEL_NEWBOARD_20140313
#define CARCASSONNE_MODEL_NEWBOARD_20140313

#include "Model/PlacedProject.h"
#include "Model/TileOnBoard.h"

#include "Utils/Location.h"
#include "Utils/Typedefs.h"

#ifndef Q_MOC_RUN
#include <boost/optional.hpp>
#include <boost/signals2/signal.hpp>
#endif

#include <vector>

namespace Model
{
	class PlacedPiece;

	/**
	 *	A Board holds the information about which TileOnBoards
	 *	are located where. For the public API, the start tile is
	 *	always located on (0,0).
	 */
	class Board
	{
	public:
		boost::signals2::signal< void ( Utils::Location, TileOnBoard ) > & getTilePlacedSignal();
		boost::signals2::signal< void ( Utils::Location, PlacedPiece ) > & getPiecePlacedSignal();
		boost::signals2::signal< void ( Utils::Location, PlacedPiece ) > & getPieceRemovedSignal();
		
	public:
		Board();
		Board( Board const & inBoard );

		/**
		 *	Count the total amount of tiles on the board.
		 */
		std::size_t getNrOfTiles() const;

		/**
		 *	Get the top most row that contains a tile.
		 */
		int getTopRow() const;

		/**
		 *	Get the bottom most row that contains a tile.
		 */
		int getBottomRow() const;

		/**
		 *	Get the left most column that contains a tile.
		 */
		int getLeftCol() const;

		/**
		 *	Get the right most column that contains a tile.
		 */
		int getRightCol() const;

		/**
		 *	Is there a tile at the specified position?
		 */
		bool isTile( int inRow, int inCol ) const;
		bool isTile( Utils::Location const & inLocation ) const;

		/**
		 *	Get a copy of the tile at the specified position,
		 *	or boost::none if the tile does not exist yet.
		 */
		boost::optional< TileOnBoard > getTile( int inRow, int inCol ) const;
		boost::optional< TileOnBoard > getTile( Utils::Location const & inLocation ) const;

		/**
		 *	Get a reference to the tile at the specified position.
		 */
		boost::optional< TileOnBoard > & getTile( int inRow, int inCol );
		boost::optional< TileOnBoard > & getTile( Utils::Location const & inLocation );

		bool isCity( PlacedCity const & inCity ) const;
		bool isCity( Utils::Location const & inLocation, Area::Area inArea ) const;

		bool isRoad( PlacedRoad const & inRoad ) const;
		bool isRoad( Utils::Location const & inLocation, Area::Area inArea ) const;

		bool isField( PlacedField const & inField ) const;
		bool isField( Utils::Location const & inLocation, Area::Area inArea ) const;

		bool isCloister( Utils::Location const & inLocation, Area::Area inArea ) const;

		/**
		 *	Place a start tile on the board on position (0,0).
		 */
		bool placeStartTile( TileOnBoard const & inTile );

		/**
		 *	Can the specified tile validly be placed at the specified location?
		 */
		bool isValidTilePlacement( TileOnBoard const & inTile, int inRow, int inCol ) const;
		bool isValidTilePlacement( TileOnBoard const & inTile, Utils::Location const & inLocation ) const;

		/**
		 *	Place a valid tile on the board.
		 */
		bool placeValidTile( TileOnBoard const & inTile, int inRow, int inCol );
		bool placeValidTile( TileOnBoard const & inTile, Utils::Location const & inLocation );

		/**
		 *	Place a valid piece on the board.
		 */
		bool placeValidPiece( PlacedPiece const & inPiece, Utils::Location const & inLocation );

		/**
		 *	Is it possible to place the provided tile somewhere?
		 */
		bool isPossibleTile( Tile const & inTile ) const;

		/**
		 *	Get all the possible locations for this Tile.
		 */
		Utils::Locations getPossibleLocations( Tile const & inTile ) const;

		/**
		 *	Can this piece validly be placed at this location?
		 */
		bool isValidPiecePlacement( Utils::Location const & inLocation, PlacedPiece const & inPiece ) const;

		/**
		 *	Check whether the specified road is occupied.
		 *	Returns false when the specified tile does not exist,
		 *	the specified area is not a road, or the road is not
		 *	occupied.
		 */
		bool isOccupiedRoad( PlacedRoad const & inRoad ) const;
		bool isOccupiedRoad( Utils::Location const & inLocation, Area::Area inArea ) const;

		/**
		 *	Check whether the specified city is occupied.
		 *	Returns false when the specified tile does not exist,
		 *	the specified area is not a city, or the city is not
		 *	occupied.
		 */
		bool isOccupiedCity( PlacedCity const & inCity ) const;
		bool isOccupiedCity( Utils::Location const & inLocation, Area::Area inArea ) const;

		/**
		 *	Check whether the specified field is occupied.
		 *	Returns false when the specified tile does not exist,
		 *	the specified area is not a field, or the field is not
		 *	occupied.
		 */
		bool isOccupiedField( PlacedField const & inField ) const;
		bool isOccupiedField( Utils::Location const & inLocation, Area::Area inArea ) const;

		/**
		 *	Get all the pieces from the specified area on that tile.
		 *	The pieces remain on the board.
		 */
		std::vector< PlacedPiece > getPieces( PlacedProject inArea ) const;

		/**
		 *	Remove all the pieces from the specified area on that tile.
		 *	Returns all the removed pieces.
		 */
		std::vector< PlacedPiece > removePieces( PlacedProject inArea );

		/**
		 *	Is the specified city finished?
		 */
		bool isFinishedCity( PlacedCity const & inCity ) const;
		bool isFinishedCity( Utils::Location const & inLocation, Area::Area inArea ) const;

		/**
		 *	Is this road finished?
		 */
		bool isFinishedRoad( PlacedRoad const & inRoad ) const;
		bool isFinishedRoad( Utils::Location const & inLocation, Area::Area inArea ) const;

		/**
		 *	Is this cloister finished?
		 */
		bool isFinishedCloister( Utils::Location const & inLocation ) const;
		bool isFinishedCloister( int inRow, int inCol ) const;

		/**
		 *	Returns the identifier PlacedCity.
		 *	Several NewPlacedCities that are part of the same contiguous city,
		 *	will return the same identifier PlacedCity.
		 */
		PlacedCity getIdentifierCity( PlacedCity const & inCity ) const;

		// Deprecated. To be removed when RobotPlayer does not use it anymore.
		/**
		 *	Returns the number of tiles around the specified tile, including
		 *	the specified tile.
		 *	Returns 0 if there is no tile at the specified location.
		 *	Returns 1 for a tile that has no neighbors (even diagonally).
		 *	Returns 9 for a fully surrounded tile.
		 */
		std::size_t getNrOfSurroundingTiles( int inRow, int inCol ) const;
		std::size_t getNrOfSurroundingTiles( Utils::Location const & inLocation ) const;

		/**
		 *	Get the complete city that is connected with the specified
		 *	city area.
		 */
		std::vector< PlacedCity > getCompleteCity( PlacedCity const & inCity ) const;
		std::vector< PlacedCity > getCompleteCity( Utils::Location const & inLocation, Area::Area inArea ) const;

		/**
		 *	Get the complete road that is connected with the specified
		 *	road area.
		 */
		std::vector< PlacedRoad > getCompleteRoad( PlacedRoad const & inRoad ) const;
		std::vector< PlacedRoad > getCompleteRoad( Utils::Location const & inLocation, Area::Area inArea ) const;

		/**
		 *	Get the complete field that is connected with the specified
		 *	field area.
		 */
		std::vector< PlacedField > getCompleteField( PlacedField const & inField ) const;
		std::vector< PlacedField > getCompleteField( Utils::Location const & inLocation, Area::Area inArea ) const;

		/**
		 *	Does the specified city area have a pennant?
		 */
		bool hasPennant( PlacedCity const & inCity ) const;

		/**
		 *	Does the specified road area have an inn next to it?
		 */
		bool hasInn( PlacedRoad const & inRoad ) const;

		/**
		 *	Does the specified city area have a cathedral in it?
		 */
		bool hasCathedral( PlacedCity const & inCity ) const;

	private:
		int getIndex( int inRow, int inCol ) const;
		void ensureTile( int inRow, int inCol );

		void addRowsTop( int inNrRows );
		void addRowsBottom( int inNrRows );
		void addColsLeft( int inNrCols );
		void addColsRight( int inNrCols );

		bool placeTile( TileOnBoard const & inTile, int inRow, int inCol );

		bool hasNeighbor( int inRow, int inCol ) const;
		bool sidesMatch( TileOnBoard const & inTile, int inRow, int inCol ) const;

		bool isFullySurrounded( int inRow, int inCol ) const;

	private:
		typedef std::vector< boost::optional< TileOnBoard > > Tiles;
		Tiles mTiles;
		int mStartRow;
		int mStartCol;
		int mNrRows;
		int mNrCols;

		std::shared_ptr< boost::signals2::signal< void ( Utils::Location, TileOnBoard ) > > mTilePlacedSignal;
		std::shared_ptr< boost::signals2::signal< void ( Utils::Location, PlacedPiece ) > > mPiecePlacedSignal;
		std::shared_ptr< boost::signals2::signal< void ( Utils::Location, PlacedPiece ) > > mPieceRemovedSignal;
	};
}

#endif