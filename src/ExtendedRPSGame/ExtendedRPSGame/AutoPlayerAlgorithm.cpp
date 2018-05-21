#include "stdafx.h"
#include "AutoPlayerAlgorithm.h"
#include "PiecePositionImpl.h"
#include "PointImpl.h"
#include "PieceFactory.h"
#include "Move.h"
#include "FightInfo.h"
#include "StrategyPiece.h"
#include "Flag.h"
#include "Paper.h"
#include "Rock.h"
#include "Scissors.h"
#include "Bomb.h"
#include "Joker.h"
#include "JokerChangeImpl.h"


void AutoPlayerAlgorithm::getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill)
{
	mPlayer = player; //setting the players fields
	if (mPlayer = 1) {
		mOpponent = 2;
	}
	else {
		mOpponent = 1;
	}
	mFlagPlaceKnown = false;
	mNumCoveredPieces = R + P + S + B + F + J;
	mNumMovablePieces = R + P + S + B + F + J;

	initMovesVector(player, vectorToFill);

	for (std::unique_ptr<PiecePosition>& piecePos : vectorToFill){
		Piece* uncoveredPiece = PieceFactory::GetPieceFromChar(piecePos->getPiece(), mPlayer);
		StrategyPiece* strategyPiece = new StrategyPiece(mPlayer, uncoveredPiece);
		mGameBoardInfo.GetBoardInPosition(piecePos->getPosition()).ChangeSquarePiece(strategyPiece);
	}

}


void AutoPlayerAlgorithm::initMovesVector(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill)
{
	if (player == 1)
	{
		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(PointImpl(1, 1), FLAG_CHAR));
		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(PointImpl(2, 1), JOKER_CHAR, BOMB_CHAR));
		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(PointImpl(1, 3), JOKER_CHAR, BOMB_CHAR));
		mJokerLocations.push_back(PointImpl(2, 1));
		mJokerLocations.push_back(PointImpl(1, 3));
		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(PointImpl(1, 2), BOMB_CHAR));
		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(PointImpl(2, 2), BOMB_CHAR));
		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(PointImpl(3, 6), SCISSORS_CHAR));
		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(PointImpl(4, 5), ROCK_CHAR));
		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(PointImpl(2, 5), ROCK_CHAR));
		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(PointImpl(4, 4), PAPER_CHAR));
		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(PointImpl(5, 3), PAPER_CHAR));
		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(PointImpl(3, 4), PAPER_CHAR));
		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(PointImpl(1, 4), PAPER_CHAR));
		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(PointImpl(2, 3), PAPER_CHAR));
	}
	else
	{
		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(PointImpl(10, 10), FLAG_CHAR));
		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(PointImpl(9, 10), JOKER_CHAR, BOMB_CHAR));
		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(PointImpl(10, 9), JOKER_CHAR, BOMB_CHAR));
		mJokerLocations.push_back(PointImpl(9, 10));
		mJokerLocations.push_back(PointImpl(10, 9));
		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(PointImpl(9, 9), BOMB_CHAR));
		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(PointImpl(9, 8), BOMB_CHAR));
		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(PointImpl(4, 7), SCISSORS_CHAR));
		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(PointImpl(7, 6), ROCK_CHAR));
		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(PointImpl(4, 5), ROCK_CHAR));
		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(PointImpl(6, 8), PAPER_CHAR));
		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(PointImpl(8, 8), PAPER_CHAR));
		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(PointImpl(8, 9), PAPER_CHAR));
		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(PointImpl(1, 7), PAPER_CHAR));
		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(PointImpl(8, 3), PAPER_CHAR));
	}
}

void AutoPlayerAlgorithm::notifyOnInitialBoard(const Board & b, const std::vector<unique_ptr<FightInfo>>& fights)
{
	for (int row = 1; row <=M; row++)
	{
		for (int col = 1; col <= N; col++) 
		{
			PointImpl pos = PointImpl(row, col); 
			if (b.getPlayer(pos) > 0 && b.getPlayer(pos) == mOpponent) {
				StrategyPiece* strategyPiece = new StrategyPiece(mOpponent); //uncovered piece
				mGameBoardInfo.GetBoardInPosition(col, row).ClearSquare(); //delets the former piece in square
				mGameBoardInfo.GetBoardInPosition(col,row).ChangeSquarePiece(strategyPiece);
			}
		}
	}
	for (const std::unique_ptr<FightInfo>& fight : fights) {
		updateSquareAfterFight(*fight);
	}
	
	findFlag();

	updateIsThreatened(mPlayer);
	updateIsThreatened(mOpponent);
	updateIsThreatening(mPlayer);
	updateIsThreatening(mOpponent);

	}

void AutoPlayerAlgorithm::updateSquareAfterFight(const FightInfo& fight) {
	Piece* piece = mGameBoardInfo.GetBoardInPosition(fight.getPosition()).GetPiece();
	StrategyPiece* strategyPiece = dynamic_cast<StrategyPiece*>(piece);

	//next exercise remember to add handling to more than one flag
	int owner = piece->GetOwnerNum();
	int winner = fight.getWinner();
	//TODO: add here, if pos!=piece of move - reveal joker.
	if (fight.getPiece(mPlayer) == JOKER_CHAR
		&& (winner == 0 || winner == mOpponent)) {
		eraseJokerLocation(fight.getPosition());
	}
	if (winner == 0) {
		mGameBoardInfo.GetBoardInPosition(fight.getPosition()).ClearSquare();
	}

	else if (owner != winner) {
		
		mGameBoardInfo.GetBoardInPosition(fight.getPosition()).ClearSquare();
		mGameBoardInfo.GetBoardInPosition(fight.getPosition()).ChangeSquarePiece(new StrategyPiece(winner, PieceFactory::GetPieceFromChar(fight.getPiece(winner), winner)));

	}
	else {
		
		if (piece->GetPieceType() == PieceFactory::PieceType::Unknown) {
			strategyPiece->UncoverPiece(fight.getPiece(mOpponent));
		}
		if (owner == mOpponent &&
			piece->GetPieceChar() !=
			fight.getPiece(mOpponent)) {
			mGameBoardInfo.GetBoardInPosition(fight.getPosition()).ClearSquare();
			Joker* joker = new Joker(mOpponent);
			joker->SetActualPieceType(PieceFactory::GetPieceFromChar(fight.getPiece(mOpponent), mOpponent));
			mGameBoardInfo.GetBoardInPosition(fight.getPosition()).ChangeSquarePiece(new StrategyPiece(mOpponent, joker));
		}
	}
	
	if (winner == mOpponent || winner == 0){
		mNumCoveredPieces--;
	} 

}

void AutoPlayerAlgorithm::notifyOnOpponentMove(const Move & move)
{
	movePieceOnInfoBoard(move); //TODO: FIX WHATS GETTING
	
	updateIsThreatened(mPlayer);
	updateIsThreatened(mOpponent);
	updateIsThreatening(mPlayer);
	updateIsThreatening(mOpponent);
}

void AutoPlayerAlgorithm::notifyFightResult(const FightInfo & fightInfo)
{
	updateSquareAfterFight(fightInfo);
	
	updateIsThreatened(mPlayer);
	updateIsThreatened(mOpponent);
	updateIsThreatening(mPlayer);
	updateIsThreatening(mOpponent);
}

unique_ptr<Move> AutoPlayerAlgorithm::getMove()
{
	findFlag();
	unique_ptr<Move> move;
	if (mFlagPlaceKnown == true) {
		move = conquerTheFlag();	//why would it give nullptr?
	} 
	else {
		move = getNormalMove();
	}
	if (mGameBoardInfo.GetBoardInPosition(move->getFrom()).GetPiece()->GetPieceType() == PieceFactory::PieceType::Joker) {
		updateJokerLocation(move->getFrom(), move->getTo());
	}

	movePieceOnInfoBoard(*move);
	
	updateIsThreatened(mPlayer);
	updateIsThreatened(mOpponent);
	updateIsThreatening(mPlayer);
	updateIsThreatening(mOpponent);
	

	return move;
}

void AutoPlayerAlgorithm::eraseJokerLocation(const Point& jokerPos) {
	std::vector<PointImpl>::iterator itr = mJokerLocations.begin();
	for (std::vector<PointImpl>::iterator itr = mJokerLocations.begin(); itr != mJokerLocations.end(); ) {
		if (jokerPos.getX() == itr->getX()
			&& jokerPos.getY() == itr->getY()) { //TODO:impl ==
			itr = mJokerLocations.erase(itr);
			break;
		}

		++itr;
	}
}

void AutoPlayerAlgorithm::updateJokerLocation(const Point& oldJokerPos, const Point& newJokerPos) {
	for (PointImpl jokerPos : mJokerLocations) {
		if (jokerPos.getX() == oldJokerPos.getX()
			&& jokerPos.getY() == oldJokerPos.getY()) { 
			jokerPos.setX(newJokerPos.getX());
			jokerPos.setY(newJokerPos.getY());
		}
	}
}

void AutoPlayerAlgorithm::updateIsThreatened(int player) {
	
	Piece* piece;
	StrategyPiece* strategyPiece;

	for (int row = 1; row <= M; row++)
	{
		for (int col = 1; col <= N; col++)
		{
			if (mGameBoardInfo.GetBoardInPosition(col, row).IsEmpty() == false) {
				piece = mGameBoardInfo.GetBoardInPosition(col, row).GetPiece();
				strategyPiece = dynamic_cast<StrategyPiece*>(piece);
				if (piece->GetOwnerNum() == player
					&& checkIsThreatened(col, row, player)) {
					strategyPiece->SetIsThreatened(true);
				}
				else {

					strategyPiece->SetIsThreatened(false);
				}
			}
		}
	}
}

void AutoPlayerAlgorithm::updateIsThreatening(int player) {
	Piece* piece;
	StrategyPiece* strategyPiece;

	for (int row = 1; row <= M; row++)
	{
		for (int col = 1; col <= N; col++)
		{
			if (mGameBoardInfo.GetBoardInPosition(col, row).IsEmpty() == false) {
				piece = mGameBoardInfo.GetBoardInPosition(col, row).GetPiece();
				strategyPiece = dynamic_cast<StrategyPiece*>(piece);

				if (piece->GetOwnerNum() == player
					&& checkIsThreatening(col, row, player)) {
					strategyPiece->SetIsThreathening(true);
				}
				else {
					strategyPiece->SetIsThreathening(false);
				}
			}
		}
	}
}

bool AutoPlayerAlgorithm::checkIsThreatening(int xPos, int yPos, int player) {
	bool isThreatening = false;
	int opponent = mOpponent;
	if (player == mOpponent) {
		opponent = mPlayer;
	}
	Piece* piece = mGameBoardInfo.GetBoardInPosition(xPos, yPos).GetPiece();
	StrategyPiece* strategyPiece = dynamic_cast<StrategyPiece*>(piece);

	if (strategyPiece != nullptr && strategyPiece->GetIsMovingPiece()) {
		if (mGameBoardInfo.GetBoardInPosition(xPos + 1, yPos).IsEmpty() == false
			&& mGameBoardInfo.GetBoardInPosition(xPos + 1, yPos).GetPiece()->GetOwnerNum() == opponent
			&& mGameBoardInfo.GetBoardInPosition(xPos, yPos).GetPiece()->IsStrongerThan(mGameBoardInfo.GetBoardInPosition(xPos + 1, yPos).GetPiece())) {
			isThreatening = true;
		}
		else if (mGameBoardInfo.GetBoardInPosition(xPos - 1, yPos).IsEmpty() == false
			&& mGameBoardInfo.GetBoardInPosition(xPos - 1, yPos).GetPiece()->GetOwnerNum() == opponent
			&& mGameBoardInfo.GetBoardInPosition(xPos, yPos).GetPiece()->IsStrongerThan(mGameBoardInfo.GetBoardInPosition(xPos - 1, yPos).GetPiece())) {
			isThreatening = true;
		}
		else if (mGameBoardInfo.GetBoardInPosition(xPos, yPos + 1).IsEmpty() == false
			&& mGameBoardInfo.GetBoardInPosition(xPos, yPos + 1).GetPiece()->GetOwnerNum() == opponent
			&& mGameBoardInfo.GetBoardInPosition(xPos, yPos).GetPiece()->IsStrongerThan(mGameBoardInfo.GetBoardInPosition(xPos, yPos + 1).GetPiece())) {
			isThreatening = true;
		}
		else if (mGameBoardInfo.GetBoardInPosition(xPos, yPos - 1).IsEmpty() == false
			&& mGameBoardInfo.GetBoardInPosition(xPos, yPos - 1).GetPiece()->GetOwnerNum() == opponent
			&& mGameBoardInfo.GetBoardInPosition(xPos, yPos).GetPiece()->IsStrongerThan(mGameBoardInfo.GetBoardInPosition(xPos, yPos - 1).GetPiece())) {
			isThreatening = true;
		}
	}
	return isThreatening;
}

void AutoPlayerAlgorithm::movePieceOnInfoBoard(const Move& move) {

	Piece* pieceToMove = mGameBoardInfo.GetBoardInPosition(move.getTo()).GetPiece();
	StrategyPiece* strategyPiece = dynamic_cast<StrategyPiece*>(pieceToMove);
	if (pieceToMove->GetOwnerNum() == mOpponent) {
		strategyPiece->SetIsMovingPiece(true);
		mNumMovablePieces--;
	}

	mGameBoardInfo.GetBoardInPosition(move.getTo()).ClearSquare(); 
	mGameBoardInfo.GetBoardInPosition(move.getTo()).ChangeSquarePiece(pieceToMove);

	mGameBoardInfo.GetBoardInPosition(move.getFrom()).ClearSquare();
}

PointImpl* AutoPlayerAlgorithm::conquerPiece(int xPos, int yPos) {
	
	if (xPos < N && mGameBoardInfo.GetBoardInPosition(xPos + 1, yPos).IsEmpty() == false
		&& mGameBoardInfo.GetBoardInPosition(xPos + 1, yPos).GetPiece()->GetOwnerNum() == mOpponent
		&& mGameBoardInfo.GetBoardInPosition(xPos, yPos).GetPiece()->IsStrongerThan(mGameBoardInfo.GetBoardInPosition(xPos+1, yPos).GetPiece())){
		return new PointImpl(xPos + 1, yPos);
	}
	else if (yPos < N && mGameBoardInfo.GetBoardInPosition(xPos, yPos + 1).IsEmpty() == false
		&& mGameBoardInfo.GetBoardInPosition(xPos, yPos + 1).GetPiece()->GetOwnerNum() == mOpponent
		&& mGameBoardInfo.GetBoardInPosition(xPos, yPos).GetPiece()->IsStrongerThan(mGameBoardInfo.GetBoardInPosition(xPos, yPos+1).GetPiece())){
		return new PointImpl(xPos, yPos + 1);
	}
	else if (xPos > 1 && mGameBoardInfo.GetBoardInPosition(xPos - 1, yPos).GetPiece()->GetOwnerNum() == mOpponent
		&& mGameBoardInfo.GetBoardInPosition(xPos, yPos).GetPiece()->IsStrongerThan(mGameBoardInfo.GetBoardInPosition(xPos - 1, yPos).GetPiece())){
		return new PointImpl(xPos - 1, yPos);
	}
	else if (yPos > 1 && mGameBoardInfo.GetBoardInPosition(xPos, yPos - 1).IsEmpty() == false
		&& mGameBoardInfo.GetBoardInPosition(xPos, yPos - 1).GetPiece()->GetOwnerNum() == mOpponent
		&& mGameBoardInfo.GetBoardInPosition(xPos, yPos).GetPiece()->IsStrongerThan(mGameBoardInfo.GetBoardInPosition(xPos, yPos-1).GetPiece())){
		return new PointImpl(xPos, yPos - 1);
	}
	return nullptr; 
}

PointImpl* AutoPlayerAlgorithm::getEmptySquareToMoveTo(PointImpl pos) {
	int xPos = pos.getX();
	int yPos = pos.getY();
	//need to do the same as hila did
	if (xPos < N && mGameBoardInfo.GetBoardInPosition(xPos + 1, yPos).IsEmpty() == true
		&& !checkIsThreatened(xPos + 1, yPos, mPlayer)) {
		return new PointImpl(xPos + 1, yPos);
	}
	else if (yPos < N && mGameBoardInfo.GetBoardInPosition(xPos, yPos + 1).IsEmpty() == true
		&& !checkIsThreatened(xPos, yPos + 1, mPlayer)) {
		return new PointImpl(xPos, yPos + 1);
	}
	else if (xPos > 1 && mGameBoardInfo.GetBoardInPosition(xPos - 1, yPos).IsEmpty() == true
		&& !checkIsThreatened(xPos - 1, yPos, mPlayer)) {
		return new PointImpl(xPos - 1, yPos);
	}
	else if (yPos > 1 && mGameBoardInfo.GetBoardInPosition(xPos, yPos - 1).IsEmpty() == true) {
		return new PointImpl(xPos, yPos - 1);
	}
	return nullptr;
}

PointImpl* AutoPlayerAlgorithm::runForYourLife(int xPos, int yPos) {

	if (xPos <N && mGameBoardInfo.GetBoardInPosition(xPos + 1, yPos).IsEmpty() == true
		&& !checkIsThreatened(xPos + 1, yPos, mPlayer)) {
		return new PointImpl(xPos + 1, yPos);
	}
	else if (yPos < N && mGameBoardInfo.GetBoardInPosition(xPos, yPos + 1).IsEmpty() == true
		&& !checkIsThreatened(xPos , yPos+1, mPlayer)) {
		return new PointImpl(xPos, yPos + 1);
	}
	else if (xPos > 1 && mGameBoardInfo.GetBoardInPosition(xPos - 1, yPos).IsEmpty() == true
		&& !checkIsThreatened(xPos - 1, yPos, mPlayer)) {
		return new PointImpl(xPos - 1, yPos);
	}
	else if (yPos > 1 && mGameBoardInfo.GetBoardInPosition(xPos, yPos - 1).IsEmpty() == true
		&& !checkIsThreatened(xPos, yPos-1, mPlayer)) {
		return new PointImpl(xPos, yPos - 1);
	}
	return nullptr; //how to get rid? null?
}

void AutoPlayerAlgorithm::findFlag() {
	Piece* piece;
	StrategyPiece* strategyPiece;
	if (mNumCoveredPieces == F 
		|| mNumCoveredPieces - mNumMovablePieces == F) { //TODO: double check that everyone is by default false
		for (int row = 1; row <= M; row++)
		{
			for (int col = 1; col <= N; col++)
			{
				if (mGameBoardInfo.GetBoardInPosition(col, row).IsEmpty() == false) {
					piece = mGameBoardInfo.GetBoardInPosition(col, row).GetPiece();
					strategyPiece = dynamic_cast<StrategyPiece*>(piece);

					if (piece->GetPieceType() ==
						PieceFactory::PieceType::Unknown
						&& !strategyPiece->GetIsMovingPiece()) {
						strategyPiece->UncoverPiece(FLAG_CHAR);
						mNumCoveredPieces--;
						mNumMovablePieces--;
						mOpponentFlagLocations.push_back(PointImpl(col, row));
					}
				}
			}

		}
		mFlagPlaceKnown = true;
	}
}

unique_ptr<Move> AutoPlayerAlgorithm::saveAPiece() {
	Piece* piece;
	StrategyPiece* strategyPiece;

	for (int row = 1; row <= M; row++)
	{
		for (int col = 1; col <= N; col++)
		{
			PointImpl posFrom(row, col);
			if (mGameBoardInfo.GetBoardInPosition(col, row).IsEmpty() == false) {
				piece = mGameBoardInfo.GetBoardInPosition(col, row).GetPiece();
				strategyPiece = dynamic_cast<StrategyPiece*>(piece);

				if (strategyPiece->GetIsMovingPiece()
					&& piece->GetOwnerNum() == mPlayer
					&& strategyPiece->GetIsThreatened() == true) {
					PointImpl* posTo = runForYourLife(col, row);
					if (posTo != nullptr) {
						return std::make_unique<MoveImpl>(&posFrom, posTo);
					}
				}
			}
		}	

	}
	return nullptr;
}

unique_ptr<Move> AutoPlayerAlgorithm::eatOpponentPiece() {
	Piece* piece;
	StrategyPiece* strategyPiece;

	for (int row = 1; row <= M; row++)
	{
		for (int col = 1; col <= N; col++)
		{
			PointImpl posFrom(row, col);
			if (mGameBoardInfo.GetBoardInPosition(col, row).IsEmpty() == false) {
				piece = mGameBoardInfo.GetBoardInPosition(col, row).GetPiece();
				strategyPiece = dynamic_cast<StrategyPiece*>(piece);

				if (strategyPiece->GetIsMovingPiece()
					&& piece->GetOwnerNum() == mPlayer
					&& strategyPiece->GetIsThreathening() == true) {
					PointImpl* posTo = conquerPiece(col, row);
					if (posTo != nullptr) {
						return std::make_unique<MoveImpl>(&posFrom, posTo); //problem with pointers or points
					}
				}
			}
		}

	}
	return nullptr;
}

unique_ptr<Move> AutoPlayerAlgorithm::getNormalMove() {
	unique_ptr<Move> move;

	move = saveAPiece();

	if (move != nullptr) {
		return move;
	}

	move = eatOpponentPiece();

	if (move != nullptr) {
		return move;
	}

	for (int row = 1; row <= M; row++)
	{
		for (int col = 1; col <= N; col++)
		{
			PointImpl pos(col, row);
			//bool d = mGameBoardInfo.GetBoardInPosition(col, row).IsEmpty();
			//if (!d)
			//{
			//	bool k = mGameBoardInfo.GetBoardInPosition(col, row).GetPiece()->GetOwnerNum() == mPlayer;
			//	bool e = mGameBoardInfo.GetBoardInPosition(col, row).GetPiece()->GetIsMovingPiece();
			//	int g = 5;
			//}

			if (!mGameBoardInfo.GetBoardInPosition(col, row).IsEmpty()
				&& mGameBoardInfo.GetBoardInPosition(col, row).GetPiece()->GetOwnerNum() == mPlayer
				&& mGameBoardInfo.GetBoardInPosition(col, row).GetPiece()->GetIsMovingPiece()) {
				move = std::make_unique<MoveImpl>(&pos, getEmptySquareToMoveTo(pos));
				if (move != nullptr) {
					return move;
				}
				
			}
		}
	}
	return nullptr;
}

unique_ptr<Move> AutoPlayerAlgorithm::conquerTheFlag() {
	unique_ptr<Move> move(saveAPiece());
	if (move != nullptr) {
		return move;
	}
	std::vector<unique_ptr<PointImpl>> posVector;
	PointImpl* moveTo;
	for (const PointImpl& flagPoint : mOpponentFlagLocations) {
		for (int d = 0; d < N - 1 + M - 1; d++) {
			getVectorToMoveToFlag(flagPoint, d, posVector); //TODO:is it emptyed everytime?
			if (posVector.size() != 0) {
				for (unique_ptr<PointImpl>& pos : posVector) {
					moveTo = getPlaceTowardsFlag(*pos, flagPoint, true);
					if (moveTo != nullptr) {
						return std::make_unique<MoveImpl>(pos.get(), moveTo);
					}
				}
			}

		}
	}
	PointImpl* moveClosest;
	for (PointImpl& flagPoint : mOpponentFlagLocations) {
		for (int d = 0; d < N - 1 + M - 1; d++) {
			getVectorToMoveToFlag(flagPoint, d, posVector);
			if (posVector.size() != 0) {
				moveClosest = getPlaceTowardsFlag(*posVector[0], flagPoint, false);
				if (moveClosest != nullptr) {
					return std::make_unique<MoveImpl>(posVector[0].get(), moveClosest);
				}
			}
		}
	}		
	return nullptr;
}

void AutoPlayerAlgorithm::getVectorToMoveToFlag(const PointImpl &flag_pos, int distance, std::vector<unique_ptr<PointImpl>>& posVectorToFill) {
	int currDistance;
	for (int row = 1; row <= M; row++)
	{
		for (int col = 1; col <= N; col++)
		{

			if (mGameBoardInfo.GetBoardInPosition(col, row).IsEmpty() == false
				&& mGameBoardInfo.GetBoardInPosition(col, row).GetPiece()->GetOwnerNum() == mPlayer
				&& mGameBoardInfo.GetBoardInPosition(col, row).GetPiece()->GetIsMovingPiece() == true) {
				PointImpl point(col, row);
				currDistance = point.DistanceInStepsFrom(flag_pos);
				if (currDistance == distance) {
					posVectorToFill.push_back(std::make_unique<PointImpl>(col, row));
				}
			}
				
			}
		}
	}

PointImpl* AutoPlayerAlgorithm::GetMoveHorizotally(PointImpl &moveFrom, const PointImpl &flagPos)
{
	if (moveFrom.getX() > flagPos.getX()) {
		return new PointImpl(moveFrom.getX() - 1, moveFrom.getY());
	}
	else if (moveFrom.getX() < flagPos.getX()) {
		return new PointImpl(moveFrom.getX() + 1, moveFrom.getY());
	}

	return new PointImpl(moveFrom.getX(), moveFrom.getY());
}

PointImpl* AutoPlayerAlgorithm::GetMoveVertically(PointImpl &moveFrom, const PointImpl &flagPos)
{
	if (moveFrom.getY() > flagPos.getY()) {
		return new PointImpl(moveFrom.getX(), moveFrom.getY()-1);
	}
	else if (moveFrom.getY() < flagPos.getY()) {
		return new PointImpl(moveFrom.getX(), moveFrom.getY() +1);
	}

	return nullptr;
}

PointImpl* AutoPlayerAlgorithm::getPlaceTowardsFlag(PointImpl &moveFrom, const PointImpl &flagPos, bool ifToCheckThreatened) {

	PointImpl* moveTo = GetMoveHorizotally(moveFrom, flagPos);

	if (ifToCheckThreatened)
	{
		// TODO: unique ptr
		if ((moveTo != nullptr && checkIsThreatened(moveTo->getX(), moveTo->getY(), mPlayer)) || (moveTo == nullptr)) {
			moveTo = GetMoveVertically(moveFrom, flagPos);
		}

		if (moveTo != nullptr && checkIsThreatened(moveTo->getX(), moveTo->getY(), mPlayer))
		{
			return nullptr;
		}
	}
	else
	{
		if (moveTo == nullptr) {
			moveTo = GetMoveVertically(moveFrom, flagPos);
		}
	}

	return moveTo;
}

bool AutoPlayerAlgorithm::checkIsThreatened(int xPos, int yPos, int player) {
	int opponent = mOpponent;	

	if (player == mOpponent) {
		opponent = mPlayer;
	}

	bool isThreatened = false;
	if (xPos < N)
	{
		isThreatened = isThreatened || returnThreatened(xPos, yPos, xPos + 1, yPos, opponent);
	}
	
	if (xPos > 1)
	{
		isThreatened = isThreatened || returnThreatened(xPos, yPos, xPos - 1, yPos, opponent);
	}

	if (yPos < M)
	{
		isThreatened = isThreatened || returnThreatened(xPos, yPos, xPos, yPos + 1, opponent);
	}

	if (yPos > 1)
	{
		isThreatened = isThreatened || returnThreatened(xPos, yPos, xPos, yPos - 1, opponent);
	}

	return isThreatened;
}

bool AutoPlayerAlgorithm::returnThreatened(int xPos, int yPos, int newxPos, int newyPos, int opponent) {
	
	if (!mGameBoardInfo.GetBoardInPosition(newxPos, newyPos).IsEmpty()) {
		Piece* piece = mGameBoardInfo.GetBoardInPosition(newxPos, newyPos).GetPiece();
		StrategyPiece* strategyPiece = dynamic_cast<StrategyPiece*>(piece);

		if (piece == false
			&& piece->GetOwnerNum() == opponent
			&& strategyPiece->GetIsMovingPiece() == true
			&& piece->IsStrongerThan(mGameBoardInfo.GetBoardInPosition(xPos, yPos).GetPiece())) { //TODO: WILL WORK?
			return true;
		}
	}
	return false;
}

unique_ptr<JokerChange> AutoPlayerAlgorithm::getJokerChange()
{
	char newRep;
	for (PointImpl jokerPos : mJokerLocations) {
		Piece* piece = mGameBoardInfo.GetBoardInPosition(jokerPos).GetPiece();
		StrategyPiece* strategyPiece = dynamic_cast<StrategyPiece*>(piece);
		if (strategyPiece->GetIsThreatened()) {
			std::vector<PointImpl> posVector;
			getVectorThreateningPieces(jokerPos, posVector);
			if (posVector.size() != 0) {
				for (PointImpl &pos : posVector) {
					newRep = strategyPiece->GetStronger(mGameBoardInfo.GetBoardInPosition(pos).GetPiece());
					if (newRep != COVERED_CHAR) {
						Joker* joker = new Joker(mPlayer);
						joker->SetActualPieceType(PieceFactory::GetPieceFromChar(newRep, mOpponent));
						mGameBoardInfo.GetBoardInPosition(jokerPos).ChangeSquarePiece(new StrategyPiece(mPlayer, joker));

						updateIsThreatened(mPlayer);
						updateIsThreatened(mOpponent);
						updateIsThreatening(mPlayer);
						updateIsThreatening(mOpponent);
						return std::make_unique<JokerChangeImpl>(jokerPos, newRep); //TODO: piece->getStrongerPiece())
					}
				}
			}
				

		}
	}
	return nullptr;
}

bool AutoPlayerAlgorithm::returnPointThreatening(int xpos, int ypos, int newxpos, int newypos) {
	if (mGameBoardInfo.GetBoardInPosition(newxpos, newypos).IsEmpty() == false) {
		Piece* piece = mGameBoardInfo.GetBoardInPosition(newxpos, newypos).GetPiece();
		StrategyPiece* strategyPiece = dynamic_cast<StrategyPiece*>(piece);
		
		if (piece->GetOwnerNum() == mOpponent
			&& strategyPiece->GetIsMovingPiece() == true
			&& piece->IsStrongerThan(mGameBoardInfo.GetBoardInPosition(xpos, ypos).GetPiece())) //todo: impl operator
		{
			return true;
		}
	}
	return false;
}

void AutoPlayerAlgorithm::getVectorThreateningPieces(PointImpl &pos , std::vector<PointImpl>& posVectorToFill) {
	int xPos = pos.getX();
	int yPos = pos.getY();
	if (returnPointThreatening(xPos, yPos, xPos + 1, yPos)) {	
		posVectorToFill.push_back(PointImpl(xPos + 1, yPos));
	}
	if (returnPointThreatening(xPos, yPos, xPos - 1, yPos)) {
		posVectorToFill.push_back(PointImpl(xPos - 1, yPos));
	}
	if (returnPointThreatening(xPos, yPos, xPos, yPos + 1)) {
		posVectorToFill.push_back(PointImpl(xPos, yPos + 1));
	}
	if (returnPointThreatening(xPos, yPos, xPos, yPos - 1)) {
		posVectorToFill.push_back(PointImpl(xPos, yPos - 1));
	}
}

//char AutoPlayerAlgorithm::getCharFromPieceType(PieceFactory::PieceType type) { 
//	switch(type)
//	{
//		case (PieceFactory::PieceType::Rock):
//		{
//			return ROCK_CHAR;
//		}
//
//		case (PieceFactory::PieceType::Paper):
//		{
//			return PAPER_CHAR;
//		}
//		case (PieceFactory::PieceType::Scissors):
//		{
//			return SCISSORS_CHAR;
//		}
//		default:
//			return 0; //like this?
//
//	}
//
//
//}