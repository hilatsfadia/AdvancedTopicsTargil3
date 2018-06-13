#ifndef ADVANCEDTOPICS_MASTER_MOVEIMPL_H
#define ADVANCEDTOPICS_MASTER_MOVEIMPL_H

//--------------------------
// MoveImpl Class
//--------------------------

// Manages the return of a move on the board for the player.

// @author Hila Tsfadia, Jael Fafner TAU - Advanced Topics in Programming - 2018 Semester B

#include "Point.h"
#include "Move.h"
#include "PointImpl.h"

class MoveImpl : public Move{
	PointImpl mFrom;
	PointImpl mTo;
public:
	MoveImpl(const PointImpl& from, const PointImpl& to) : mFrom(from), mTo(to){}

    virtual const Point& getFrom() const override { return mFrom; }
    virtual const Point& getTo() const override { return  mTo; }
};


#endif //ADVANCEDTOPICS_MASTER_MOVEIMPL_H
