#ifndef ADTO_TARGIL1_RPS_PIECE_H
#define ADTO_TARGIL1_RPS_PIECE_H

#include "Piece.h"
#include <memory>

// An abstract class to the RPS game pieces.
class RPSPiece :
	public Piece
{
public:
	RPSPiece(std::shared_ptr<Player> owner = nullptr) : Piece(owner) {}
	RPSPiece(int ownerNum) : Piece(ownerNum) {}

	virtual WinningPiece FightWithOtherPieceType(Piece& enemy) override;
	virtual ~RPSPiece() {}
	virtual bool IsStrongerThan(const Piece& other) const override;
};

#endif //ADTO_TARGIL1_RPS_PIECE_H