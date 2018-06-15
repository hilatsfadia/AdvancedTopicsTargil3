#ifndef ADTO_TARGIL1_BOMB_H
#define ADTO_TARGIL1_BOMB_H

//--------------------------
// Bomb Class
//--------------------------

// Creates and handles all instances of piece of type bomb. Inherits type piece.

// @author Hila Tsfadia, Jael Fafner TAU - Advanced Topics in Programming - 2018 Semester B

#include "Piece.h"

namespace HilaAndJaelExtendedRPS
{
	#define BOMB_CHAR 'B'

	class Bomb :
		public Piece
	{
	protected:

		// Bomb implements it according to its own rules.
		// When a piece wants to move to an occupied location by a piece of other type,
		// Removes the loser from its owner.
		// Returns the winner.
		virtual WinningPiece FightWithOtherPieceType(Piece& enemy) override;
	public:
		Bomb(shared_ptr<Player> owner = nullptr) : Piece(owner) {}
		Bomb(int ownerNum) : Piece(ownerNum) {}

		// Gets this piece type.
		PieceType GetPieceType() const override { return PieceType::Bomb; }

		bool GetIsMovingPiece() const override { return false; }

		char GetPieceChar() const override { return BOMB_CHAR; }
	};
}
#endif //ADTO_TARGIL1_BOMB_H
