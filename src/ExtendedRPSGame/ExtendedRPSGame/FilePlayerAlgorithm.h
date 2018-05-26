#ifndef ADVANCEDTOPICS_MASTER_FILEPLAYERALGORITHM_H
#define ADVANCEDTOPICS_MASTER_FILEPLAYERALGORITHM_H

//--------------------------
// FilePlayerAlgorithm Interface
//--------------------------

// Creates a file algorithm for the player. That is generating moves according
// to a moves file that had been provided to the algorithm.

// @author Hila Tsfadia, Jael Fafner TAU - Advanced Topics in Programming - 2018 Semester B

#include "PlayerAlgorithm.h"
#include "ParserMoveFile.h"
#include <fstream>

class FilePlayerAlgorithm : public PlayerAlgorithm 
{
private:
	//int mMovesFileLineNum = 0;
	int mPlayerNum;
	std::ifstream mMovesFileStream;
	ParserMoveFile mMoveFileParser;

public:
	~FilePlayerAlgorithm();

	//-----------------------------------------------------------
	// PlayerAlgorithm methods
	//-----------------------------------------------------------
    virtual void getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill) override;
    virtual void notifyOnInitialBoard(const Board&, const std::vector<unique_ptr<FightInfo>>&) override {} //empty impl
    virtual void notifyOnOpponentMove(const Move&) override {} // called only on opponent's move - empty impl
    virtual void notifyFightResult(const FightInfo&) override {} // called only if there was a fight - empty impl
    virtual unique_ptr<Move> getMove() override;
    virtual unique_ptr<JokerChange> getJokerChange() override; // nullptr if no change is requested
};

#endif //ADVANCEDTOPICS_MASTER_FILEPLAYERALGORITHM_H
