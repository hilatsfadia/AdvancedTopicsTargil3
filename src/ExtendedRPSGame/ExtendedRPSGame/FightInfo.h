#ifndef __FIGHT_INFO_H_
#define __FIGHT_INFO_H_

//--------------------------
// FightInfo Interface
//--------------------------

// Abstract class. Manages a struct that holds all the information of a current 
// fight which was conducted on the board.

// @author Hila Tsfadia, Jael Fafner TAU - Advanced Topics in Programming - 2018 Semester B

//--------------------------
// forward declaration
class Point;
//--------------------------

class FightInfo {
public:
	virtual const Point& getPosition() const = 0;
	virtual char getPiece(int player) const = 0; // R, P, S, B or F (but NOT J)
	virtual int getWinner() const = 0; // 0 - both lost / tie, 1 - player 1 won, 2 - player 2 won
	virtual ~FightInfo() {}
};

#endif