#ifndef ADTO_TARGIL1_SCISSORS_H
#define ADTO_TARGIL1_SCISSORS_H

//--------------------------
// Scissors Interface
//--------------------------

// Creates and handles all instances of piece of type scissors. 

// @author Hila Tsfadia, Jael Fafner TAU - Advanced Topics in Programming - 2018 Semester B

#include "RPSPiece.h"

#define SCISSORS_CHAR 'S'

class Scissors :
	public RPSPiece
{
protected:
	// Scissors implements it according to its own rules.
	// When a piece wants to move to an occupied location by a piece of other type,
	// Deletes and removes the loser from its owner.
	// Returns the winning piece. If there is no winner, returns nullptr.
	virtual WinningPiece FightWithOtherPieceType(Piece& enemy) override;
public:
	Scissors(shared_ptr<Player> owner = nullptr) : RPSPiece(owner) {}
	Scissors(int ownerNum) : RPSPiece(ownerNum) {}

	// Gets this piece type.
	PieceFactory::PieceType GetPieceType() const override { return PieceFactory::PieceType::Scissors; }

	char GetPieceChar() const override { return SCISSORS_CHAR; }

	virtual bool IsStrongerThan(const Piece& other) const override;
};

#endif //ADTO_TARGIL1_SCISSORS_H
