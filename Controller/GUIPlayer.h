#ifndef CARCASSONNE_CONTROLLER_GUIPLAYER_20160406
#define CARCASSONNE_CONTROLLER_GUIPLAYER_20160406

#include "Controller/Player.h"

#include "View/Typedefs.h"

#include <boost/signals2/connection.hpp>

#include <vector>

namespace View
{
	class Meeple;
}

namespace Controller
{
	class GUIPlayer final : public Player
	{
	public:
		GUIPlayer
		(
			std::string const & inName,
			Model::Color::Color inColor,
			std::map< Model::Piece::PieceType, std::size_t > const & inMeepleSupply
		);
		~GUIPlayer() override;

		void setWindow( std::shared_ptr< View::GameWindow > inWindow ) override;

		void
		placeTile( GameState const & inGameState, Model::Tile const & inTile ) override;

		void
		placePiece( GameState const & inGameState, Utils::Location const & inPlacedTile ) override;

	private:
		void
		onTilePlaced
		(
			Utils::Location const & inLocation,
			std::string const & inTileId,
			View::Rotation inRotation
		);

		void
		disconnectAllPlacePieceConnections();

	private:
		std::shared_ptr< View::GameWindow > mWindow;
		std::vector< boost::signals2::connection > mPlacePieceConnections;
	};
}

#endif