#ifndef ADVANCEDTOPICS_MASTER_PIECEPOSITIONIMPL_H
#define ADVANCEDTOPICS_MASTER_PIECEPOSITIONIMPL_H

#include "PointImpl.h"
#include "PiecePosition.h"

#define NON_JOKER_JOKER_REP '#'

class PiecePositionImpl : public PiecePosition {
    PointImpl mPosition;
    char mPiece;
    char mJokerRps;

public:
    PiecePositionImpl(PointImpl pos,  char piece, char jokerRps = NON_JOKER_JOKER_REP) : mPosition(pos), mPiece(piece), mJokerRps(jokerRps){}
    virtual const Point& getPosition() const { return mPosition; }
    virtual char getPiece() const  { return mPiece; } // R, P, S, B, J or F
    virtual char getJokerRep() const { return mJokerRps; }// ONLY for Joker: R, P, S or B -- non-Joker may return '#'
};

#endif //ADVANCEDTOPICS_MASTER_PIECEPOSITIONIMPL_H
