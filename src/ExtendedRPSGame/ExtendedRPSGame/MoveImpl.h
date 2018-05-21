//
// Created by USER on 29/04/2018.
//

#ifndef ADVANCEDTOPICS_MASTER_MOVEIMPL_H
#define ADVANCEDTOPICS_MASTER_MOVEIMPL_H

#include "Point.h"
#include "Move.h"
#include "PointImpl.h"

class MoveImpl : public Move{
	PointImpl mFrom;
	PointImpl mTo;
public:
	MoveImpl(const PointImpl& from, const PointImpl& to) : mFrom(from), mTo(to){}
    virtual const Point& getFrom() const { return mFrom; }
    virtual const Point& getTo() const { return  mTo; }
};


#endif //ADVANCEDTOPICS_MASTER_MOVEIMPL_H
