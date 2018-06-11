#ifndef ADVANCEDTOPICS_MASTER_JOKERCHANGEIMPL_H
#define ADVANCEDTOPICS_MASTER_JOKERCHANGEIMPL_H

//--------------------------
// JokerChangeImpl Interface
//--------------------------

// Manages the return of a joker change move on the board for the player.

// @author Hila Tsfadia, Jael Fafner TAU - Advanced Topics in Programming - 2018 Semester B

#include "PointImpl.h"
#include "JokerChange.h"

class JokerChangeImpl : public JokerChange {
private:
    PointImpl mJokerChangePosition;
    char mJokerNewRep;

public:
    JokerChangeImpl(const PointImpl& jokerChangePos, char jokerNewRep): 
		mJokerChangePosition(jokerChangePos), mJokerNewRep(jokerNewRep){}
	JokerChangeImpl(const Point& jokerChangePos, char jokerNewRep) : 
		mJokerChangePosition(jokerChangePos.getX(), jokerChangePos.getY()), mJokerNewRep(jokerNewRep) {}

	virtual const Point& getJokerChangePosition() const override { return mJokerChangePosition; }
    virtual char getJokerNewRep() const override { return mJokerNewRep; } // R, P, S or B (but NOT J and NOT F)
};


#endif //ADVANCEDTOPICS_MASTER_JOKERCHANGEIMPL_H
