#ifndef CARCASSONNE_CONTROLLER_MODERATORCONTROLLER_20160407
#define CARCASSONNE_CONTROLLER_MODERATORCONTROLLER_20160407

#include "Model/Piece.h"

#include "Utils/Typedefs.h"

#include <QObject>

#include <boost/optional.hpp>

#include <memory>
#include <set>
#include <vector>

namespace Model
{
	class PlacedPiece;
	class TileOnBoard;
}

namespace View
{
	class GameWindow;
	struct PlayerInfo;
}

namespace Utils
{
	struct Location;
}

namespace Controller
{
	class Moderator;
	class Player;

	class ModeratorController : public QObject
	{
		Q_OBJECT

	signals:
		void gameStarted();

	public:
		ModeratorController
		(
			std::vector< View::PlayerInfo > const & inPlayers,
			std::set< Utils::Expansion::Type > const & inExpansions
		);
		~ModeratorController();

		void startGame();

	private slots:
		void onTilePlaced( Utils::Location const & inLocation, Model::TileOnBoard const & inTile, boost::optional< Utils::PlayerID > const & inPlayer );
		void onTilesLeftChanged( unsigned inTilesLeft );
		void onNextTileChanged( std::string const & inNextTile );

		void onPiecePlaced( Utils::Location const & inLocation, Model::PlacedPiece const & inPiece );
		void onPieceRemoved( Utils::Location const & inLocation, Model::PlacedPiece const & inPiece );

		void onCurrentPlayerChanged( std::string const & inName );

		void onPlayerScoreChanged( Utils::PlayerID inPlayer, std::size_t inScore );
		void onPlayerSupplyChanged( Utils::PlayerID inPlayer, Model::Piece::PieceType inPieceType, std::size_t inSupply );

	private:
		std::shared_ptr< View::GameWindow > mGameWindow;
		Moderator * mModerator;
	};
}

#endif