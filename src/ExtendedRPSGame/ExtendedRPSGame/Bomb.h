#ifndef ADTO_TARGIL1_BOMB_H
#define ADTO_TARGIL1_BOMB_H

#include "Piece.h"

#define BOMB_CHAR 'B'

class Bomb :
	public Piece
{
protected:
	
	// Bomb implements it according to its own rules.
	// When a piece wants to move to an occupied location by a piece of other type,
	// Deletes and removes the loser from its owner.
	// Returns the winning piece. If there is no winner, returns nullptr.
	virtual Piece* FightWithOtherPieceType(Piece* enemy) override;
public:
	Bomb(Player* owner = nullptr) : Piece(owner) {}

	// Gets this piece type.
	PieceFactory::PieceType GetPieceType() const override { return PieceFactory::PieceType::Bomb; }

	bool isMovingPiece() const override { return false; }

	char GetPieceChar() const override { return BOMB_CHAR; }
};

#endif //ADTO_TARGIL1_BOMB_H
