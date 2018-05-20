#ifndef ADVANCEDTOPICS_MASTER_FILEPLAYERALGORITHM_H
#define ADVANCEDTOPICS_MASTER_FILEPLAYERALGORITHM_H

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
    virtual void getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill) override;
    virtual void notifyOnInitialBoard(const Board& b, const std::vector<unique_ptr<FightInfo>>& fights) override {} //empty impl
    virtual void notifyOnOpponentMove(const Move& move) override {} // called only on opponent�s move - empty impl
    virtual void notifyFightResult(const FightInfo& fightInfo) override {} // called only if there was a fight - empty impl
    virtual unique_ptr<Move> getMove() override;
    virtual unique_ptr<JokerChange> getJokerChange() override; // nullptr if no change is requested
};

#endif //ADVANCEDTOPICS_MASTER_FILEPLAYERALGORITHM_H
