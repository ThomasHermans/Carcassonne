#ifndef CARCASSONNE_CONTROLLER_ROBOTPLAYER_20160406
#define CARCASSONNE_CONTROLLER_ROBOTPLAYER_20160406

#include "Controller/Player.h"

#include "Model/Board.h"
#include "Model/Tile.h"

#include <boost/optional.hpp>

#include <QTimer>

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
		placeTile
		(
			Model::Board const & inCurrentBoard,
			std::size_t inTilesLeft,
			Model::Tile const & inTileToPlace
		) override;

		void
		placePiece
		(
			Model::Board const & inCurrentBoard,
			std::size_t inTilesLeft,
			Utils::Location const & inLocation
		) override;

	private:
		void sendTilePlaced();
		void sendPiecePlaced();

		void decideTileAndPiecePlacement();

	private:
		std::unique_ptr< QTimer > mPlaceTileTimer;
		std::unique_ptr< QTimer > mPlacePieceTimer;
		std::size_t mNumberOfPlayers;
		boost::optional< Model::Board > mCurrentBoard;
		boost::optional< std::size_t > mTilesLeft;
		boost::optional< Model::Tile > mTileToPlace;
		boost::optional< TilePlacement > mTilePlacement;
		boost::optional< Model::PlacedPiece > mPiecePlacement;
	};
}

#endif