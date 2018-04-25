#ifndef ADTO_TARGIL1_FLAG_H
#define ADTO_TARGIL1_FLAG_H

#include "Piece.h"

class Flag :
	public Piece
{
protected:
	// Flag implements it according to its own rules.
	// When a piece wants to move to an occupied location by a piece of other type,
	// Deletes and removes the loser from its owner.
	// Returns the winning piece. If there is no winner, returns nullptr.
	virtual Piece* FightWithOtherPieceType(Piece* enemy) override;
public:
	Flag(char pieceChar, Player* owner = nullptr) : Piece(pieceChar, owner) {}

	// Gets this piece type.
	PieceFactory::PieceType GetPieceType() const override { return PieceFactory::PieceType::Flag; }

	bool isMovingPiece() const override { return false; }
};

#endif //ADTO_TARGIL1_FLAG_H
