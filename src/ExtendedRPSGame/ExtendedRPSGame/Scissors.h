#ifndef ADTO_TARGIL1_SCISSORS_H
#define ADTO_TARGIL1_SCISSORS_H

#include "Piece.h"

#define SCISSORS_CHAR 'S'

class Scissors :
	public Piece
{
protected:
	// Scissors implements it according to its own rules.
	// When a piece wants to move to an occupied location by a piece of other type,
	// Deletes and removes the loser from its owner.
	// Returns the winning piece. If there is no winner, returns nullptr.
	virtual Piece* FightWithOtherPieceType(Piece* enemy) override;
public:
	Scissors(Player* owner = nullptr) : Piece(owner) {}

	// Gets this piece type.
	PieceFactory::PieceType GetPieceType() const override { return PieceFactory::PieceType::Scissors; }
	~Scissors();

	bool isMovingPiece() const override { return true; }

	char GetPieceChar() const override { return SCISSORS_CHAR; }
};

#endif //ADTO_TARGIL1_SCISSORS_H
