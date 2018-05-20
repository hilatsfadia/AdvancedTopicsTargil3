#ifndef ADVANCEDTOPICS_MASTER_POINTIMPL_H
#define ADVANCEDTOPICS_MASTER_POINTIMPL_H

#include "Point.h"

// Represents a position in the board.
class PointImpl : public Point {
public:
    int x; // column
    int y; // row

public:
    PointImpl() : x(1), y(1) {}
    PointImpl(int x, int y) : x(x), y(y) {}
	//PointImpl(const Point& point) : x(point.getX()), y(point.getY()) {}

    virtual int getX() const { return this->x; }
    virtual int getY() const { return this->y; }

	// The L1 distance
	int DistanceInStepsFrom(PointImpl& other);
};


#endif //ADVANCEDTOPICS_MASTER_POINTIMPL_H
