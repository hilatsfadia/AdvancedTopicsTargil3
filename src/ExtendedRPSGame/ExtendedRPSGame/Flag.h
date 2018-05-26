#ifndef ADTO_TARGIL1_FLAG_H
#define ADTO_TARGIL1_FLAG_H

//--------------------------
// Flag Interface
//--------------------------

// Creates and handles all instances of piece of type flag. 

// @author Hila Tsfadia, Jael Fafner TAU - Advanced Topics in Programming - 2018 Semester B

#include "Piece.h"

#define FLAG_CHAR 'F'

class Flag :
	public Piece
{
protected:
	// Flag implements it according to its own rules.
	// When a piece wants to move to an occupied location by a piece of other type,
	// Deletes and removes the loser from its owner.
	// Returns the winner.
	virtual WinningPiece FightWithOtherPieceType(Piece&) override;
public:
	Flag(shared_ptr<Player> owner = nullptr) : Piece(owner) {}
	Flag(int ownerNum) : Piece(ownerNum) {}

	// Gets this piece type.
	PieceFactory::PieceType GetPieceType() const override { return PieceFactory::PieceType::Flag; }

	bool GetIsMovingPiece() const override { return false; }

	char GetPieceChar() const override { return FLAG_CHAR; }
};

#endif //ADTO_TARGIL1_FLAG_H
