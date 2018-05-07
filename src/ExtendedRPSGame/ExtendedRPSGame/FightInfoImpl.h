//
// Created by USER on 29/04/2018.
//

#ifndef ADVANCEDTOPICS_MASTER_FIGHTINFOIMPL_H
#define ADVANCEDTOPICS_MASTER_FIGHTINFOIMPL_H

#include "FightInfo.h"

class FightInfoImpl : public FightInfo{
private:
    const Point* mPos;
	// TODO: use NUM_OF_PLAYERS
    char mPlayersPieces[2];
    int mWinner;

public:
	FightInfoImpl(const Point* position, char player1Piece, char player2Piece, int winner) : mPos(position), mPlayersPieces{ player1Piece, player2Piece }, mWinner(winner) {}
    virtual const Point& getPosition() const override {return *mPos;}
	virtual char getPiece(int player) const { return mPlayersPieces[player - 1]; } // R, P, S, B or F (but NOT J)
    virtual int getWinner() const override {return mWinner;}// 0 - both lost / tie, 1 - player 1 won, 2 - player 2 won
};


#endif //ADVANCEDTOPICS_MASTER_FIGHTINFOIMPL_H
