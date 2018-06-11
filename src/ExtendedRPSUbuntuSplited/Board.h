#ifndef __BOARD_H_
#define __BOARD_H_

//--------------------------
// Board Interface
//--------------------------

// Abstract class. Generates and handles the board which the game is played upon.

// @author Hila Tsfadia, Jael Fafner TAU - Advanced Topics in Programming - 2018 Semester B

//--------------------------
// forward declaration
class Point;
//--------------------------

class Board {
public:
	virtual int getPlayer(const Point& pos) const = 0; // 1 for player 1’s piece, 2 for 2, 0 if empty
	virtual ~Board() {}
};

#endif