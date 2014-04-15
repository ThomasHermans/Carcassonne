#ifndef PLAYER_H
#define PLAYER_H

#include "Model/Color.h"
#include "Model/Piece.h"

#include <boost/shared_ptr.hpp>
#include <boost/signals2/signal.hpp>

#include <vector>

namespace Model
{
	class NewPlayer
	{
	public:
		/**
		 *	Construct a new player with the given name and color.
		 */
		NewPlayer( std::string const & inName, Color::Color inColor );

		/**
		 *	Get the name of this player.
		 */
		std::string const & getName() const;

		/**
		 *	Get the color of this player.
		 */
		Color::Color getColor() const;

		/**
		 *	Get the score of this player.
		 */
		std::size_t getScore() const;

		/**
		 *	Get the number of free pieces this player has.
		 */
		std::size_t getNumberOfFreePieces() const;

		/**
		 *	Does this player have free pieces?
		 */
		bool hasFreePieces() const;

		/**
		 *	Remove one of the free pieces so you can place it.
		 *	@note this player should have free pieces.
		 */
		Piece getPieceToPlace();

		/**
		 *	Return a piece to this player.
		 */
		void returnPiece( Piece const & inPiece );

		/**
		 *	Award some points to this player.
		 */
		void awardPoints( std::size_t inPoints );

		/**
		 *	Something for this player has changed.
		 */
		boost::signals2::signal< void () > &
		GetInfoChangedSignal();

	private:
		std::string mName;
		Color::Color mColor;
		std::size_t mScore;
		std::vector< Piece > mFreePieces;
		boost::shared_ptr< boost::signals2::signal< void () > > mInfoChanged;
	};
}

#endif // PLAYER_H
