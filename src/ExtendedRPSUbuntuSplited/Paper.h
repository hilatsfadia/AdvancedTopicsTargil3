#ifndef ADTO_TARGIL1_PAPER_H
#define ADTO_TARGIL1_PAPER_H

//--------------------------
// Paper Interface
//--------------------------

// Creates and handles all instances of piece of type paper. 

// @author Hila Tsfadia, Jael Fafner TAU - Advanced Topics in Programming - 2018 Semester B

#include "RPSPiece.h"

#define PAPER_CHAR 'P'

class Paper :
	public RPSPiece
{
protected:
	// Paper implements it according to its own rules.
	// When a piece wants to move to an occupied location by a piece of other type,
	// Removes the loser from its owner.
	// Returns the winner.
	virtual WinningPiece FightWithOtherPieceType(Piece& enemy) override;
public:
	Paper(std::shared_ptr<Player> owner = nullptr) : RPSPiece(owner) {}
	Paper(int ownerNum) : RPSPiece(ownerNum) {}

	// Gets this piece type.
	PieceType GetPieceType() const override { return PieceType::Paper; }

	char GetPieceChar() const override { return PAPER_CHAR; }

	virtual bool IsStrongerThan(const Piece& other) const override;
};

#endif //ADTO_TARGIL1_PAPER_H
