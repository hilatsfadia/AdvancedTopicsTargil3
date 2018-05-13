#ifndef ADTO_TARGIL1_JOKER_PIECE_H
#define ADTO_TARGIL1_JOKER_PIECE_H

#include "Piece.h"

#define JOKER_CHAR 'J'

class Joker :
	public Piece
{
protected:
	Piece* mActualPiece = nullptr;

	// When a joker wants to move to an occupied location by other joker.
	// Make a fight between the jokers actual representation.
	// Deletes and removes the loser and it's actual representation from their owner.
	// Returns the winning piece. If there is no winner, returns nullptr.
	virtual Piece* FightPieceOfTheSameType(Piece* enemy) override;

	// Joker implements it according to its own rules.
	// When a piece wants to move to an occupied location by a piece of other type,
	// Deletes and removes the loser from its owner.
	// Returns the winning piece. If there is no winner, returns nullptr.
	virtual Piece* FightWithOtherPieceType(Piece* enemy) override;
public:
	Joker(Player* owner = nullptr) : Piece(owner) {}
	Joker(int ownerNum) : Piece(ownerNum) {}

	// Gets this piece type.
	PieceFactory::PieceType GetPieceType() const override { return PieceFactory::PieceType::Joker; }

	// Return true if the given new piece has the same owner as this joker,
	// and it represents a piece the Joker wants to be, from: R P S B
	// If so, delete old actual piece and replace it by the given new piece.
	bool SetActualPieceType(Piece* newPiece);

	bool isMovingPiece() const override { return mActualPiece->isMovingPiece(); }

	char GetPieceChar() const override { return JOKER_CHAR; }
};

#endif //ADTO_TARGIL1_JOKER_PIECE_H
