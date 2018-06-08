#ifndef ADTO_TARGIL1_Player_H
#define ADTO_TARGIL1_Player_H

//--------------------------
// Player Interface
//--------------------------

// Manages and creates a new player to play on board and handles all his pieces.

// @author Hila Tsfadia, Jael Fafner TAU - Advanced Topics in Programming - 2018 Semester B

#include <map>
#include <vector>
#include "PieceFactory.h"
#include "PlayerAlgorithm.h"

// TODO: delete it!
//#define R 1
//#define P 0
//#define S 0
//#define B 0
//#define J 0
//#define F 1

#define R 2
#define P 5
#define S 1
#define B 2
#define J 2
#define F 1

// Type Alias
using PieceType = PieceFactory::PieceType;

class Player
{
	// Number of pieces of each type that the player holds.
	std::map<PieceType, int> mCountPieces;

	// Max number of pieces of each type that the player can hold.
	static std::map<PieceType, int> mMaxCountPieces;

	int mPlayerNum;
	int mCountMovingPieces = 0;
	int mCountPoints = 0;
	int mPositionedAllFlags = false;

	//unique_ptr<PlayerAlgorithm> mPlayerAlgorithm = nullptr;

public:
	//Player(PlayerAlgorithm* playerAlgorithm);
	Player(int playerNum);
	~Player();

	int GetPlayerNum() const { return mPlayerNum; }

	// Gets the number of pieces this player has of the given type.
	int GetPieceCount(PieceType pieceType) const;

	// Removes one piece of the given type from this player.
	// Returns true if this player had at least one piece 
	// of the given type before removing.
	bool DecPieceCount(PieceType pieceType);

	// Adds one piece of the given type from this player.
	// Returns true if this player can hold one more piece
	// of the given type before addition.
	// Also, checks if the given type is flag, If so, updates if
	// all flags were positioned.
	bool IncPieceCountInInitialization(PieceType pieceType);

	// Gets the number of players' pieces that can move.
	int GetCountOfMovingPieces() const;

	// Gets the number of flag this player has.
	int GetFlagsCount() const { return GetPieceCount(PieceType::Flag); };

	bool DoesPosiotionedAllFlags() const { return mPositionedAllFlags; }

	void AddToCountMovingPieces(PieceType pieceType);
	void RemoveFromCountMovingPieces(PieceType pieceType);

	//PlayerAlgorithm* GetPlayerAlgorithm() { return mPlayerAlgorithm; }
	//friend bool operator==(const Player& lhs, const Player& rhs);
	//friend bool operator!=(const Player& lhs, const Player& rhs) { return !(lhs == rhs); }
};

#endif //ADTO_TARGIL1_Player_H
