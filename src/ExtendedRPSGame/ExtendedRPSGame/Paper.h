#ifndef ADTO_TARGIL1_PAPER_H
#define ADTO_TARGIL1_PAPER_H

#include "Piece.h"

#define PAPER_CHAR 'P'

class Paper :
	public Piece
{
protected:
	// Paper implements it according to its own rules.
	// When a piece wants to move to an occupied location by a piece of other type,
	// Deletes and removes the loser from its owner.
	// Returns the winning piece. If there is no winner, returns nullptr.
	virtual Piece* FightWithOtherPieceType(Piece* enemy) override;
public:
	Paper(Player* owner = nullptr) : Piece(owner) {}
	Paper(int ownerNum) : Piece(ownerNum) {}

	// Gets this piece type.
	PieceFactory::PieceType GetPieceType() const override { return PieceFactory::PieceType::Paper; }

	bool isMovingPiece() const override { return true; }

	char GetPieceChar() const override { return PAPER_CHAR; }
};

#endif //ADTO_TARGIL1_PAPER_H
