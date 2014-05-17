#ifndef PLAYER_H
#define PLAYER_H

#include "Model/Color.h"
#include "Model/Piece.h"

#include "Utils/Typedefs.h"

#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/signals2/signal.hpp>

#include <map>
#include <set>
#include <string>

namespace Model
{
	class Player
	{
	public:
		/**
		 *	Construct a new player with the given name and color.
		 *	The player will have the correct pieces according to
		 *	the specified expansions.
		 */
		Player
		(
			std::string const & inName,
			Color::Color inColor,
			std::set< Utils::Expansion::Type > const & inExpansions
		);

		/**
		 *	Construct a new player with the given name, color, score
		 *	and free pieces.
		 */
		Player
		(
			std::string const & inName,
			Color::Color inColor,
			std::size_t inScore,
			std::map< Piece::PieceType, std::size_t > const & inPieces
		);

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
		std::size_t getNumberOfFreePieces( Piece::PieceType inType ) const;

		/**
		 *	Does this player has a piece of the specified type
		 *	to place?
		 */
		bool hasPieceToPlace( Piece::PieceType inType ) const;

		/**
		 *	Remove one of the free pieces so you can place it.
		 *	If this player does not have a free piece of the
		 *	specified type, boost::none is returned.
		 */
		boost::optional< Piece > getPieceToPlace( Piece::PieceType inType );

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

		/**
		 *	Get all the free pieces this player has.
		 */
		std::map< Piece::PieceType, std::size_t > const &
		getPieces() const;

	private:
		std::string mName;
		Color::Color mColor;
		std::size_t mScore;
		std::map< Piece::PieceType, std::size_t > mPieces;
		boost::shared_ptr< boost::signals2::signal< void () > > mInfoChanged;
	};
}

#endif // PLAYER_H
