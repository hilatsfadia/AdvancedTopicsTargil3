#ifndef ADTO_TARGIL1_PIECE_H
#define ADTO_TARGIL1_PIECE_H

#include <ostream>
#include "PieceFactory.h"
class Player;

class Piece
{
protected:
	char mPieceChar;
	Player* mOwner;
	
	//PieceType mPieceType;

	// When a piece wants to move to an occupied location by a piece of the same type.
	// Deletes and removes both pieces from their owner.
	virtual Piece* FightPieceOfTheSameType(Piece* enemy);

	// Inherited classes should implement according to their own rules.
	// When a piece wants to move to an occupied location by a piece of other type,
	// Deletes and removes the loser from its owner.
	// Returns the winning piece. If there is no winner, returns nullptr.
	virtual Piece* FightWithOtherPieceType(Piece* enemy) = 0;
public:
	virtual ~Piece() {};

	Piece(char pieceChar, Player* owner = nullptr) : mPieceChar(pieceChar), mOwner(owner) {}

	// Deletes and removes the loser from its owner.
	// Returns winner.
	Piece* LoseToPiece(Piece* enemy);

	// For Explosion with bomb or tools of the same type.
	// Deletes and removes both pieces from their owner.
	// Returns nullptr.
	Piece* BothPiecesLosers(Piece* enemy);

	// Update both the piece and the owner of the ownership.
	bool InitializeOwner(Player* owner);

	// Ownership through joker
	// Sets the owner without notifying him.
	// Notify only the piece.
	void SetTransparentOwner(Player* owner);

	// Gets the owner of this piece
	Player* GetOwner() { return mOwner; }

	// When a piece wants to move to an occupied location,
	// the two pieces fight, and at most one of them stays alive.
	// Returns the winning piece. If there is no winner, returns nullptr.
	Piece* Fight(Piece* enemy);

	// Gets this piece type. (Look at PieceFactory::PieceType enum).
	virtual PieceFactory::PieceType GetPieceType() const = 0;

	virtual bool isMovingPiece() const = 0;

	// Operator overloading for printing issues.
	friend std::ostream& operator<<(std::ostream& out, const Piece& piece) {
		return out << piece.mPieceChar;
	}
};

#endif //ADTO_TARGIL1_PIECE_H
