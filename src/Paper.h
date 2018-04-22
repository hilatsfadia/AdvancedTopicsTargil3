#ifndef ADTO_TARGIL1_PAPER_H
#define ADTO_TARGIL1_PAPER_H

#include "Piece.h"
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
	Paper(char pieceChar, Player* owner = nullptr) : Piece(pieceChar, owner) {}

	// Gets this piece type.
	PieceFactory::PieceType GetPieceType() const override { return PieceFactory::PieceType::Paper; }

	bool isMovingPiece() const override { return true; }
};

#endif //ADTO_TARGIL1_PAPER_H
