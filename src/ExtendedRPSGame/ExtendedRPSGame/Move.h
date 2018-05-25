#ifndef __MOVE_H_
#define __MOVE_H_

//--------------------------
// Move Interface
//--------------------------

// Abstract class. Manages the return of a move on the board for the player.

// @author Hila Tsfadia, Jael Fafner TAU - Advanced Topics in Programming - 2018 Semester B

//--------------------------
// forward declaration
class Point;
//--------------------------

class Move {
public:
	virtual const Point& getFrom() const = 0;
	virtual const Point& getTo() const = 0;
	virtual ~Move() {}
};

#endif