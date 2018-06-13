#ifndef ADVANCEDTOPICS_MASTER_POINTIMPL_H
#define ADVANCEDTOPICS_MASTER_POINTIMPL_H

//--------------------------
// PointImpl Class
//--------------------------

// Represents a certain coordinate on board.

// @author Hila Tsfadia, Jael Fafner TAU - Advanced Topics in Programming - 2018 Semester B

#include "Point.h"

// Represents a position in the board.
class PointImpl : public Point {
public:
    int x; // column
    int y; // row

public:
    PointImpl() : x(1), y(1) {}
	PointImpl(const Point& point) : PointImpl(point.getX(), point.getY()) {}
    PointImpl(int x, int y) : x(x), y(y) {}

    virtual int getX() const { return this->x; }
    virtual int getY() const { return this->y; }
	void setX(int x) { this->x = x; }
	void setY(int y) { this->y = y; }
/*
	friend bool operator==(const PointImpl& lhs, const PointImpl& rhs)
	{
		return ((lhs.getX() == rhs.getX()) && (lhs.getY() == rhs.getY()));
	}

	friend inline bool operator!=(const PointImpl& lhs, const PointImpl& rhs) { return !(lhs == rhs); }*/

	// The L1 distance
	int DistanceInStepsFrom(const PointImpl& other) const;
};


#endif //ADVANCEDTOPICS_MASTER_POINTIMPL_H
