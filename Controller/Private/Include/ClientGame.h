#ifndef CARCASSONNE_CONTROLLER_CLIENTGAME
#define CARCASSONNE_CONTROLLER_CLIENTGAME

#include "Model/PlacedPiece.h"
#include "Model/Player.h"
#include "Model/Rotation.h"

#include "Utils/Typedefs.h"

#include <QAbstractSocket>
#include <QObject>

#include <boost/signals2/signal.hpp>

#include <string>

QT_BEGIN_NAMESPACE
	class QNetworkSession;
	class QTcpSocket;
QT_END_NAMESPACE

namespace Controller
{
	class ClientGame : public QObject
	{
		Q_OBJECT
	public:
		/**
		 *	There is a new current player.
		 */
		boost::signals2::signal< void ( Model::Player const & inPlayer ) > currentPlayerChanged;
		/**
		 *	The information about a player has changed.
		 */
		boost::signals2::signal< void ( Model::Player const & inPlayer ) > playerInfoChanged;
		/**
		 *	A tile was placed on the specified location.
		 */
		boost::signals2::signal< void ( Utils::Location const & inLocation, std::string const & inTileID, Model::Rotation inRotation ) > tilePlaced;
		/**
		 *	A tile was removed from the specified location.
		 */
		boost::signals2::signal< void ( Utils::Location const & inLocation ) > tileRemoved;
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
		boost::signals2::signal< void ( Utils::Location const & inLocation, Model::PlacedPiece const & inPiece, Model::Player const & inPlayer ) > piecePlaced;
		/**
		 *	A piece has been removed from the board.
		 */
		boost::signals2::signal< void ( Utils::Location const & inLocation, Model::PlacedPiece const & inPiece, Model::Player const & inPlayer ) > pieceRemoved;
		/**
		 *	There is a new list of possible locations for the (new) tile.
		 */
		boost::signals2::signal< void ( Utils::Locations inNewLocations ) > possibleLocationsChanged;
		/**
		 *	The game has ended. The number of tiles left in the bag is passed.
		 */
		boost::signals2::signal< void ( std::size_t ) > endOfGame;
	public:
		ClientGame();

	private slots:
		void readGame();
		void handleError( QAbstractSocket::SocketError const & inError );

	private:
		QTcpSocket * mTcpSocket;
		quint16 mBlockSize;
		QNetworkSession * mNetworkSession;
	};
}

#endif