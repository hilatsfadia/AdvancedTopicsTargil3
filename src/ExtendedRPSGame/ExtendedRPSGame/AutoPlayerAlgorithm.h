#ifndef ADVANCEDTOPICS_MASTER_AUTOPLAYERALGORITHM_H
#define ADVANCEDTOPICS_MASTER_AUTOPLAYERALGORITHM_H
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include <iterator>
#include <iostream>
#include "PlayerAlgorithm.h"
#include "BoardImpl.h"
#include <cctype>
#include "Player.h"
#include "MoveImpl.h"
#include "PiecePositionImpl.h"
#include "StrategyPiece.h"
#define NUM_OF_PLAYERS 2
#define EMPTY_BOARD_LOCATION 0
#define TIE 0
#define PIECE_TYPE_NUM 6

class AutoPlayerAlgorithm : public PlayerAlgorithm
{
private:
	BoardImpl<StrategyPiece> mPlayersStrategyBoards[NUM_OF_PLAYERS];
	//bool mFlagPlaceKnown;
	int mPlayer;
	int mOpponent;
	int mNumCoveredPieces;
	int mNumMovablePieces;
	std::vector<PointImpl> mOpponentFlagLocations;
	std::vector<PointImpl> mJokerLocations;

	//-----------------------------------------------------------
	// getInitialPositions helper functions
	//-----------------------------------------------------------
	// Updates the line number according to given isToMoveForward. 
	// If true, inc pos, else, dec pos
	void UpdateLineNumber(int& yPos, bool isToMoveForward);

	// Init the initial positions for a specific piece type, starting from the given position.
	// Updates the given position to the next position available 
	void initPositionsVectorOneType(std::vector<unique_ptr<PiecePosition>>& vectorToFill, int& xPos, int& yPos, bool isToMoveForward, int count, char typeChar, char jokerReper = NON_JOKER_REP);

	// Does the filling of the given vector with the positions.
	void initPositionsVector(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill);

	// Init the board of the player of this algorithm.
	void initTheAlgorithmPlayerBoard(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill);

	//-----------------------------------------------------------
	// notifyOnInitialBoard helper functions
	//-----------------------------------------------------------
	void ClearPlayersBoardsInPosition(const Point& pos);
	void updateStrategyAccordingToBoard(const Board& b);

	//-----------------------------------------------------------
	// getMove helper functions
	//-----------------------------------------------------------
	unique_ptr<PointImpl> getEmptySquareToMoveTo(const PointImpl& pos);
	unique_ptr<Move> getNormalMove();
	//void movePieceOnInfoBoard(const Move& getMove);


	void updateStrategyAccordingToFight(const FightInfo& fight);

//	void eraseJokerLocation(const Point& jokerPos);
//
//	void updateJokerLocation(const Point& oldJokerPos, const Point& newJokerPos);
//
//	void updateIsThreatened(int player);
//
//	void updateIsThreatening(int player);
//
//	bool checkIsThreatening(int xPos, int yPos, int player);
//	
//	PointImpl * conquerPiece(int xPos, int yPos);
//
//	PointImpl * runForYourLife(int xPos, int yPos);
//
	void findOpponentFlag();
//
//	unique_ptr<Move> saveAPiece();
//
//	unique_ptr<Move> eatOpponentPiece();

	bool AreBothBoardsEmptyInPosition(int x, int y) const;
	bool AreBothBoardsEmptyInPosition(const Point& pos) const;

	//-----------------------------------------------------------
	// conquerTheFlag helper functions
	//-----------------------------------------------------------
	void getMovingPiecesInDistanceFromFlag(const PointImpl & flag_pos, int distance, std::vector<unique_ptr<PointImpl>>& posVectorToFill);

	unique_ptr<PointImpl> GetUnoccupiedPlace(bool condition1, PointImpl& option1, bool condition2, PointImpl& option2) const;

	unique_ptr<PointImpl> GetUnoccupiedPlaceHorizotally(const PointImpl &moveFrom, const PointImpl &flagPos) const;

	unique_ptr<PointImpl> GetUnoccupiedPlaceVertically(const PointImpl & moveFrom, const PointImpl & flagPos)const;

	unique_ptr<PointImpl> getUnoccupiedPlaceTowardsFlag(const PointImpl & moveFrom, const PointImpl & flagPos, bool ifToCheckThreatened) const;

	unique_ptr<Move> conquerTheFlag();

//
//	bool checkIsThreatened(int xPos, int yPos, int player);
//
//	bool returnThreatened(int xPos, int yPos, int newxPos, int newyPos, int opponent);
//
//	bool returnPointThreatening(int xpos, int ypos, int newxpos, int newypos);
//
//	void getVectorThreateningPieces(PointImpl & pos, std::vector<PointImpl>& posVectorToFill);
//
//	//char getCharFromPieceType(PieceFactory::PieceType type);

public:

	virtual void getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill) override;
	virtual void notifyOnInitialBoard(const Board& b, const std::vector<unique_ptr<FightInfo>>& fights) override;
	virtual void notifyOnOpponentMove(const Move& move) override; // called only on opponent's move
	virtual void notifyFightResult(const FightInfo& fightInfo) override; // called only if there was a fight
	virtual unique_ptr<Move> getMove() override;
	virtual unique_ptr<JokerChange> getJokerChange() override; // nullptr if no change is requested


//
//	virtual void notifyOnInitialBoard(const Board& b, const std::vector<unique_ptr<FightInfo>>& fights) override;
//	
//	
//	virtual void notifyOnOpponentMove(const Move& move) override; // called only on opponent's move
//	
//	virtual void notifyFightResult(const FightInfo& fightInfo) override; // called only if there was a fight
//	
//	virtual unique_ptr<Move> getMove() override;
//
//	virtual unique_ptr<JokerChange> getJokerChange() override; // nullptr if no change is requested

	
};


#endif //ADTO_TARGIL1_UNCOVEREDPIECE_H

