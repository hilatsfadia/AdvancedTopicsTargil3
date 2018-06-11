#ifndef ADVANCEDTOPICS_MASTER_PIECEPOSITIONIMPL_H
#define ADVANCEDTOPICS_MASTER_PIECEPOSITIONIMPL_H

//--------------------------
// PiecePositionImpl Interface
//--------------------------

// Manages a certain position on board given to it.
// @author Hila Tsfadia, Jael Fafner TAU - Advanced Topics in Programming - 2018 Semester B

#include "PointImpl.h"
#include "PiecePosition.h"

#define NON_JOKER_REP '#'

class PiecePositionImpl : public PiecePosition {
    PointImpl mPosition;
    char mPiece;
    char mJokerRps;

public:
    PiecePositionImpl(const PointImpl& pos, char piece, char jokerRps = NON_JOKER_REP) : 
		mPosition(pos), mPiece(piece), mJokerRps(jokerRps) {}

    virtual const Point& getPosition() const { return mPosition; }
    virtual char getPiece() const  { return mPiece; } // R, P, S, B, J or F
    virtual char getJokerRep() const { return mJokerRps; }// ONLY for Joker: R, P, S or B -- non-Joker may return '#'
};

#endif //ADVANCEDTOPICS_MASTER_PIECEPOSITIONIMPL_H
