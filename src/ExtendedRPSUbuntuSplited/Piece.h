#ifndef ADTO_TARGIL1_PIECE_H
#define ADTO_TARGIL1_PIECE_H

//--------------------------
// Piece Interface
//--------------------------

// Manages the characteristics of the individual piece and its 
// fights on the actual gameboard.
// @author Hila Tsfadia, Jael Fafner TAU - Advanced Topics in Programming - 2018 Semester B

#include <ostream>
#include "PieceFactory.h"
#include "Player.h"
#include <memory>

using std::shared_ptr;
// Type Alias
using PieceType = PieceFactory::PieceType;

class Piece
{
protected:

	// shared ptr because it is shared among it's pieces (as their owner) and the game.
	shared_ptr<Player> mOwner;
	int mOwnerNum;

	// For Explosion with bomb or tools of the same type.
	// Removes both pieces from their owner.
	void BothPiecesLosers(Piece& enemy);

public:
	enum class WinningPiece { Tie = 0, CallingObject = 1, OtherObject = 2 };

	virtual ~Piece() {};

	// TODO: maybe remove the owner itself and keep only it's number
	Piece(shared_ptr<Player> owner = nullptr) : mOwner(owner) { if (owner != nullptr) mOwnerNum = owner->GetPlayerNum(); }
	Piece(int ownerNum) : mOwner(nullptr), mOwnerNum(ownerNum) {}

	// Update both the piece and the owner of the ownership.
	bool InitializeOwner(std::shared_ptr<Player> owner);

	// Ownership through joker
	// Sets the owner without notifying him.
	// Notify only the piece.
	//void SetTransparentOwner(Player* owner);

	// Gets the owner of this piece
	shared_ptr<Player> GetOwner() const { return mOwner; }

	// Gets the owner number of this piece
	int GetOwnerNum() const { return mOwnerNum; }

	// Removes the loser from its owner.
	// Returns winner.
	void LoseToPiece();

	// When a piece wants to move to an occupied location by a piece of the same type.
	// Deletes and removes both pieces from their owner.
	virtual WinningPiece FightPieceOfTheSameType(Piece& enemy);

	// Inherited classes should implement according to their own rules.
	// When a piece wants to move to an occupied location by a piece of other type,
	// Deletes and removes the loser from its owner.
	// Returns the winning piece. If there is no winner, returns Tie.
	//virtual Piece* FightWithOtherPieceType(Piece* enemy) = 0;
	// TODO: look
	virtual WinningPiece FightWithOtherPieceType(Piece&) { return WinningPiece::Tie; }

	// When a piece wants to move to an occupied location,
	// the two pieces fight, and at most one of them stays alive.
	// Returns the winning piece. If there is no winner, returns nullptr.
	WinningPiece Fight(Piece& enemy);

	// Gets this piece type. (Look at PieceType enum).
	virtual PieceType GetPieceType() const = 0;

	// Rock, Paper, Scissors, Bomb or Flag (but NOT Joker)
	virtual PieceType GetActualPieceType() const
	{
		return GetPieceType();
	}

	virtual bool GetIsMovingPiece() const = 0;

	// R, P, S, B or F (but NOT J)
	virtual char GetActualPieceChar() const 
	{
		return GetPieceChar();
	}

	virtual char GetPieceChar() const = 0;
	
	virtual bool IsStrongerThan(const Piece&) const { return false; };

	// Operator overloading for printing issues.
	friend std::ostream& operator<<(std::ostream& out, const Piece& piece);
};

#endif //ADTO_TARGIL1_PIECE_H
