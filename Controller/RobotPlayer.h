#ifndef CARCASSONNE_CONTROLLER_ROBOTPLAYER_20160406
#define CARCASSONNE_CONTROLLER_ROBOTPLAYER_20160406

#include "Controller/Player.h"

#include "Model/Board.h"
#include "Model/Tile.h"

#include <boost/optional.hpp>

#include <QTimer>

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

		void
		placeTile
		(
			Model::Board const & inCurrentBoard,
			Model::Tile const & inTileToPlace
		) override;

		void
		placePiece
		(
			Model::Board const & inCurrentBoard,
			Utils::Location const & inLocation
		) override;

	private:
		void sendTilePlaced();
		void sendPiecePlaced();

		void decideTileAndPiecePlacement();

	private:
		QTimer * mPlaceTileTimer;
		QTimer * mPlacePieceTimer;
		boost::optional< Model::Board > mCurrentBoard;
		boost::optional< Model::Tile > mTileToPlace;
		boost::optional< TilePlacement > mTilePlacement;
		boost::optional< Model::PlacedPiece > mPiecePlacement;
	};
}

#endif