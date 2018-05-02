#ifndef ADTO_TARGIL1_Player_H
#define ADTO_TARGIL1_Player_H

#include <map>
#include <vector>
#include "PieceFactory.h"
#include "PlayerAlgorithm.h"

#define R 2
#define P 5
#define S 1
#define B 2
#define J 2
#define F 1

class Player
{
	static int mPlayerCounter;

	// Number of pieces of each type that the player holds.
	std::map<PieceFactory::PieceType, int> mCountPieces;

	// Max number of pieces of each type that the player can hold.
	static std::map<PieceFactory::PieceType, int> mMaxCountPieces;

	int mPlayerNum;
	int mCountMovingPieces = 0;
	int mCountPoints = 0;
	int mPositionedAllFlags = false;

	PlayerAlgorithm* mPlayerAlgorithm = nullptr;

public:
	Player(PlayerAlgorithm* playerAlgorithm);
	~Player();

	int GetPlayerNum() const { return mPlayerNum; }

	// Gets the number of pieces this player has of the given type.
	int GetPieceCount(PieceFactory::PieceType pieceType);

	// Removes one piece of the given type from this player.
	// Returns true if this player had at least one piece 
	// of the given type before removing.
	bool DecPieceCount(PieceFactory::PieceType pieceType);

	// Adds one piece of the given type from this player.
	// Returns true if this player can hold one more piece
	// of the given type before addition.
	// Also, checks if the given type is flag, If so, updates if
	// all flags were positioned.
	bool IncPieceCountInInitialization(PieceFactory::PieceType pieceType);

	// Gets the number of players' pieces that can move.
	int GetCountOfMovingPieces();

	// Gets the number of flag this player has.
	int GetFlagsCount() { return mCountPieces[PieceFactory::PieceType::Flag]; }

	bool DoesPosiotionedAllFlags() const { return mPositionedAllFlags; }

	void AddToCountMovingPieces(PieceFactory::PieceType pieceType);
	void RemoveFromCountMovingPieces(PieceFactory::PieceType pieceType);

	PlayerAlgorithm* GetPlayerAlgorithm() { return mPlayerAlgorithm; }
	//friend bool operator==(const Player& lhs, const Player& rhs);
	//friend bool operator!=(const Player& lhs, const Player& rhs) { return !(lhs == rhs); }
};

#endif //ADTO_TARGIL1_Player_H
