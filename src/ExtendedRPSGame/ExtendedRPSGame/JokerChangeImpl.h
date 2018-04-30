//
// Created by USER on 29/04/2018.
//

#ifndef ADVANCEDTOPICS_MASTER_JOKERCHANGEIMPL_H
#define ADVANCEDTOPICS_MASTER_JOKERCHANGEIMPL_H

#include "Point.h"
#include "JokerChange.h"

class JokerChangeImpl : public JokerChange {
private:
    Point* mJokerChangePosition;
    char mJokerNewRep;

public:
    JokerChangeImpl(Point* jokerChangePos, char jokerNewRep): mJokerChangePosition(jokerChangePos), mJokerNewRep(jokerNewRep){}
    virtual const Point& getJokerChangePosition() const override { return *mJokerChangePosition;}
    virtual char getJokerNewRep() const override {return mJokerNewRep;} // R, P, S or B (but NOT J and NOT F)
};


#endif //ADVANCEDTOPICS_MASTER_JOKERCHANGEIMPL_H
