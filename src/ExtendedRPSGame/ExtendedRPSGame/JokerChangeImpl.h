#ifndef ADVANCEDTOPICS_MASTER_JOKERCHANGEIMPL_H
#define ADVANCEDTOPICS_MASTER_JOKERCHANGEIMPL_H

#include "PointImpl.h"
#include "JokerChange.h"

class JokerChangeImpl : public JokerChange {
private:
    PointImpl mJokerChangePosition;
    char mJokerNewRep;

public:
    JokerChangeImpl(PointImpl jokerChangePos, char jokerNewRep): mJokerChangePosition(jokerChangePos), mJokerNewRep(jokerNewRep){}
	inline virtual const Point& getJokerChangePosition() const override { return mJokerChangePosition; }
    virtual char getJokerNewRep() const override {return mJokerNewRep;} // R, P, S or B (but NOT J and NOT F)
};


#endif //ADVANCEDTOPICS_MASTER_JOKERCHANGEIMPL_H
