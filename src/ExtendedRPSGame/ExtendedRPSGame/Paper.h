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
	// Deletes and removes the loser from its owner.
	// Returns the winning piece. If there is no winner, returns nullptr.
	virtual WinningPiece FightWithOtherPieceType(Piece& enemy) override;
public:
	Paper(std::shared_ptr<Player> owner = nullptr) : RPSPiece(owner) {}
	Paper(int ownerNum) : RPSPiece(ownerNum) {}

	// Gets this piece type.
	PieceFactory::PieceType GetPieceType() const override { return PieceFactory::PieceType::Paper; }

	bool GetIsMovingPiece() const override { return true; }

	char GetPieceChar() const override { return PAPER_CHAR; }

	virtual bool IsStrongerThan(const Piece& other) const override;
};

#endif //ADTO_TARGIL1_PAPER_H
