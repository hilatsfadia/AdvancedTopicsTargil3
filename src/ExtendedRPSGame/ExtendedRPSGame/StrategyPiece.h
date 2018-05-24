#ifndef ADTO_TARGIL1_PIECE_STRATEGY_H
#define ADTO_TARGIL1_PIECE_STRATEGY_H

#include "Piece.h"
#include "PieceFactory.h"
#define COVERED_CHAR 'C'

class StrategyPiece :
	public Piece
{
private:
	unique_ptr<Piece> mUncoveredPiece = nullptr;
	bool mIsThreatened = false;
	bool mIsThreathening = false;
	bool mIsMovingPiece = false;

	char GetStrongerNotJoker(const Piece* piece) const;

public:
	StrategyPiece(int ownerNum, unique_ptr<Piece> uncoveredPiece);
	StrategyPiece(int ownerNum) : Piece(ownerNum) { }

	// Gets this piece type.
	// If this piece is still covered, gets PieceType::Unknown.
	PieceFactory::PieceType GetPieceType() const override;

	bool GetIsMovingPiece() const override;

	char GetPieceChar() const override;

	void SetIsMovingPiece(bool isMovingPiece) { mIsMovingPiece = isMovingPiece; }

	// Uncovers this piece by giving it the actual piece it represents.
	void UncoverPiece(std::unique_ptr<Piece> uncoveredPiece);

	// Uncover this piece by setting it's real piece.
	// If this piece is already uncovered, check if it's indeed the given char. If not, update
	// it to be the relevant joker (with a relevant representation).
	// uncoveredPieceChar should be R, P, S, B or F (but NOT J)
	void UncoverPiece(char uncoveredPieceChar);

	// Returns true iff this piece is threatened by an enemy piece (one or more)
	bool GetIsThreatened() const { return mIsThreatened; }
	void SetIsThreatened(bool isThreatend) { mIsThreatened = isThreatend; }

	// Returns true iff this piece is threatening an enemy piece (one or more)
	bool GetIsThreathening() const { return mIsThreathening; }
	void SetIsThreathening(bool isThreatening) { mIsThreathening = isThreatening; }

	virtual bool IsStrongerThan(const Piece& other) const;

	char GetStronger(const Piece* piece) const;
};

#endif //ADTO_TARGIL1_PIECE_STRATEGY_H
