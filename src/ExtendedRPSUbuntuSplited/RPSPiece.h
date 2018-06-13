#ifndef ADTO_TARGIL1_RPS_PIECE_H
#define ADTO_TARGIL1_RPS_PIECE_H

//--------------------------
// RPSPiece Class
//--------------------------

// Abstract class to the RPS game pieces. Manages the characteristics 
// of the rock paper scissors game piece. A piece of rock paper scissors 
// game should inherit from this class.

// @author Hila Tsfadia, Jael Fafner TAU - Advanced Topics in Programming - 2018 Semester B

#include "Piece.h"
#include <memory>

class RPSPiece :
	public Piece
{
public:
	RPSPiece(std::shared_ptr<Player> owner = nullptr) : Piece(owner) {}
	RPSPiece(int ownerNum) : Piece(ownerNum) {}
	virtual ~RPSPiece() {}

	virtual bool GetIsMovingPiece() const override { return true; }
	virtual WinningPiece FightWithOtherPieceType(Piece& enemy) override;
	virtual bool IsStrongerThan(const Piece& other) const override;
};

#endif //ADTO_TARGIL1_RPS_PIECE_H