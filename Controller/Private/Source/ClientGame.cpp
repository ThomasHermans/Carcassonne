#include "ClientGame.h"

#include <QNetworkSession>
#include <QTcpSocket>

namespace
{
	QString const kHostName( "host" );
	int const kHostPort = 456;
}

Controller::ClientGame::ClientGame()
:
	QObject(),
	mTcpSocket( new QTcpSocket( this ) ),
	mBlockSize( 0 ),
	mNetworkSession()
{
	connect( mTcpSocket, SIGNAL(readyRead()), this, SLOT(readGame()) );
	connect( mTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
		this, SLOT(handleError(QAbstractSocket::SocketError)) );

	mBlockSize = 0;
	mTcpSocket->abort();
	mTcpSocket->connectToHost( kHostName, kHostPort );
}

void
Controller::ClientGame::readGame()
{
	QDataStream instream( mTcpSocket );
	instream.setVersion( QDataStream::Qt_4_0 );
	if ( mBlockSize == 0 )
	{
		if ( mTcpSocket->bytesAvailable() < (int)sizeof(quint16) )
		{
			return;
		}
		instream >> mBlockSize;
	}
	if ( mTcpSocket->bytesAvailable() < mBlockSize )
	{
		return;
	}
	QString serializedGame;
	instream >> serializedGame;
	// Decrypt serialized game
	// Send out new game state
}

void
Controller::ClientGame::handleError( QAbstractSocket::SocketError const & inError )
{
	switch ( inError )
	{
		case QAbstractSocket::RemoteHostClosedError:
			break;
		case QAbstractSocket::HostNotFoundError:
			// Warn user about unfound host
			break;
		case QAbstractSocket::ConnectionRefusedError:
			// Warn user about refused connection
			break;
		default:
			// Warn user about other error
			break;
	}
}