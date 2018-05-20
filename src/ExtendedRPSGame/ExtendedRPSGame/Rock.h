#ifndef ADTO_TARGIL1_ROCK_H
#define ADTO_TARGIL1_ROCK_H

#include "Piece.h"

#define ROCK_CHAR 'R'

class Rock :
	public Piece
{
protected:
	// Rock implements it according to its own rules.
	// When a piece wants to move to an occupied location by a piece of other type,
	// Deletes and removes the loser from its owner.
	// Returns the winning piece. If there is no winner, returns nullptr.
	virtual Piece* FightWithOtherPieceType(Piece* enemy) override;
public:
	Rock(Player* owner = nullptr) : Piece(owner) {}
	Rock(int ownerNum) : Piece(ownerNum) {}

	// Gets this piece type.
	PieceFactory::PieceType GetPieceType() const override { return PieceFactory::PieceType::Rock; }

	bool GetIsMovingPiece() const override { return true; }

	char GetPieceChar() const override { return ROCK_CHAR; }

	virtual bool IsStrongerThan(Piece* other) const override;
};

#endif //ADTO_TARGIL1_ROCK_H
