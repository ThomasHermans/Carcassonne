#ifndef CARCASSONNE_CONTROLLER_PLAYER_20160328
#define CARCASSONNE_CONTROLLER_PLAYER_20160328

#include "Model/Board.h"
#include "Model/Color.h"
#include "Model/Rotation.h"
#include "Model/PlacedPiece.h"

#include "Utils/Location.h"
#include "Utils/Typedefs.h"

#include <boost/optional.hpp>
#include <boost/signals2/signal.hpp>

#include <QObject>

#include <map>
#include <memory>
#include <string>

namespace Model
{
	class Piece;
	class Tile;
}

namespace View
{
	class GameWindow;
}

namespace Controller
{
	struct TilePlacement
	{
		Utils::Location location;
		Model::Rotation rotation;

		TilePlacement
		(
			Utils::Location const & inLocation,
			Model::Rotation const & inRotation
		);
	};

	struct PlayerInfo
	{
		std::string name;
		Utils::PlayerID id;
		Model::Color::Color color;
		std::size_t score;
	};

	struct GameState
	{
		Model::Board board;
		std::set< Utils::Expansion::Type > expansionsInUse;
		std::size_t tilesLeft;
		std::vector< PlayerInfo > players;
	};

	class Player;

	PlayerInfo createPlayerInfo( Player const & inPlayer );

	extern Utils::PlayerID const kInvalidID;

	class Player
	{
	public:
		Player
		(
			std::string const & inName,
			Model::Color::Color inColor,
			std::map< Model::Piece::PieceType, std::size_t > const & inMeepleSupply
		);
		virtual ~Player();
		Player( Player const & ) = delete;
		Player & operator=( Player const & ) = delete;

		std::string const &
		getName() const;

		Utils::PlayerID
		getID() const;

		Model::Color::Color
		getColor() const;

		std::size_t
		getScore() const;

		void
		awardPoints( std::size_t inPoints );

		bool
		hasPiece( Model::Piece::PieceType inPieceType ) const;

		std::size_t
		getSupply( Model::Piece::PieceType inPieceType ) const;

		void
		takePiece( Model::Piece::PieceType inPieceType );

		void
		returnPiece( Model::Piece const & inPiece );

		boost::signals2::signal< void ( std::size_t ) > scoreChanged;
		boost::signals2::signal< void () > meepleSupplyChanged;
		boost::signals2::signal< void ( TilePlacement ) > tilePlaced;
		boost::signals2::signal< void ( boost::optional< Model::PlacedPiece > ) > piecePlaced;

		virtual
		void setWindow( std::shared_ptr< View::GameWindow > inWindow );

		virtual
		void setNumberOfPlayers( std::size_t inNumberOfPlayers );

		virtual
		void placeTile( GameState const & inGameState, Model::Tile const & inTile ) = 0;

		virtual
		void placePiece( GameState const & inGameState, Utils::Location const & inPlacedTile ) = 0;

	private:
		std::string const mName;
		Utils::PlayerID const mPlayerID;
		Model::Color::Color const mColor;
		std::size_t mScore;
		std::map< Model::Piece::PieceType, std::size_t > mMeepleSupply;
	};
}

#endif