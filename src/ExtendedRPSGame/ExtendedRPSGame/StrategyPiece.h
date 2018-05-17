#ifndef ADTO_TARGIL1_PIECE_STRATEGY_H
#define ADTO_TARGIL1_PIECE_STRATEGY_H

#include "Piece.h"
#include "PieceFactory.h"
#define COVERED_CHAR 'C'

class StrategyPiece :
	public Piece
{
private:
	Piece* mUncoveredPiece = nullptr;
	bool mIsThreatened = false;
	bool mIsThreathening = false;

public:
	StrategyPiece(int ownerNum, Piece* uncoveredPiece = nullptr) : Piece(ownerNum) { mUncoveredPiece = uncoveredPiece; }

	// Gets this piece type.
	// If this piece is still covered, gets PieceType::Unknown.
	PieceFactory::PieceType GetPieceType() const override;

	bool isMovingPiece() const override;
	char GetPieceChar() const override;

	// Uncovers this piece by giving it the actual piece it represents.
	void UncoverPiece(Piece* uncoveredPiece) { mUncoveredPiece = uncoveredPiece; }

	bool GetIsThreatened() { return mIsThreatened; }
	void SetIsThreatened(bool isThreatend) { mIsThreatened = isThreatend; }
	bool getIsThreathening() { return mIsThreathening; }
	void SetIsThreathening(bool isThreatening) { mIsThreathening = isThreatening; }
};

#endif //ADTO_TARGIL1_PIECE_STRATEGY_H
