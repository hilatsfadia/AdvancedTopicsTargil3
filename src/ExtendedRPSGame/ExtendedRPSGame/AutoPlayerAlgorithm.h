#ifndef ADVANCEDTOPICS_MASTER_AUTOPLAYERALGORITHM_H
#define ADVANCEDTOPICS_MASTER_AUTOPLAYERALGORITHM_H

#include "PlayerAlgorithm.h"

class AutoPlayerAlgorithm :
	public PlayerAlgorithm
{
public:
	// Returns the initialization file name of the given player
	virtual void getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill) override;
	virtual void notifyOnInitialBoard(const Board& b, const std::vector<unique_ptr<FightInfo>>& fights) override;
	virtual void notifyOnOpponentMove(const Move& move) override; // called only on opponent's move
	virtual void notifyFightResult(const FightInfo& fightInfo) override; // called only if there was a fight
	virtual unique_ptr<Move> getMove() override;
	virtual unique_ptr<JokerChange> getJokerChange() override; // nullptr if no change is requested
};


#endif //ADVANCEDTOPICS_MASTER_AUTOPLAYERALGORITHM_H