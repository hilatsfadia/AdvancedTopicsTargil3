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
#define NUM_OF_ADJACENT_POSITIONS 4
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
	// General helper functions
	//-----------------------------------------------------------

	bool AreBothBoardsEmptyInPosition(int x, int y) const;

	bool AreBothBoardsEmptyInPosition(const Point& pos) const;

	void FillAdjacentLegalPositions(const PointImpl & pos, std::vector<unique_ptr<PointImpl>>& vectorToFill) const;

	bool isThreatenedInPosition(const Piece& piece, const PointImpl& pos) const;

	bool isThreateningInPosition(const Piece& piece, const PointImpl& pos) const;

	void updateStrategyAccordingToFight(const FightInfo& fight);

	void findOpponentFlag();

	void updateThreatsForPlayerInPosition(int player, const PointImpl& pos);

	void updateThreats();


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
	unique_ptr<PointImpl> getEmptyThreatFreeSquareToMoveTo(const PointImpl& from);
	unique_ptr<Move> getNormalMove();
	void getMovingPiecesInDistanceFromFlag(const PointImpl & flag_pos, int distance, std::vector<unique_ptr<PointImpl>>& posVectorToFill);
	unique_ptr<PointImpl> getUnoccupiedPlaceTowardsFlag(const PointImpl & moveFrom, const PointImpl & flagPos, bool ifToCheckThreatened) const;
	unique_ptr<Move> conquerTheFlag();
	//void movePieceOnInfoBoard(const Move& getMove);

	

//	void eraseJokerLocation(const Point& jokerPos);
//
//	void updateJokerLocation(const Point& oldJokerPos, const Point& newJokerPos);
//

//	PointImpl * conquerPiece(int xPos, int yPos);
//
//	PointImpl * runForYourLife(int xPos, int yPos);
//
//
//	unique_ptr<Move> saveAPiece();
//
//	unique_ptr<Move> eatOpponentPiece();
//
//	bool returnPointThreatening(int xpos, int ypos, int newxpos, int newypos);
//
//	void getVectorThreateningPieces(PointImpl & pos, std::vector<PointImpl>& posVectorToFill);

public:

	virtual void getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill) override;
	virtual void notifyOnInitialBoard(const Board& b, const std::vector<unique_ptr<FightInfo>>& fights) override;
	virtual void notifyOnOpponentMove(const Move& move) override; // called only on opponent's move
	virtual void notifyFightResult(const FightInfo& fightInfo) override; // called only if there was a fight
	virtual unique_ptr<Move> getMove() override;
	virtual unique_ptr<JokerChange> getJokerChange() override; // nullptr if no change is requested
};


#endif //ADTO_TARGIL1_UNCOVEREDPIECE_H

