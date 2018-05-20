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
	bool mIsMovingPiece = false;

	PieceFactory::PieceType GetStrongerNotJoker(const Piece* piece) const;

public:
	StrategyPiece(int ownerNum, Piece* uncoveredPiece = nullptr) : Piece(ownerNum) { mUncoveredPiece = uncoveredPiece; }

	// Gets this piece type.
	// If this piece is still covered, gets PieceType::Unknown.
	PieceFactory::PieceType GetPieceType() const override;

	bool GetIsMovingPiece() const override { return mIsMovingPiece; }

	char GetPieceChar() const override;

	void SetIsMovingPiece(bool isMovingPiece) { mIsMovingPiece = isMovingPiece; }

	// Uncovers this piece by giving it the actual piece it represents.
	void UncoverPiece(Piece* uncoveredPiece);
	void UncoverPiece(char uncoveredPieceChar);

	// Returns true iff this piece is threatened by an enemy piece (one or more)
	bool GetIsThreatened() { return mIsThreatened; }
	void SetIsThreatened(bool isThreatend) { mIsThreatened = isThreatend; }

	// Returns true iff this piece is threatening an enemy piece (one or more)
	bool GetIsThreathening() { return mIsThreathening; }
	void SetIsThreathening(bool isThreatening) { mIsThreathening = isThreatening; }

	virtual bool IsStrongerThan(Piece* other) const;

	PieceFactory::PieceType GetStronger(const Piece* piece) const;
};

#endif //ADTO_TARGIL1_PIECE_STRATEGY_H
