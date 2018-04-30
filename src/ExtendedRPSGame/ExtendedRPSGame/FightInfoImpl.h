//
// Created by USER on 29/04/2018.
//

#ifndef ADVANCEDTOPICS_MASTER_FIGHTINFOIMPL_H
#define ADVANCEDTOPICS_MASTER_FIGHTINFOIMPL_H

#include "FightInfo.h"

class FightInfoImpl : FightInfo{
private:
    Point* mPos;
    char mOpponentPiece;
    int mWinner;

public:
    FightInfoImpl(Point* position, char opponentPiece, int winner): mPos(position), mOpponentPiece(opponentPiece), mWinner(winner){}
    virtual const Point& getPosition() const override {return *mPos;}
    virtual char getOpponentPiece() const override {return mOpponentPiece;}// R, P, S, B or F (but NOT J)
    virtual int getWinner() const override {return mWinner;}// 0 - both lost / tie, 1 - player 1 won, 2 - player 2 won
};


#endif //ADVANCEDTOPICS_MASTER_FIGHTINFOIMPL_H
