
#ifndef ADVANCEDTOPICS_MASTER_FIGHTINFOIMPL_H
#define ADVANCEDTOPICS_MASTER_FIGHTINFOIMPL_H

//--------------------------
// FightInfoImpl Class
//--------------------------

// Manages a struct that holds all the information of a current 
// fight which was conducted on the board.

// @author Hila Tsfadia, Jael Fafner TAU - Advanced Topics in Programming - 2018 Semester B

#include "FightInfo.h"
#include "PointImpl.h"
#include <memory>

using std::unique_ptr;

class FightInfoImpl : public FightInfo{
private:
    unique_ptr<PointImpl> mPos = nullptr;
	// TODO: use NUM_OF_PLAYERS
    char mPlayersPieces[2];
    int mWinner;

public:
	FightInfoImpl() {}
	FightInfoImpl(const Point& position, char player1Piece, char player2Piece, int winner) : 
		mPlayersPieces{ player1Piece, player2Piece }, mWinner(winner) 
	{ mPos = std::make_unique<PointImpl>(position.getX(), position.getY()); }

	//-----------------------------------------------------------
	// FightInfo methods
	//-----------------------------------------------------------
    virtual const Point& getPosition() const override {return *mPos;}
	virtual char getPiece(int player) const { return mPlayersPieces[player - 1]; } // R, P, S, B or F (but NOT J)
    virtual int getWinner() const override {return mWinner;}// 0 - both lost / tie, 1 - player 1 won, 2 - player 2 won

	// Sets the empty fight info to contain information about a fight
	void SetFightInfoValues(const Point& position, char player1Piece, char player2Piece, int winner);

	// Return true iff theis object refer to a fight that occured
	bool isInitialized() const { return mPos != nullptr; }
};


#endif //ADVANCEDTOPICS_MASTER_FIGHTINFOIMPL_H
