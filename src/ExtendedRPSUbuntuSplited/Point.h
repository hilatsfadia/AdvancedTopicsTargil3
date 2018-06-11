#ifndef __POINT_H_
#define __POINT_H_

//--------------------------
// Point Interface
//--------------------------

// Abstract class. Represents a certain coordinate on board. 

// @author Hila Tsfadia, Jael Fafner TAU - Advanced Topics in Programming - 2018 Semester B

class Point {
public:
	virtual int getX() const = 0;
	virtual int getY() const = 0;
	virtual ~Point() {}
};

#endif