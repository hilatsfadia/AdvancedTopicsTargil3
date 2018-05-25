#ifndef __JOKER_CHANGE_H_
#define __JOKER_CHANGE_H_

//--------------------------
// JokerChange Interface
//--------------------------

// Abstract class. Manages the return of a joker change move on the board for the player.

// @author Hila Tsfadia, Jael Fafner TAU - Advanced Topics in Programming - 2018 Semester B

//--------------------------
// forward declaration
class Point;
//--------------------------

class JokerChange {
public:
	virtual const Point& getJokerChangePosition() const = 0;
	virtual char getJokerNewRep() const = 0; // R, P, S or B (but NOT J and NOT F)
	virtual ~JokerChange() {}
};

#endif