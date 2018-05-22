#include "stdafx.h"
//#include "AutoPlayerAlgorithm.h"
//#include "PiecePositionImpl.h"
//#include "PointImpl.h"
//#include "PieceFactory.h"
//#include "Move.h"
//#include "FightInfo.h"
//#include "StrategyPiece.h"
//#include "Flag.h"
//#include "Paper.h"
//#include "Rock.h"
//#include "Scissors.h"
//#include "Bomb.h"
//#include "Joker.h"
//#include "JokerChangeImpl.h"
//#include <cstdlib>
//#include <ctime>
//
//
//void AutoPlayerAlgorithm::initMovesVector(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill)
//{
//	int yPos = (player - 1) * (M / 2) + 1;
//	// TODO: delete!
//	//if (player == 2)
//	//{
//	//	yPos--;
//	//}
//	PointImpl point;
//	int xPos = 1;
//	//int yPos = 1;
//
//	for (int flag = 0; flag < F; flag++) {
//		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(PointImpl(xPos, yPos), FLAG_CHAR));
//		if (xPos == N) {
//			xPos = 1;
//			yPos++;
//		}
//		else
//		{
//			xPos++;
//		}
//	}
//	//if (yPos != M) {
//	//	yPos++;
//	//	xPos = 1;
//	//}
//	for (int bomb = 0; bomb < B; bomb++) {
//		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(PointImpl(xPos, yPos), BOMB_CHAR));
//
//		if (xPos == N) {
//			xPos = 1;
//			yPos++;
//		}
//		else
//		{
//			xPos++;
//		}
//	}
//
//	for (int joker = 0; joker < J; joker++) {
//		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(PointImpl(xPos, yPos), JOKER_CHAR, ROCK_CHAR));
//		mJokerLocations.push_back(PointImpl(xPos, yPos));
//
//		if (xPos == N) {
//			xPos = 1;
//			yPos++;
//		}
//		else
//		{
//			xPos++;
//		}
//	}
//
//	for (int rock = 0; rock < R; rock++) {
//
//		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(PointImpl(xPos, yPos), ROCK_CHAR));
//		if (xPos == N) {
//			xPos = 1;
//			yPos++;
//		}
//		else
//		{
//			xPos++;
//		}
//	}
//
//	for (int paper = 0; paper < P; paper++) {
//
//		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(PointImpl(xPos, yPos), PAPER_CHAR));
//		if (xPos == N) {
//			xPos = 1;
//			yPos++;
//		}
//		else
//		{
//			xPos++;
//		}
//	}
//
//	for (int scissors = 0; scissors < S; scissors++) {
//
//		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(PointImpl(xPos, yPos), SCISSORS_CHAR));
//		if (xPos == N) {
//			xPos = 1;
//			yPos++;
//		}
//		else
//		{
//			xPos++;
//		}
//	}
//
//}
//
////bool AutoPlayerAlgorithm::pointExists(PointImpl point, std::vector<unique_ptr<PiecePosition>>& vectorToFill) {
////	for (unique_ptr<PiecePosition>& piece : vectorToFill) {
////		if (piece->getPosition().getX() == point.getX()
////			&& piece->getPosition().getY() == point.getY()) {
////			return true;
////		}
////	}
////	return false;
////}
////
////PointImpl AutoPlayerAlgorithm::generateRandomPoint() {
////	int range = N;
////	int rnd_x = 1 + ( % range);
////	int rnd_y = 1 + ( % range);
////
////	return PointImpl(rnd_x, rnd_y);
////}
//
//
//void AutoPlayerAlgorithm::getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill)
//{
//	mPlayer = player; //setting the players fields
//	if (mPlayer == 1) {
//		mOpponent = 2;
//	}
//	else {
//		mOpponent = 1;
//	}
//	mFlagPlaceKnown = false;
//	mNumCoveredPieces = R + P + S + B + F + J;
//	mNumMovablePieces = 0;
//
//	initMovesVector(player, vectorToFill);
//
//	for (std::unique_ptr<PiecePosition>& piecePos : vectorToFill){
//		Piece* uncoveredPiece = nullptr;
//		if (piecePos->getPiece() == JOKER_CHAR)
//		{
//			Joker* jokerPiece = new Joker(mPlayer);
//			jokerPiece->SetActualPieceType(PieceFactory::GetPieceFromChar(piecePos->getJokerRep(), mPlayer));
//			uncoveredPiece = jokerPiece;
//		}
//		else
//		{
//			uncoveredPiece = PieceFactory::GetPieceFromChar(piecePos->getPiece(), mPlayer);
//		}
//		StrategyPiece* strategyPiece = new StrategyPiece(mPlayer, uncoveredPiece);
//		mGameBoardInfo.GetBoardInPosition(piecePos->getPosition()).ChangeSquarePiece(strategyPiece);
//	}
//
//}
//
//void AutoPlayerAlgorithm::notifyOnInitialBoard(const Board & b, const std::vector<unique_ptr<FightInfo>>& fights)
//{
//	for (int row = 1; row <=M; row++)
//	{
//		for (int col = 1; col <= N; col++) 
//		{
//			PointImpl pos = PointImpl(col, row); 
//			if (b.getPlayer(pos) == mOpponent) {
//				StrategyPiece* strategyPiece = new StrategyPiece(mOpponent); //uncovered piece
//				mGameBoardInfo.GetBoardInPosition(col, row).ClearSquare(); //delets the former piece in square
//				mGameBoardInfo.GetBoardInPosition(col,row).ChangeSquarePiece(strategyPiece);
//			}
//		}
//	}
//	for (const std::unique_ptr<FightInfo>& fight : fights) {
//		updateSquareAfterFight(*fight);
//	}
//	
//	findFlag();
//
//	updateIsThreatened(mPlayer);
//	updateIsThreatened(mOpponent);
//	updateIsThreatening(mPlayer);
//	updateIsThreatening(mOpponent);
//
//	}
//
//void AutoPlayerAlgorithm::updateSquareAfterFight(const FightInfo& fight) {
//	Piece* piece = mGameBoardInfo.GetBoardInPosition(fight.getPosition()).GetPiece();
//	StrategyPiece* strategyPiece = dynamic_cast<StrategyPiece*>(piece);
//
//	//next exercise remember to add handling to more than one flag
//	//int owner = piece->GetOwnerNum();
//	int winner = fight.getWinner();
//
//	if (winner == 0) {
//		if (mGameBoardInfo.GetBoardInPosition(fight.getPosition()).GetPiece()->GetPieceChar() == JOKER_CHAR) {
//			eraseJokerLocation(fight.getPosition());
//		}
//		mGameBoardInfo.GetBoardInPosition(fight.getPosition()).ClearSquare();
//	}
//	else if (winner == mOpponent) {
//		if (mGameBoardInfo.GetBoardInPosition(fight.getPosition()).GetPiece()->GetPieceChar() == JOKER_CHAR) {
//			eraseJokerLocation(fight.getPosition());
//		}
//		mGameBoardInfo.GetBoardInPosition(fight.getPosition()).ClearSquare();
//		mGameBoardInfo.GetBoardInPosition(fight.getPosition()).ChangeSquarePiece(new StrategyPiece(winner, PieceFactory::GetPieceFromChar(fight.getPiece(winner), winner)));
//		//cant remember movable here. maybe need update?
//	}
//	//else if (owner != winner) {
//	//	
//	//	
//
//	//}
//	//else {
//	//	
//	//	
//	//	if (owner == mOpponent &&
//	//		piece->GetPieceChar() !=
//	//		fight.getPiece(mOpponent)) {
//	//		mGameBoardInfo.GetBoardInPosition(fight.getPosition()).ClearSquare();
//	//		Joker* joker = new Joker(mOpponent);
//	//		joker->SetActualPieceType(PieceFactory::GetPieceFromChar(fight.getPiece(mOpponent), mOpponent));
//	//		mGameBoardInfo.GetBoardInPosition(fight.getPosition()).ChangeSquarePiece(new StrategyPiece(mOpponent, joker));
//	//	}
//	//}
//	
//	if (winner == mOpponent || winner == 0){
//		mNumCoveredPieces--;
//	} 
//
//}
//
//void AutoPlayerAlgorithm::notifyOnOpponentMove(const Move & move)
//{
//	movePieceOnInfoBoard(move); //TODO: FIX WHATS GETTING
//	
//	updateIsThreatened(mPlayer);
//	updateIsThreatened(mOpponent);
//	updateIsThreatening(mPlayer);
//	updateIsThreatening(mOpponent);
//}
//
//void AutoPlayerAlgorithm::notifyFightResult(const FightInfo & fightInfo)
//{
//	updateSquareAfterFight(fightInfo);
//	
//	updateIsThreatened(mPlayer);
//	updateIsThreatened(mOpponent);
//	updateIsThreatening(mPlayer);
//	updateIsThreatening(mOpponent);
//}
//
//unique_ptr<Move> AutoPlayerAlgorithm::getMove()
//{
//	findFlag();
//	unique_ptr<Move> move;
//	//if (mFlagPlaceKnown == true) {
//	//	move = conquerTheFlag();	//why would it give nullptr?
//	//} 
//	//else {
//	move = getNormalMove();
//	//}
//	if (mGameBoardInfo.GetBoardInPosition(move->getFrom()).GetPiece()->GetPieceType() == PieceFactory::PieceType::Joker) {
//		updateJokerLocation(move->getFrom(), move->getTo());
//	}
//
//	movePieceOnInfoBoard(*move);
//	
//	updateIsThreatened(mPlayer);
//	updateIsThreatened(mOpponent);
//	updateIsThreatening(mPlayer);
//	updateIsThreatening(mOpponent);
//	
//
//	return move;
//}
//
//void AutoPlayerAlgorithm::eraseJokerLocation(const Point& jokerPos) {
//	std::vector<PointImpl>::iterator itr = mJokerLocations.begin();
//	for (std::vector<PointImpl>::iterator itr = mJokerLocations.begin(); itr != mJokerLocations.end(); ) {
//		if (jokerPos.getX() == itr->getX()
//			&& jokerPos.getY() == itr->getY()) { //TODO:impl ==
//			itr = mJokerLocations.erase(itr);
//			break;
//		}
//
//		++itr;
//	}
//}
//
//void AutoPlayerAlgorithm::updateJokerLocation(const Point& oldJokerPos, const Point& newJokerPos) {
//	for (PointImpl jokerPos : mJokerLocations) {
//		if (jokerPos.getX() == oldJokerPos.getX()
//			&& jokerPos.getY() == oldJokerPos.getY()) { 
//			jokerPos.setX(newJokerPos.getX());
//			jokerPos.setY(newJokerPos.getY());
//		}
//	}
//}
//
//void AutoPlayerAlgorithm::updateIsThreatened(int player) {
//	
//	Piece* piece;
//	StrategyPiece* strategyPiece;
//
//	for (int row = 1; row <= M; row++)
//	{
//		for (int col = 1; col <= N; col++)
//		{
//			if (mGameBoardInfo.GetBoardInPosition(col, row).IsEmpty() == false) {
//				piece = mGameBoardInfo.GetBoardInPosition(col, row).GetPiece();
//				strategyPiece = dynamic_cast<StrategyPiece*>(piece);
//				if (piece->GetOwnerNum() == player
//					&& checkIsThreatened(col, row, player)) {
//					strategyPiece->SetIsThreatened(true);
//				}
//				else {
//
//					strategyPiece->SetIsThreatened(false);
//				}
//			}
//		}
//	}
//}
//
//void AutoPlayerAlgorithm::updateIsThreatening(int player) {
//	Piece* piece;
//	StrategyPiece* strategyPiece;
//
//	for (int row = 1; row <= M; row++)
//	{
//		for (int col = 1; col <= N; col++)
//		{
//			if (mGameBoardInfo.GetBoardInPosition(col, row).IsEmpty() == false) {
//				piece = mGameBoardInfo.GetBoardInPosition(col, row).GetPiece();
//				strategyPiece = dynamic_cast<StrategyPiece*>(piece);
//
//				if (piece->GetOwnerNum() == player
//					&& checkIsThreatening(col, row, player)) {
//					strategyPiece->SetIsThreathening(true);
//				}
//				else {
//					strategyPiece->SetIsThreathening(false);
//				}
//			}
//		}
//	}
//}
//
//bool AutoPlayerAlgorithm::checkIsThreatening(int xPos, int yPos, int player) {
//	bool isThreatening = false;
//	int opponent = mOpponent;
//	if (player == mOpponent) {
//		opponent = mPlayer;
//	}
//	Piece* piece = mGameBoardInfo.GetBoardInPosition(xPos, yPos).GetPiece();
//	StrategyPiece* strategyPiece = dynamic_cast<StrategyPiece*>(piece);
//
//	if (strategyPiece != nullptr && strategyPiece->GetIsMovingPiece()) {
//		if (xPos < N && mGameBoardInfo.GetBoardInPosition(xPos + 1, yPos).IsEmpty() == false
//			&& mGameBoardInfo.GetBoardInPosition(xPos + 1, yPos).GetPiece()->GetOwnerNum() == opponent
//			&& mGameBoardInfo.GetBoardInPosition(xPos, yPos).GetPiece()->IsStrongerThan(mGameBoardInfo.GetBoardInPosition(xPos + 1, yPos).GetPiece())) {
//			isThreatening = true;
//		}
//		else if (xPos > 1 && mGameBoardInfo.GetBoardInPosition(xPos - 1, yPos).IsEmpty() == false
//			&& mGameBoardInfo.GetBoardInPosition(xPos - 1, yPos).GetPiece()->GetOwnerNum() == opponent
//			&& mGameBoardInfo.GetBoardInPosition(xPos, yPos).GetPiece()->IsStrongerThan(mGameBoardInfo.GetBoardInPosition(xPos - 1, yPos).GetPiece())) {
//			isThreatening = true;
//		}
//		else if (yPos < M && mGameBoardInfo.GetBoardInPosition(xPos, yPos + 1).IsEmpty() == false
//			&& mGameBoardInfo.GetBoardInPosition(xPos, yPos + 1).GetPiece()->GetOwnerNum() == opponent
//			&& mGameBoardInfo.GetBoardInPosition(xPos, yPos).GetPiece()->IsStrongerThan(mGameBoardInfo.GetBoardInPosition(xPos, yPos + 1).GetPiece())) {
//			isThreatening = true;
//		}
//		else if (yPos > 1 && mGameBoardInfo.GetBoardInPosition(xPos, yPos - 1).IsEmpty() == false
//			&& mGameBoardInfo.GetBoardInPosition(xPos, yPos - 1).GetPiece()->GetOwnerNum() == opponent
//			&& mGameBoardInfo.GetBoardInPosition(xPos, yPos).GetPiece()->IsStrongerThan(mGameBoardInfo.GetBoardInPosition(xPos, yPos - 1).GetPiece())) {
//			isThreatening = true;
//		}
//	}
//	return isThreatening;
//}
//
//void AutoPlayerAlgorithm::movePieceOnInfoBoard(const Move& move) {
//	// TODO: delete!
//	//BoardImpl::BoardSquare& boardSquare = mGameBoardInfo.GetBoardInPosition(move.getFrom());
//	Piece* pieceToMove = mGameBoardInfo.GetBoardInPosition(move.getFrom()).GetPiece();
//	StrategyPiece* strategyPiece = dynamic_cast<StrategyPiece*>(pieceToMove);
//	if (pieceToMove->GetOwnerNum() == mOpponent) {
//		strategyPiece->SetIsMovingPiece(true);
//		mNumMovablePieces++;
//	}
//	if (mGameBoardInfo.GetBoardInPosition(move.getTo()).IsEmpty()) {
//		mGameBoardInfo.GetBoardInPosition(move.getTo()).ChangeSquarePiece(pieceToMove);
//	}
//	if (pieceToMove->GetOwnerNum() == mPlayer && !mGameBoardInfo.GetBoardInPosition(move.getTo()).IsEmpty()) {
//		mGameBoardInfo.GetBoardInPosition(move.getTo()).ClearSquare();
//		mGameBoardInfo.GetBoardInPosition(move.getTo()).ChangeSquarePiece(pieceToMove);
//	}
//	
//	mGameBoardInfo.GetBoardInPosition(move.getFrom()).ClearSquare();
//}
//
//PointImpl* AutoPlayerAlgorithm::conquerPiece(int xPos, int yPos) {
//	
//	if (xPos < N && mGameBoardInfo.GetBoardInPosition(xPos + 1, yPos).IsEmpty() == false
//		&& mGameBoardInfo.GetBoardInPosition(xPos + 1, yPos).GetPiece()->GetOwnerNum() == mOpponent
//		&& mGameBoardInfo.GetBoardInPosition(xPos, yPos).GetPiece()->IsStrongerThan(mGameBoardInfo.GetBoardInPosition(xPos+1, yPos).GetPiece())){
//		return new PointImpl(xPos + 1, yPos);
//	}
//	else if (yPos < N && mGameBoardInfo.GetBoardInPosition(xPos, yPos + 1).IsEmpty() == false
//		&& mGameBoardInfo.GetBoardInPosition(xPos, yPos + 1).GetPiece()->GetOwnerNum() == mOpponent
//		&& mGameBoardInfo.GetBoardInPosition(xPos, yPos).GetPiece()->IsStrongerThan(mGameBoardInfo.GetBoardInPosition(xPos, yPos+1).GetPiece())){
//		return new PointImpl(xPos, yPos + 1);
//	}
//	else if (xPos > 1 && mGameBoardInfo.GetBoardInPosition(xPos - 1, yPos).GetPiece()->GetOwnerNum() == mOpponent
//		&& mGameBoardInfo.GetBoardInPosition(xPos, yPos).GetPiece()->IsStrongerThan(mGameBoardInfo.GetBoardInPosition(xPos - 1, yPos).GetPiece())){
//		return new PointImpl(xPos - 1, yPos);
//	}
//	else if (yPos > 1 && mGameBoardInfo.GetBoardInPosition(xPos, yPos - 1).IsEmpty() == false
//		&& mGameBoardInfo.GetBoardInPosition(xPos, yPos - 1).GetPiece()->GetOwnerNum() == mOpponent
//		&& mGameBoardInfo.GetBoardInPosition(xPos, yPos).GetPiece()->IsStrongerThan(mGameBoardInfo.GetBoardInPosition(xPos, yPos-1).GetPiece())){
//		return new PointImpl(xPos, yPos - 1);
//	}
//	return nullptr; 
//}
//
//PointImpl* AutoPlayerAlgorithm::getEmptySquareToMoveTo(PointImpl pos) {
//	int xPos = pos.getX();
//	int yPos = pos.getY();
//	//need to do the same as hila did
//	if (xPos < N && mGameBoardInfo.GetBoardInPosition(xPos + 1, yPos).IsEmpty() == true
//		&& !checkIsThreatened(xPos + 1, yPos, mPlayer)) {
//		return new PointImpl(xPos + 1, yPos);
//	}
//	else if (yPos < N && mGameBoardInfo.GetBoardInPosition(xPos, yPos + 1).IsEmpty() == true
//		&& !checkIsThreatened(xPos, yPos + 1, mPlayer)) {
//		return new PointImpl(xPos, yPos + 1);
//	}
//	else if (xPos > 1 && mGameBoardInfo.GetBoardInPosition(xPos - 1, yPos).IsEmpty() == true
//		&& !checkIsThreatened(xPos - 1, yPos, mPlayer)) {
//		return new PointImpl(xPos - 1, yPos);
//	}
//	else if (yPos > 1 && mGameBoardInfo.GetBoardInPosition(xPos, yPos - 1).IsEmpty() == true) {
//		return new PointImpl(xPos, yPos - 1);
//	}
//	return nullptr;
//}
//
//PointImpl* AutoPlayerAlgorithm::runForYourLife(int xPos, int yPos) {
//
//	if (xPos <N && mGameBoardInfo.GetBoardInPosition(xPos + 1, yPos).IsEmpty() == true
//		&& !checkIsThreatened(xPos + 1, yPos, mPlayer)) {
//		return new PointImpl(xPos + 1, yPos);
//	}
//	else if (yPos < N && mGameBoardInfo.GetBoardInPosition(xPos, yPos + 1).IsEmpty() == true
//		&& !checkIsThreatened(xPos , yPos+1, mPlayer)) {
//		return new PointImpl(xPos, yPos + 1);
//	}
//	else if (xPos > 1 && mGameBoardInfo.GetBoardInPosition(xPos - 1, yPos).IsEmpty() == true
//		&& !checkIsThreatened(xPos - 1, yPos, mPlayer)) {
//		return new PointImpl(xPos - 1, yPos);
//	}
//	else if (yPos > 1 && mGameBoardInfo.GetBoardInPosition(xPos, yPos - 1).IsEmpty() == true
//		&& !checkIsThreatened(xPos, yPos-1, mPlayer)) {
//		return new PointImpl(xPos, yPos - 1);
//	}
//	return nullptr; //how to get rid? null?
//}
//
//void AutoPlayerAlgorithm::findFlag() {
//	Piece* piece;
//	StrategyPiece* strategyPiece;
//	if (mNumCoveredPieces == F 
//		|| mNumCoveredPieces - mNumMovablePieces == F) { //TODO: double check that everyone is by default false
//		for (int row = 1; row <= M; row++)
//		{
//			for (int col = 1; col <= N; col++)
//			{
//				if (mGameBoardInfo.GetBoardInPosition(col, row).IsEmpty() == false) {
//					piece = mGameBoardInfo.GetBoardInPosition(col, row).GetPiece();
//					strategyPiece = dynamic_cast<StrategyPiece*>(piece);
//
//					if (piece->GetPieceType() ==
//						PieceFactory::PieceType::Unknown
//						&& !strategyPiece->GetIsMovingPiece()) {
//						strategyPiece->UncoverPiece(FLAG_CHAR);
//						mNumCoveredPieces--;
//						mOpponentFlagLocations.push_back(PointImpl(col, row));
//					}
//				}
//			}
//
//		}
//		mFlagPlaceKnown = true;
//	}
//}
//
//unique_ptr<Move> AutoPlayerAlgorithm::saveAPiece() {
//	Piece* piece;
//	StrategyPiece* strategyPiece;
//
//	for (int row = 1; row <= M; row++)
//	{
//		for (int col = 1; col <= N; col++)
//		{
//			PointImpl posFrom(row, col);
//			if (mGameBoardInfo.GetBoardInPosition(col, row).IsEmpty() == false) {
//				piece = mGameBoardInfo.GetBoardInPosition(col, row).GetPiece();
//				strategyPiece = dynamic_cast<StrategyPiece*>(piece);
//
//				if (strategyPiece->GetIsMovingPiece()
//					&& piece->GetOwnerNum() == mPlayer
//					&& strategyPiece->GetIsThreatened() == true) {
//					PointImpl* posTo = runForYourLife(col, row);
//					if (posTo != nullptr) {
//						return std::make_unique<MoveImpl>(posFrom, *posTo);
//					}
//				}
//			}
//		}	
//
//	}
//	return nullptr;
//}
//
//unique_ptr<Move> AutoPlayerAlgorithm::eatOpponentPiece() {
//	Piece* piece;
//	StrategyPiece* strategyPiece;
//
//	for (int row = 1; row <= M; row++)
//	{
//		for (int col = 1; col <= N; col++)
//		{
//			PointImpl posFrom(row, col);
//			if (mGameBoardInfo.GetBoardInPosition(col, row).IsEmpty() == false) {
//				piece = mGameBoardInfo.GetBoardInPosition(col, row).GetPiece();
//				strategyPiece = dynamic_cast<StrategyPiece*>(piece);
//
//				if (strategyPiece->GetIsMovingPiece()
//					&& piece->GetOwnerNum() == mPlayer
//					&& strategyPiece->GetIsThreathening() == true) {
//					PointImpl* posTo = conquerPiece(col, row);
//					if (posTo != nullptr) {
//						return std::make_unique<MoveImpl>(posFrom, *posTo); //problem with pointers or points
//					}
//				}
//			}
//		}
//
//	}
//	return nullptr;
//}
//
//unique_ptr<Move> AutoPlayerAlgorithm::getNormalMove() {
//	unique_ptr<Move> move;
//
//	/*move = saveAPiece();
//
//	if (move != nullptr) {
//		return move;
//	}
//
//	move = eatOpponentPiece();
//
//	if (move != nullptr) {
//		return move;
//	}
//*/
//	for (int row = 1; row <= M; row++)
//	{
//		for (int col = 1; col <= N; col++)
//		{
//			PointImpl pos(col, row);
//			//bool d = mGameBoardInfo.GetBoardInPosition(col, row).IsEmpty();
//			//if (!d)
//			//{
//			//	bool k = mGameBoardInfo.GetBoardInPosition(col, row).GetPiece()->GetOwnerNum() == mPlayer;
//			//	bool e = mGameBoardInfo.GetBoardInPosition(col, row).GetPiece()->GetIsMovingPiece();
//			//	int g = 5;
//			//}
//
//			// TODO: delete!!!
//			//Piece* stam = mGameBoardInfo.GetBoardInPosition(col, row).GetPiece();
//			if (!mGameBoardInfo.GetBoardInPosition(col, row).IsEmpty()
//				&& mGameBoardInfo.GetBoardInPosition(col, row).GetPiece()->GetOwnerNum() == mPlayer
//				&& mGameBoardInfo.GetBoardInPosition(col, row).GetPiece()->GetIsMovingPiece()) {
//				move = std::make_unique<MoveImpl>(pos, *getEmptySquareToMoveTo(pos));
//				if (move != nullptr) {
//					return move;
//				}
//				
//			}
//		}
//	}
//	return nullptr;
//}
//
//unique_ptr<Move> AutoPlayerAlgorithm::conquerTheFlag() {
//	unique_ptr<Move> move(saveAPiece());
//	if (move != nullptr) {
//		return move;
//	}
//	std::vector<unique_ptr<PointImpl>> posVector;
//	PointImpl* moveTo;
//	for (const PointImpl& flagPoint : mOpponentFlagLocations) {
//		for (int d = 0; d < N - 1 + M - 1; d++) {
//			getVectorToMoveToFlag(flagPoint, d, posVector); //TODO:is it emptyed everytime?
//			if (posVector.size() != 0) {
//				for (unique_ptr<PointImpl>& pos : posVector) {
//					moveTo = getPlaceTowardsFlag(*pos, flagPoint, true);
//					if (moveTo != nullptr) {
//						return std::make_unique<MoveImpl>(*pos, *moveTo);
//					}
//				}
//			}
//
//		}
//	}
//
//	PointImpl* moveClosest;
//	for (PointImpl& flagPoint : mOpponentFlagLocations) {
//		for (int d = 0; d < N - 1 + M - 1; d++) {
//			getVectorToMoveToFlag(flagPoint, d, posVector);
//			if (posVector.size() != 0) {
//				moveClosest = getPlaceTowardsFlag(*posVector[0], flagPoint, false);
//				if (moveClosest != nullptr) {
//					return std::make_unique<MoveImpl>(*posVector[0], *moveClosest);
//				}
//			}
//		}
//	}		
//	return nullptr;
//}
//
//void AutoPlayerAlgorithm::getVectorToMoveToFlag(const PointImpl &flag_pos, int distance, std::vector<unique_ptr<PointImpl>>& posVectorToFill) {
//	int currDistance;
//	for (int row = 1; row <= M; row++)
//	{
//		for (int col = 1; col <= N; col++)
//		{
//
//			if (mGameBoardInfo.GetBoardInPosition(col, row).IsEmpty() == false
//				&& mGameBoardInfo.GetBoardInPosition(col, row).GetPiece()->GetOwnerNum() == mPlayer
//				&& mGameBoardInfo.GetBoardInPosition(col, row).GetPiece()->GetIsMovingPiece() == true) {
//				PointImpl point(col, row);
//				currDistance = point.DistanceInStepsFrom(flag_pos);
//				if (currDistance == distance) {
//					posVectorToFill.push_back(std::make_unique<PointImpl>(col, row));
//				}
//			}
//				
//			}
//		}
//	}
//
//PointImpl* AutoPlayerAlgorithm::GetMoveHorizotally(PointImpl &moveFrom, const PointImpl &flagPos)
//{
//	if (moveFrom.getX() > flagPos.getX()) {
//		return new PointImpl(moveFrom.getX() - 1, moveFrom.getY());
//	}
//	else if (moveFrom.getX() < flagPos.getX()) {
//		return new PointImpl(moveFrom.getX() + 1, moveFrom.getY());
//	}
//
//	return new PointImpl(moveFrom.getX(), moveFrom.getY());
//}
//
//PointImpl* AutoPlayerAlgorithm::GetMoveVertically(PointImpl &moveFrom, const PointImpl &flagPos)
//{
//	if (moveFrom.getY() > flagPos.getY()) {
//		return new PointImpl(moveFrom.getX(), moveFrom.getY()-1);
//	}
//	else if (moveFrom.getY() < flagPos.getY()) {
//		return new PointImpl(moveFrom.getX(), moveFrom.getY() +1);
//	}
//
//	return nullptr;
//}
//
//PointImpl* AutoPlayerAlgorithm::getPlaceTowardsFlag(PointImpl &moveFrom, const PointImpl &flagPos, bool ifToCheckThreatened) {
//
//	PointImpl* moveTo = GetMoveHorizotally(moveFrom, flagPos);
//
//	if (ifToCheckThreatened)
//	{
//		// TODO: unique ptr
//		if ((moveTo != nullptr && checkIsThreatened(moveTo->getX(), moveTo->getY(), mPlayer)) || (moveTo == nullptr)) {
//			moveTo = GetMoveVertically(moveFrom, flagPos);
//		}
//
//		if (moveTo != nullptr && checkIsThreatened(moveTo->getX(), moveTo->getY(), mPlayer))
//		{
//			return nullptr;
//		}
//	}
//	else
//	{
//		if (moveTo == nullptr) {
//			moveTo = GetMoveVertically(moveFrom, flagPos);
//		}
//	}
//
//	return moveTo;
//}
//
//bool AutoPlayerAlgorithm::checkIsThreatened(int xPos, int yPos, int player) {
//	int opponent = mOpponent;	
//
//	if (player == mOpponent) {
//		opponent = mPlayer;
//	}
//
//	bool isThreatened = false;
//	if (xPos < N)
//	{
//		isThreatened = isThreatened || returnThreatened(xPos, yPos, xPos + 1, yPos, opponent);
//	}
//	
//	if (xPos > 1)
//	{
//		isThreatened = isThreatened || returnThreatened(xPos, yPos, xPos - 1, yPos, opponent);
//	}
//
//	if (yPos < M)
//	{
//		isThreatened = isThreatened || returnThreatened(xPos, yPos, xPos, yPos + 1, opponent);
//	}
//
//	if (yPos > 1)
//	{
//		isThreatened = isThreatened || returnThreatened(xPos, yPos, xPos, yPos - 1, opponent);
//	}
//
//	return isThreatened;
//}
//
//bool AutoPlayerAlgorithm::returnThreatened(int xPos, int yPos, int newxPos, int newyPos, int opponent) {
//	
//	if (!mGameBoardInfo.GetBoardInPosition(newxPos, newyPos).IsEmpty()) {
//		Piece* piece = mGameBoardInfo.GetBoardInPosition(newxPos, newyPos).GetPiece();
//		StrategyPiece* strategyPiece = dynamic_cast<StrategyPiece*>(piece);
//
//		if (piece == false
//			&& piece->GetOwnerNum() == opponent
//			&& strategyPiece->GetIsMovingPiece() == true
//			&& piece->IsStrongerThan(mGameBoardInfo.GetBoardInPosition(xPos, yPos).GetPiece())) { //TODO: WILL WORK?
//			return true;
//		}
//	}
//	return false;
//}
//
//unique_ptr<JokerChange> AutoPlayerAlgorithm::getJokerChange()
//{
//	char newRep;
//	for (PointImpl jokerPos : mJokerLocations) {
//		Piece* piece = mGameBoardInfo.GetBoardInPosition(jokerPos).GetPiece();
//		StrategyPiece* strategyPiece = dynamic_cast<StrategyPiece*>(piece);
//		if (strategyPiece->GetIsThreatened()) {
//			std::vector<PointImpl> posVector;
//			getVectorThreateningPieces(jokerPos, posVector);
//			if (posVector.size() != 0) {
//				for (PointImpl &pos : posVector) {
//					newRep = strategyPiece->GetStronger(mGameBoardInfo.GetBoardInPosition(pos).GetPiece());
//					if (newRep != COVERED_CHAR) {
//						Joker* joker = new Joker(mPlayer);
//						joker->SetActualPieceType(PieceFactory::GetPieceFromChar(newRep, mOpponent));
//						mGameBoardInfo.GetBoardInPosition(jokerPos).ChangeSquarePiece(new StrategyPiece(mPlayer, joker));
//
//						updateIsThreatened(mPlayer);
//						updateIsThreatened(mOpponent);
//						updateIsThreatening(mPlayer);
//						updateIsThreatening(mOpponent);
//						return std::make_unique<JokerChangeImpl>(jokerPos, newRep); //TODO: piece->getStrongerPiece())
//					}
//				}
//			}
//				
//
//		}
//	}
//	return nullptr;
//}
//
//bool AutoPlayerAlgorithm::returnPointThreatening(int xpos, int ypos, int newxpos, int newypos) {
//	if (mGameBoardInfo.GetBoardInPosition(newxpos, newypos).IsEmpty() == false) {
//		Piece* piece = mGameBoardInfo.GetBoardInPosition(newxpos, newypos).GetPiece();
//		StrategyPiece* strategyPiece = dynamic_cast<StrategyPiece*>(piece);
//		
//		if (piece->GetOwnerNum() == mOpponent
//			&& strategyPiece->GetIsMovingPiece() == true
//			&& piece->IsStrongerThan(mGameBoardInfo.GetBoardInPosition(xpos, ypos).GetPiece())) //todo: impl operator
//		{
//			return true;
//		}
//	}
//	return false;
//}
//
//void AutoPlayerAlgorithm::getVectorThreateningPieces(PointImpl &pos , std::vector<PointImpl>& posVectorToFill) {
//	int xPos = pos.getX();
//	int yPos = pos.getY();
//	if (returnPointThreatening(xPos, yPos, xPos + 1, yPos)) {	
//		posVectorToFill.push_back(PointImpl(xPos + 1, yPos));
//	}
//	if (returnPointThreatening(xPos, yPos, xPos - 1, yPos)) {
//		posVectorToFill.push_back(PointImpl(xPos - 1, yPos));
//	}
//	if (returnPointThreatening(xPos, yPos, xPos, yPos + 1)) {
//		posVectorToFill.push_back(PointImpl(xPos, yPos + 1));
//	}
//	if (returnPointThreatening(xPos, yPos, xPos, yPos - 1)) {
//		posVectorToFill.push_back(PointImpl(xPos, yPos - 1));
//	}
//}
//
////char AutoPlayerAlgorithm::getCharFromPieceType(PieceFactory::PieceType type) { 
////	switch(type)
////	{
////		case (PieceFactory::PieceType::Rock):
////		{
////			return ROCK_CHAR;
////		}
////
////		case (PieceFactory::PieceType::Paper):
////		{
////			return PAPER_CHAR;
////		}
////		case (PieceFactory::PieceType::Scissors):
////		{
////			return SCISSORS_CHAR;
////		}
////		default:
////			return 0; //like this?
////
////	}
////
////
////}