#ifndef ADTO_TARGIL1_PIECE_H
#define ADTO_TARGIL1_PIECE_H

#include <ostream>
#include "PieceFactory.h"
#include "Player.h"

class Piece
{
protected:
	Player* mOwner;
	int mOwnerNum;
	
	//PieceType mPieceType;

	// When a piece wants to move to an occupied location by a piece of the same type.
	// Deletes and removes both pieces from their owner.
	virtual Piece* FightPieceOfTheSameType(Piece* enemy);

	// Inherited classes should implement according to their own rules.
	// When a piece wants to move to an occupied location by a piece of other type,
	// Deletes and removes the loser from its owner.
	// Returns the winning piece. If there is no winner, returns nullptr.
	//virtual Piece* FightWithOtherPieceType(Piece* enemy) = 0;
	// TODO: look
	virtual Piece* FightWithOtherPieceType(Piece* enemy) { return nullptr; }

public:
	virtual ~Piece() {};
	//Piece() : mOwner(nullptr) {};

	// TODO: maybe remove the owner itself and keep only it's number
	// TODO!!!
	Piece(Player* owner) : mOwner(owner) { if (owner != nullptr) mOwnerNum = owner->GetPlayerNum(); }
	Piece(int ownerNum) : mOwner(nullptr), mOwnerNum(ownerNum) {}

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
	//void SetTransparentOwner(Player* owner);

	// Gets the owner of this piece
	Player* GetOwner() const { return mOwner; }

	// Gets the owner number of this piece
	int GetOwnerNum() { return mOwnerNum; }

	// When a piece wants to move to an occupied location,
	// the two pieces fight, and at most one of them stays alive.
	// Returns the winning piece. If there is no winner, returns nullptr.
	Piece* Fight(Piece* enemy);

	// Gets this piece type. (Look at PieceFactory::PieceType enum).
	virtual PieceFactory::PieceType GetPieceType() const = 0;

	virtual bool GetIsMovingPiece() const = 0;

	virtual char GetPieceChar() const = 0;
	
	virtual bool IsStrongerThan(Piece* other) const { return false; };

	// Operator overloading for printing issues.
	friend std::ostream& operator<<(std::ostream& out, const Piece& piece);
};

#endif //ADTO_TARGIL1_PIECE_H
