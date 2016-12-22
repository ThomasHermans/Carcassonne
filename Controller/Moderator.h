#ifndef CARCASSONNE_CONTROLLER_MODERATOR_20160324
#define CARCASSONNE_CONTROLLER_MODERATOR_20160324

#include "Model/Board.h"
#include "Model/PlacedPiece.h"

#include "Utils/Typedefs.h"

#ifndef Q_MOC_RUN
#include <boost/optional.hpp>
#include <boost/signals2/signal.hpp>
#endif

#include <QObject>

#include <memory>
#include <set>
#include <vector>

namespace Model
{
	class Tile;
}

namespace Utils
{
	struct Location;
}

namespace View
{
	class GameWindow;
	struct PlayerInfo;
}

namespace Controller
{
	class Player;
	struct TilePlacement;

	class Moderator : public QObject
	{
		Q_OBJECT

	public:
		Moderator
		(
			std::vector< View::PlayerInfo > const & inPlayers,
			std::set< Utils::Expansion::Type > const & inExpansions,
			std::shared_ptr< View::GameWindow > inGameWindow,
			QObject * inParent
		);

		void playGame();

		Model::Board const &
		getBoard() const;

		std::vector< std::shared_ptr< Player > >
		getGameWinners();

		std::string
		getPlayerName( Utils::PlayerID inPlayer ) const;

		Model::Color::Color
		getPlayerColor( Utils::PlayerID inPlayer ) const;

	signals:
		void tilePlaced( Utils::Location const & inLocation, Model::TileOnBoard const & inTile, boost::optional< Utils::PlayerID > const & inPlayer );
		void piecePlaced( Utils::Location const & inLocation, Model::PlacedPiece const & inPiece );
		void pieceRemoved( Utils::Location const & inLocation, Model::PlacedPiece const & inPiece );
		void tilesLeftChanged( unsigned inTilesLeft );
		void nextTileChanged( std::string const & inID );
		void currentPlayerChanged( std::string const & inName );
		void playerScoreChanged( Utils::PlayerID inPlayer, std::size_t inScore );
		void playerSupplyChanged( Utils::PlayerID inPlayer, Model::Piece::PieceType inPiece, std::size_t inScore );

	private:
		void pickNextTile();
		void switchToNextPlayer();

		void prepareNewTurn();
		void requestTilePlacement();
		void onTilePlaced
		(
			boost::signals2::connection const & inConnection,
			TilePlacement const & inTilePlacement
		);
		void requestPiecePlacement( Utils::Location const & inLocation );
		void onPiecePlaced
		(
			boost::signals2::connection const & inConnection,
			Utils::Location const & inLocation,
			boost::optional< Model::PlacedPiece > const & inPiecePlacement
		);
		void endTurn( Utils::Location const & inLocation );
		void endGame();

		std::shared_ptr< Player >
		getPlayer( Model::Color::Color inColor );
		std::shared_ptr< Player >
		getPlayer( Utils::PlayerID inPlayerID );

		bool
		isValidPlacement
		(
			Model::Tile const & inTile,
			TilePlacement const & inPlacement
		) const;

		bool
		isValidPlacement
		(
			Utils::Location const & inLocation,
			Model::PlacedPiece const & inPiece
		) const;

		void finishProjects( Utils::Location const & inLocation );
		void finishRoads( Utils::Location const & inLocation );
		void finishCities( Utils::Location const & inLocation );
		void finishCloisters( Utils::Location const & inLocation );

		void awardEndPoints();
		void awardEndRoadPoints();
		void awardEndCityPoints();
		void awardEndCloisterPoints();
		void awardEndFarmPoints();

		void returnPieces( std::vector< Model::PlacedPiece > const & inPieces );

		void
		awardPoints
		(
			std::set< Model::Color::Color > const & inColors,
			std::size_t inPoints
		);

		void
		sendTilePlaced
		(
			Utils::Location const & inLocation,
			Model::TileOnBoard const & inTile
		);

		void
		sendPiecePlaced
		(
			Utils::Location const & inLocation,
			Model::PlacedPiece const & inPiece
		);

		void
		sendPieceRemoved
		(
			Utils::Location const & inLocation,
			Model::PlacedPiece const & inPiece
		);

		void
		sendPlayerSupply( Utils::PlayerID inPlayerID );

	private:
		std::vector< std::shared_ptr< Player > > mPlayers;
		std::shared_ptr< Player > mCurrentPlayer;
		Model::Board mBoard;
		std::vector< Model::Tile > mBag;
		boost::optional< Model::Tile > mNextTile;
	};
}

#endif