//
// Created by USER on 29/04/2018.
//

#ifndef ADVANCEDTOPICS_MASTER_FILEPLAYERALGORITHM_H
#define ADVANCEDTOPICS_MASTER_FILEPLAYERALGORITHM_H

#include "PlayerAlgorithm.h"

#define INPUT_FILE_NAME_MAX_LEN 100
#define PLAYER_POSITION_FILE "player%d.rps_board"

class FilePlayerAlgorithm : public PlayerAlgorithm 
{
private:
	std::string GetPositionsFileName(int playerNum);
	//bool ParsePlayerInitFile(Player& player, const std::string & playerInputfileName);
public:
	// Returns the initialization file name of the given player
    virtual void getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill) override;
    virtual void notifyOnInitialBoard(const Board& b, const std::vector<unique_ptr<FightInfo>>& fights) override {} //empty impl
    virtual void notifyOnOpponentMove(const Move& move) override {} // called only on opponent�s move - empty impl
    virtual void notifyFightResult(const FightInfo& fightInfo) override {} // called only if there was a fight - empty impl
    virtual unique_ptr<Move> getMove() override;
    virtual unique_ptr<JokerChange> getJokerChange() override; // nullptr if no change is requested
};


#endif //ADVANCEDTOPICS_MASTER_FILEPLAYERALGORITHM_H
