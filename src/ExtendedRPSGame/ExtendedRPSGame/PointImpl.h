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

    virtual int getX() const { return this->x; }
    virtual int getY() const { return this->y; }
	void setX(int x) { this->x = x; }
	void setY(int y) { this->y = y; }

	friend inline bool operator==(const PointImpl& lhs, const PointImpl& rhs);
	friend inline bool operator!=(const PointImpl& lhs, const PointImpl& rhs) { return !(lhs == rhs); }

	// The L1 distance
	int DistanceInStepsFrom(const PointImpl& other) const;
};


#endif //ADVANCEDTOPICS_MASTER_POINTIMPL_H
