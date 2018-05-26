#ifndef ADVANCEDTOPICS_MASTER_AUTOPLAYERALGORITHM_H
#define ADVANCEDTOPICS_MASTER_AUTOPLAYERALGORITHM_H

//--------------------------
// AutoPlayerAlgorithm Interface
//--------------------------

// Creates an auto algorithm for the player. 
// That is, generating the moves according to finding the best move possible.

// @author Hila Tsfadia, Jael Fafner TAU - Advanced Topics in Programming - 2018 Semester B

#include <vector>
#include "PlayerAlgorithm.h"
#include "BoardImpl.h"
#include "PiecePositionImpl.h"
#include "StrategyPiece.h"

#define NONE -1
#define NUM_OF_PLAYERS 2
#define NUM_OF_ADJACENT_POSITIONS 4
#define FIRST_PLAYER_NUM 1
#define SECOND_PLAYER_NUM 2
#define TIE 0

class AutoPlayerAlgorithm : public PlayerAlgorithm
{
private:
	BoardImpl<StrategyPiece> mPlayersStrategyBoards[NUM_OF_PLAYERS];
	//bool mFlagPlaceKnown;
	int mPlayer;
	int mOpponent;
	int mOpponentNumCoveredPieces;
	int mOpponentNumCoveredMovablePieces = 0;
	//int mPlayerNumMovablePieces;
	std::vector<PointImpl> mOpponentFlagLocations;
	std::vector<PointImpl> mPlayerJokerLocations;
	//int lastMovedPieceID = NONE;

	enum class MoveType { RunAway, Attack, TowardsFlag, Random };

	//-----------------------------------------------------------
	// General helper functions
	//-----------------------------------------------------------

	// Return true iff the two playe's strategy boards are empty in the given location
	bool AreBothBoardsEmptyInPosition(int x, int y) const;

	// Return true iff the two playe's strategy boards are empty in the given location
	bool AreBothBoardsEmptyInPosition(const Point& pos) const;

	// Fill all the adjacent locations to the given position which are legal in the board.
	void FillAdjacentLegalPositions(const Point& pos, std::vector<unique_ptr<PointImpl>>& vectorToFill) const;

	// Retruns true iff the given piece is/might be threatened in the given position
	bool isThreatenedInPosition(const StrategyPiece& piece, const PointImpl& pos) const;

	// Retruns true iff the given piece is/might be threatening in the given position
	bool isThreateningInPosition(const StrategyPiece& piece, const PointImpl& pos) const;

	// Updates the strategy inner represantations according to the given fight information.
	void updateStrategyAccordingToFight(const FightInfo& fight);

	// Tries to find the opponnent's flags.
	void findOpponentFlags();

	// Updates if the player's piece in the given position is threatened/threatening.
	void updateThreatsForPlayerInPosition(int player, const PointImpl& pos);

	// Updates all the pieces of all players if they are threatened/threatening.
	void updateThreats();

	// Deletes this joker location from the jokers vector of locations
	void eraseJokerLocation(const Point& jokerPos);

	// Change the joker location in the jokers vector of locations
	void updateJokerLocation(const Point& oldJokerPos, const Point& newJokerPos);


	//-----------------------------------------------------------
	// getInitialPositions helper functions
	//-----------------------------------------------------------
	
	// Updates the line number according to given isToMoveForward. 
	// If true, inc pos, else, dec pos
	void UpdateLineNumber(int& yPos, bool isToMoveForward) const;

	// Init the initial positions for a specific piece type, starting from the given position.
	// Updates the given position to the next position available 
	void initPositionsVectorOneType(std::vector<unique_ptr<PiecePosition>>& vectorToFill, int& xPos, int& yPos, bool isToMoveForward, 
		int count, char typeChar, char jokerReper = NON_JOKER_REP) const;

	// Does the filling of the given vector with the initial positions of the player.
	void initPositionsVector(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill) const;

	// Init the board of the player of this algorithm.
	void initTheAlgorithmPlayerBoard(int player, const std::vector<unique_ptr<PiecePosition>>& vectorToFill);


	//-----------------------------------------------------------
	// notifyOnInitialBoard helper functions
	//-----------------------------------------------------------
	
	// Clears the two players' boards in the given location
	void ClearPlayersBoardsInPosition(const Point& pos);

	// Updates the strategy inner represantations according to the given board.
	void updateStrategyAccordingToBoard(const Board& b);


	//-----------------------------------------------------------
	// getMove helper functions
	//-----------------------------------------------------------
	
	// Fill in the given vector all the pieces which are moving and in the given distance from the flag position
	void getMovingPiecesInDistanceFromFlag(const PointImpl& flag_pos, 
		int distance, std::vector<unique_ptr<PointImpl>>& posVectorToFill) const;

	// Get a free place for the piece in the given location, which is closer to the flag position.
	unique_ptr<PointImpl> getUnoccupiedPlaceTowardsFlag(const PointImpl& from, const PointImpl & flagPos) const;

	// Return a move which is making a moving piece closer to a flag.
	unique_ptr<Move> conquerTheFlag() const;

	// Return true iff can move the given piece to the given position, according to the given MoveType.
	bool isRelevantDestination(const StrategyPiece& piece, const PointImpl& pos, MoveType moveType) const;

	// Search in the adjacent places to the given postion for a relevant destination for 
	// the given piece, according to the given MoveType.
	unique_ptr<PointImpl> getStrategyDestination(const StrategyPiece& piece, const PointImpl& from, MoveType moveType) const;

	// Return true iff the given piece has to be moved according to the given MoveType
	bool isPieceToMove(const StrategyPiece& strategyPiece, MoveType moveType) const;

	// Get a move according to the given MoveType in the given row
	unique_ptr<Move> getStrategyMoveInPosition(MoveType moveType, int row, int col) const;

	// Get a move according to the given MoveType in the player's board.
	unique_ptr<Move> getStrategyMove(MoveType moveType) const;

	//void movePieceOnInfoBoard(const Move& getMove);


	//-----------------------------------------------------------
	// getJokerChange helper functions
	//-----------------------------------------------------------
	
	// Change the representation of a threatened joker, and return the change details.
	unique_ptr<JokerChange> changeThreatenedJoker(const Point& pos);

public:

	//-----------------------------------------------------------
	// PlayerAlgorithm methods
	//-----------------------------------------------------------
	virtual void getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill) override;
	virtual void notifyOnInitialBoard(const Board& b, const std::vector<unique_ptr<FightInfo>>& fights) override;
	virtual void notifyOnOpponentMove(const Move& move) override; // called only on opponent's move
	virtual void notifyFightResult(const FightInfo& fightInfo) override; // called only if there was a fight
	virtual unique_ptr<Move> getMove() override;
	virtual unique_ptr<JokerChange> getJokerChange() override; // nullptr if no change is requested
};


#endif //ADTO_TARGIL1_UNCOVEREDPIECE_H

