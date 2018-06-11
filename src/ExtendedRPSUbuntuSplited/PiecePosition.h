#ifndef __PIECE_POSITION_H_
#define __PIECE_POSITION_H_

//--------------------------
// PiecePosition Interface
//--------------------------

// Abstract class. Manages a certain position on board given to it.

// @author Hila Tsfadia, Jael Fafner TAU - Advanced Topics in Programming - 2018 Semester B

//--------------------------
// forward declaration
class Point;
//--------------------------

class PiecePosition {
public:
	virtual const Point& getPosition() const = 0;
	virtual char getPiece() const = 0; // R, P, S, B, J or F
	virtual char getJokerRep() const = 0; // ONLY for Joker: R, P, S or B -- non-Joker may return ‘#’
	virtual ~PiecePosition() {}
};

#endif