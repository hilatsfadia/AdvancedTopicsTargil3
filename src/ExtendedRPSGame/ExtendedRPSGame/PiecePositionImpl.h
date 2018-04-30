//
// Created by USER on 29/04/2018.
//

#ifndef ADVANCEDTOPICS_MASTER_PIECEPOSITIONIMPL_H
#define ADVANCEDTOPICS_MASTER_PIECEPOSITIONIMPL_H
#include "Point.h"
#include "PiecePosition.h"

class PiecePositionImpl : public PiecePosition {
    Point* mPosition;
    char mPiece;
    char mJokerRps;

public:
    PiecePositionImpl(Point* pos,  char piece, char jokerRps) : mPosition(pos), mPiece(piece), mJokerRps(jokerRps){}
    virtual const Point& getPosition() const { return *mPosition; }
    virtual char getPiece() const  { return mPiece; } // R, P, S, B, J or F
    virtual char getJokerRep() const { return mJokerRps; }// ONLY for Joker: R, P, S or B -- non-Joker may return �#�
};


#endif //ADVANCEDTOPICS_MASTER_PIECEPOSITIONIMPL_H
