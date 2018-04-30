//
// Created by USER on 29/04/2018.
//

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
};


#endif //ADVANCEDTOPICS_MASTER_POINTIMPL_H
