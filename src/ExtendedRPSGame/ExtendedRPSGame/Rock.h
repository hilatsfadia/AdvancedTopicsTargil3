#ifndef ADTO_TARGIL1_ROCK_H
#define ADTO_TARGIL1_ROCK_H

//--------------------------
// Rock Interface
//--------------------------

// Creates and handles all instances of piece of type rock. 

// @author Hila Tsfadia, Jael Fafner TAU - Advanced Topics in Programming - 2018 Semester B

#include "RPSPiece.h"

#define ROCK_CHAR 'R'

class Rock :
	public RPSPiece
{
protected:
	// Rock implements it according to its own rules.
	// When a piece wants to move to an occupied location by a piece of other type,
	// Deletes and removes the loser from its owner.
	// Returns the winning piece. If there is no winner, returns nullptr.
	virtual WinningPiece FightWithOtherPieceType(Piece& enemy) override;
public:
	Rock(shared_ptr<Player> owner = nullptr) : RPSPiece(owner) {}
	Rock(int ownerNum) : RPSPiece(ownerNum) {}

	// Gets this piece type.
	PieceFactory::PieceType GetPieceType() const override { return PieceFactory::PieceType::Rock; }

	bool GetIsMovingPiece() const override { return true; }

	char GetPieceChar() const override { return ROCK_CHAR; }

	virtual bool IsStrongerThan(const Piece& other) const override;
};

#endif //ADTO_TARGIL1_ROCK_H
