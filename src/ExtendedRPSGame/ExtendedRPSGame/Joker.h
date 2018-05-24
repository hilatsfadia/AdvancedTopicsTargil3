#ifndef ADTO_TARGIL1_JOKER_PIECE_H
#define ADTO_TARGIL1_JOKER_PIECE_H

#include "Piece.h"
#include <memory>

#define JOKER_CHAR 'J'

class Joker :
	public Piece
{
protected:
	std::unique_ptr<Piece> mActualPiece = nullptr;

	// When a joker wants to move to an occupied location by other joker.
	// Make a fight between the jokers actual representation.
	// Deletes and removes the loser and it's actual representation from their owner.
	// Returns the winning piece. If there is no winner, returns nullptr.
	virtual WinningPiece FightPieceOfTheSameType(Piece& enemy) override;

	// Joker implements it according to its own rules.
	// When a piece wants to move to an occupied location by a piece of other type,
	// Deletes and removes the loser from its owner.
	// Returns the winning piece. If there is no winner, returns nullptr.
	virtual WinningPiece FightWithOtherPieceType(Piece& enemy) override;
public:
	Joker(shared_ptr<Player> owner = nullptr) : Piece(owner) {}
	Joker(int ownerNum) : Piece(ownerNum) {}
	Joker(int ownerNum, std::unique_ptr<Piece> actualPiece) : Piece(ownerNum), mActualPiece(std::move(actualPiece)) {}

	// Gets this piece type.
	PieceFactory::PieceType GetPieceType() const override { return PieceFactory::PieceType::Joker; }

	// Return true if the given new piece has the same owner as this joker,
	// and it represents a piece the Joker wants to be, from: R P S B
	// If so, delete old actual piece and replace it by the given new piece.
	bool SetActualPiece(unique_ptr<Piece> newPiece);

	bool GetIsMovingPiece() const override { return mActualPiece->GetIsMovingPiece(); }

	char GetPieceChar() const override { return JOKER_CHAR; }

	char GetActualPieceChar() const override { return mActualPiece->GetPieceChar(); }

	virtual bool IsStrongerThan(const Piece& other) const { return mActualPiece->IsStrongerThan(other); };

	const Piece& PeekActualPiece() const { return *mActualPiece; }

	// Return the actual piece that this joker represents at the moment
	// TODO: delete it!
	const Piece* GetActualPiece() const { return mActualPiece.get(); }
};

#endif //ADTO_TARGIL1_JOKER_PIECE_H
