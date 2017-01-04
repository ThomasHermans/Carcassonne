#ifndef CARCASSONNE_CONTROLLER_ROBOTPLAYER_20160406
#define CARCASSONNE_CONTROLLER_ROBOTPLAYER_20160406

#include "Controller/Player.h"

#include "Model/Board.h"
#include "Model/Tile.h"

#include <boost/optional.hpp>

#include <memory>

namespace Controller
{
	class RobotPlayer final : public Player
	{
	public:
		RobotPlayer
		(
			std::string const & inName,
			Model::Color::Color inColor,
			std::map< Model::Piece::PieceType, std::size_t > const & inMeepleSupply
		);
		~RobotPlayer() override;

		void setNumberOfPlayers( std::size_t inNumberOfPlayers ) override;

		void
		placeTile( GameState const & inGameState, Model::Tile const & inTileToPlace ) override;

		void
		placePiece( GameState const & inGameState, Utils::Location const & inPlacedTile ) override;

	private:
		void sendTilePlaced();
		void sendPiecePlaced();

		void doPlaceTile();

		void decideTileAndPiecePlacement();

	private:
		std::size_t mNumberOfPlayers;
		boost::optional< GameState > mCurrentGameState;
		boost::optional< Model::Tile > mTileToPlace;
		boost::optional< TilePlacement > mTilePlacement;
		boost::optional< Model::PlacedPiece > mPiecePlacement;
	};
}

#endif