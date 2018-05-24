#include "stdafx.h"
#include "AutoPlayerAlgorithm.h"
#include "PiecePositionImpl.h"
#include "PointImpl.h"
#include "PieceFactory.h"
#include "Move.h"
#include "FightInfo.h"
#include "Flag.h"
#include "Paper.h"
#include "Rock.h"
#include "Scissors.h"
#include "Bomb.h"
#include "Joker.h"
#include "JokerChangeImpl.h"
#include <cstdlib>
#include <ctime>

using std::make_unique;


void AutoPlayerAlgorithm::UpdateLineNumber(int & yPos, bool isToMoveForward)
{
	if (isToMoveForward)
	{
		yPos++;
	}
	else
	{
		yPos--;
	}
}

void AutoPlayerAlgorithm::initPositionsVectorOneType(std::vector<unique_ptr<PiecePosition>>& vectorToFill, int& xPos, int& yPos, bool isToMoveForward, int count, char typeChar, char jokerReper)
{
	for (int i = 0; i < count; i++) {
		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(PointImpl(xPos, yPos), typeChar, jokerReper));
		if (xPos == N) {
			xPos = 1;
			UpdateLineNumber(yPos, isToMoveForward);
		}
		else
		{
			xPos++;
		}
	}
	//if (yPos != M) {
	//	yPos++;
	//	xPos = 1;
	//}
}

void AutoPlayerAlgorithm::initPositionsVector(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill)
{
	PointImpl point;
	int xPos = 1;
	//int yPos = 1;
	//int yPos = (player - 1) * (M / 2) + 1;
	//bool isToMoveForward = true;
	int yPos = (player == 1) ? 1 : M;
	bool isToMoveForward = (player == 1) ? true : false;

	initPositionsVectorOneType(vectorToFill, xPos, yPos, isToMoveForward, F, FLAG_CHAR);

	if (B > 0)
	{
		initPositionsVectorOneType(vectorToFill, xPos, yPos, isToMoveForward, 1, BOMB_CHAR);
		xPos = 1;
		UpdateLineNumber(yPos, isToMoveForward);
		initPositionsVectorOneType(vectorToFill, xPos, yPos, isToMoveForward, B - 1, BOMB_CHAR);
	}

	initPositionsVectorOneType(vectorToFill, xPos, yPos, isToMoveForward, J, JOKER_CHAR, BOMB_CHAR);
	initPositionsVectorOneType(vectorToFill, xPos, yPos, isToMoveForward, R, ROCK_CHAR);
	initPositionsVectorOneType(vectorToFill, xPos, yPos, isToMoveForward, P, PAPER_CHAR);
	initPositionsVectorOneType(vectorToFill, xPos, yPos, isToMoveForward, S, SCISSORS_CHAR);
}

void AutoPlayerAlgorithm::initTheAlgorithmPlayerBoard(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill)
{
	for (std::unique_ptr<PiecePosition>& piecePos : vectorToFill) {
		unique_ptr<Piece> uncoveredPiece = nullptr;
		if (piecePos->getPiece() == JOKER_CHAR)
		{
			unique_ptr<Joker> jokerPiece = make_unique<Joker>(mPlayer);
			jokerPiece->SetActualPiece(PieceFactory::GetPieceFromChar(piecePos->getJokerRep(), mPlayer));
			uncoveredPiece = std::move(jokerPiece);
		}
		else
		{
			uncoveredPiece = PieceFactory::GetPieceFromChar(piecePos->getPiece(), mPlayer);
		}

		unique_ptr<StrategyPiece> strategyPiece = make_unique<StrategyPiece>(mPlayer, std::move(uncoveredPiece));
		mPlayersStrategyBoards[player - 1].PutPieceInPosition(piecePos->getPosition(), std::move(strategyPiece));
	}
}

void AutoPlayerAlgorithm::getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill)
{
	mPlayer = player; //setting the players fields
	if (mPlayer == 1) {
		mOpponent = 2;
	}
	else {
		mOpponent = 1;
	}

	mNumCoveredPieces = R + P + S + B + F + J;
	mNumMovablePieces = 0;

	initPositionsVector(player, vectorToFill);
	initTheAlgorithmPlayerBoard(player, vectorToFill);
}

void AutoPlayerAlgorithm::ClearPlayersBoardsInPosition(const Point& pos)
{
	mPlayersStrategyBoards[mPlayer - 1].ClearBoardInPosition(pos);
	mPlayersStrategyBoards[mOpponent - 1].ClearBoardInPosition(pos);
}

void AutoPlayerAlgorithm::updateStrategyAccordingToBoard(const Board & b)
{
	for (int row = 1; row <= M; row++)
	{
		for (int col = 1; col <= N; col++)
		{
			PointImpl pos = PointImpl(col, row);

			// Update the board of the player of this algorithm, by removing
			// eaten pieces.
			if (b.getPlayer(pos) != mPlayer) // it's mOpponent or 0
			{
				if (!mPlayersStrategyBoards[mPlayer - 1].IsEmptyInPosition(pos))
				{
					mPlayersStrategyBoards[mPlayer - 1].ClearBoardInPosition(pos);
				}
			}

			// Update the board of the oponnent player, by getting all it's pieces locations
			if (b.getPlayer(pos) == mOpponent)
			{
				unique_ptr<StrategyPiece> strategyPiece = make_unique<StrategyPiece>(mOpponent); //uncovered piece
				mPlayersStrategyBoards[mOpponent - 1].ClearBoardInPosition(pos); //delets the former piece in square
				mPlayersStrategyBoards[mOpponent - 1].PutPieceInPosition(pos, std::move(strategyPiece));
			}
		}
	}
}

void AutoPlayerAlgorithm::updateStrategyAccordingToFight(const FightInfo& fight)
{
	//next exercise remember to add handling to more than one flag
	int winner = fight.getWinner();

	if (winner == TIE) {
		//if (mGameBoardInfo.GetBoardInPosition(fight.getPosition()).GetPiece()->GetPieceChar() == JOKER_CHAR) {
		//	eraseJokerLocation(fight.getPosition());
		//}
		ClearPlayersBoardsInPosition(fight.getPosition());
	}
	else if (winner == mOpponent) {
		//if (mGameBoardInfo.GetBoardInPosition(fight.getPosition()).GetPiece()->GetPieceChar() == JOKER_CHAR) {
		//	eraseJokerLocation(fight.getPosition());
		//}
		mPlayersStrategyBoards[mPlayer - 1].ClearBoardInPosition(fight.getPosition()); // already done by the supplied board?
		mPlayersStrategyBoards[mOpponent - 1].PeekPieceInPosition(fight.getPosition()).UncoverPiece(fight.getPiece(winner));
	}
	else // winner == mPlayer
	{
		mPlayersStrategyBoards[mOpponent - 1].ClearBoardInPosition(fight.getPosition());
	}

	if (winner == mOpponent || winner == TIE){
		mNumCoveredPieces--;
	} 
}

void AutoPlayerAlgorithm::findOpponentFlag() 
{
	if (mNumCoveredPieces == F || (mNumCoveredPieces - mNumMovablePieces == F)) 
	{ //TODO: double check that everyone is by default false
		for (int row = 1; row <= M; row++)
		{
			for (int col = 1; col <= N; col++)
			{
				if (!mPlayersStrategyBoards[mOpponent - 1].IsEmptyInPosition(col, row)) 
				{
					StrategyPiece& piece = mPlayersStrategyBoards[mOpponent - 1].PeekPieceInPosition(col, row);

					if ((piece.GetPieceType() == PieceFactory::PieceType::Unknown)
						&& (!piece.GetIsMovingPiece())) // TODO: ask why is it needed
					{
						piece.UncoverPiece(FLAG_CHAR);
						mNumCoveredPieces--;
						mOpponentFlagLocations.push_back(PointImpl(col, row));
					}
				}
			}
		}
	}
}

void AutoPlayerAlgorithm::notifyOnInitialBoard(const Board & b, const std::vector<unique_ptr<FightInfo>>& fights)
{
	updateStrategyAccordingToBoard(b);

	for (const std::unique_ptr<FightInfo>& fight : fights)
	{
		updateStrategyAccordingToFight(*fight);
	}

	findOpponentFlag();

	updateThreats();
}

bool AutoPlayerAlgorithm::AreBothBoardsEmptyInPosition(int x, int y) const
{
	return (mPlayersStrategyBoards[mPlayer - 1].IsEmptyInPosition(x, y) &&
		mPlayersStrategyBoards[mOpponent - 1].IsEmptyInPosition(x, y));
}

bool AutoPlayerAlgorithm::AreBothBoardsEmptyInPosition(const Point& pos) const
{
	return AreBothBoardsEmptyInPosition(pos.getX(), pos.getY());
}

void AutoPlayerAlgorithm::FillAdjacentLegalPositions(const PointImpl& pos, std::vector<unique_ptr<PointImpl>>& vectorToFill) const
{
	int xPos = pos.getX();
	int yPos = pos.getY();
	PointImpl options[NUM_OF_ADJACENT_POSITIONS] =
	{ PointImpl(xPos + 1, yPos) , PointImpl(xPos, yPos + 1), PointImpl(xPos - 1, yPos) , PointImpl(xPos, yPos - 1) };

	for (int i = 0; i < NUM_OF_ADJACENT_POSITIONS; i++)
	{
		if (BoardImpl<StrategyPiece>::CheckIfValidPosition(options[i]))
		{
			vectorToFill.push_back(make_unique<PointImpl>(options[i]));
		}
	}
}

unique_ptr<PointImpl> AutoPlayerAlgorithm::getEmptyThreatFreeSquareToMoveTo(const PointImpl& from) {
	std::vector<unique_ptr<PointImpl>> adjacentLegalPositions;
	FillAdjacentLegalPositions(from, adjacentLegalPositions);
	Piece& piece = mPlayersStrategyBoards[mPlayer - 1].PeekPieceInPosition(from);
	int currX, currY;

	for (const unique_ptr<PointImpl>& pos : adjacentLegalPositions)
	{
		currX = pos->getX();
		currY = pos->getY();

		if (BoardImpl<StrategyPiece>::CheckIfValidPosition(*pos)
			&& AreBothBoardsEmptyInPosition(*pos)
			&& (!isThreatenedInPosition(piece, *pos)))
		{
			return make_unique<PointImpl>(currX, currY);
		}
	}

	return nullptr;
}

unique_ptr<Move> AutoPlayerAlgorithm::getNormalMove() {
	unique_ptr<Move> move;

	/*move = saveAPiece();

	if (move != nullptr) {
		return move;
	}

	move = eatOpponentPiece();

	if (move != nullptr) {
		return move;
	}
*/
	for (int row = 1; row <= M; row++)
	{
		for (int col = 1; col <= N; col++)
		{
			PointImpl pos(col, row);

			if ((!mPlayersStrategyBoards[mPlayer - 1].IsEmptyInPosition(pos))
				&& (mPlayersStrategyBoards[mPlayer - 1].PeekPieceInPosition(pos).GetIsMovingPiece())) 
			{
				unique_ptr<PointImpl> moveTo = getEmptyThreatFreeSquareToMoveTo(pos);
				if (moveTo != nullptr) {
					return std::make_unique<MoveImpl>(pos, *moveTo);
				}
			}
		}
	}

	return nullptr;
}
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
void AutoPlayerAlgorithm::getMovingPiecesInDistanceFromFlag(const PointImpl &flag_pos, int distance, std::vector<unique_ptr<PointImpl>>& posVectorToFill) 
{
	for (int row = 1; row <= M; row++)
	{
		for (int col = 1; col <= N; col++)
		{
			if (!mPlayersStrategyBoards[mPlayer - 1].IsEmptyInPosition(col, row))
			{
				if (mPlayersStrategyBoards[mPlayer - 1].PeekPieceInPosition(col, row).GetIsMovingPiece()) 
				{
					PointImpl point(col, row);
					if (point.DistanceInStepsFrom(flag_pos) == distance) {
						posVectorToFill.push_back(std::make_unique<PointImpl>(col, row));
					}
				}
			}
				
		}
	}
}

unique_ptr<PointImpl> AutoPlayerAlgorithm::getUnoccupiedPlaceTowardsFlag(const PointImpl& moveFrom, const PointImpl& flagPos, bool ifToCheckThreatened) const
{
	std::vector<unique_ptr<PointImpl>> adjacentLegalPositions;
	FillAdjacentLegalPositions(moveFrom, adjacentLegalPositions);
	const StrategyPiece& piece = mPlayersStrategyBoards[mPlayer - 1].PeekPieceInPosition(moveFrom);

	int currX, currY;
	bool threatenedCheck;

	for (const unique_ptr<PointImpl>& pos : adjacentLegalPositions)
	{
		currX = pos->getX();
		currY = pos->getY();
		threatenedCheck = !ifToCheckThreatened || !isThreatenedInPosition(piece, *pos);
		if (BoardImpl<StrategyPiece>::CheckIfValidPosition(*pos) 
			&& AreBothBoardsEmptyInPosition(*pos)
			&& pos->DistanceInStepsFrom(flagPos) < moveFrom.DistanceInStepsFrom(flagPos)
			&& threatenedCheck)
		{
			return make_unique<PointImpl>(currX, currY);
		}
	}

	return nullptr;
}

unique_ptr<Move> AutoPlayerAlgorithm::conquerTheFlag() 
{
	// TODO: why is it needed here
	//unique_ptr<Move> move(saveAPiece());
	//if (move != nullptr) {
	//	return move;
	//}
	std::vector<unique_ptr<PointImpl>> posVector;
	unique_ptr<PointImpl> moveTo;
	for (const PointImpl& flagPoint : mOpponentFlagLocations) {

		// If a moving piece is adjacent to the opponent flag, conquer it.
		getMovingPiecesInDistanceFromFlag(flagPoint, 1, posVector);
		if (posVector.size() != 0) 
		{
			return std::make_unique<MoveImpl>(*posVector[0], flagPoint);
		}

		// Get a closest piece to the flag and move it closer if possible.
		for (int d = 2; d < N - 1 + M - 1; d++) {
			getMovingPiecesInDistanceFromFlag(flagPoint, d, posVector); //TODO:is it emptyed everytime?
			//if (posVector.size() != 0) {
				for (unique_ptr<PointImpl>& pos : posVector) {
					moveTo = getUnoccupiedPlaceTowardsFlag(*pos, flagPoint, true);
					if (moveTo != nullptr) {
						return std::make_unique<MoveImpl>(*pos, *moveTo);
					}
				}
			//}
		}
	}

	// TODO: why is it needed
	//PointImpl* moveClosest;
	//for (PointImpl& flagPoint : mOpponentFlagLocations) {
	//	for (int d = 0; d < N - 1 + M - 1; d++) {
	//		getPiecesOfSpecificDistanceFromFlag(flagPoint, d, posVector);
	//		if (posVector.size() != 0) {
	//			moveClosest = getPlaceTowardsFlag(*posVector[0], flagPoint, false);
	//			if (moveClosest != nullptr) {
	//				return std::make_unique<MoveImpl>(*posVector[0], *moveClosest);
	//			}
	//		}
	//	}
	//}		

	return nullptr;
}

unique_ptr<Move> AutoPlayerAlgorithm::getMove()
{
	//findFlag(); // Why is it needed here?

	unique_ptr<Move> move = nullptr;
	if (mOpponentFlagLocations.size() != 0) 
	{
		move = conquerTheFlag();	//why would it give nullptr?
	} 
	
	if (move == nullptr)
	{
		move = getNormalMove();
	}

	//if (mGameBoardInfo.GetBoardInPosition(move->getFrom()).GetPiece()->GetPieceType() == PieceFactory::PieceType::Joker) {
	//	updateJokerLocation(move->getFrom(), move->getTo());
	//}

	//movePieceOnInfoBoard(*move);
	mPlayersStrategyBoards[mPlayer - 1].MovePieceWithoutChecks(move->getFrom(), move->getTo());
	
	updateThreats();

	return move;
}

unique_ptr<JokerChange> AutoPlayerAlgorithm::getJokerChange()
{
	return nullptr;
}

void AutoPlayerAlgorithm::notifyOnOpponentMove(const Move& move)
{
	if (!mPlayersStrategyBoards[mOpponent - 1].PeekPieceInPosition(move.getFrom()).GetIsMovingPiece())
	{
		mPlayersStrategyBoards[mOpponent - 1].PeekPieceInPosition(move.getFrom()).SetIsMovingPiece(true);
		mNumMovablePieces++;
	}

	mPlayersStrategyBoards[mOpponent - 1].MovePieceWithoutChecks(move.getFrom(), move.getTo());

	findOpponentFlag();
	updateThreats();
}

void AutoPlayerAlgorithm::notifyFightResult(const FightInfo & fightInfo)
{
	updateStrategyAccordingToFight(fightInfo);
	
	findOpponentFlag();
	updateThreats();
}

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

void AutoPlayerAlgorithm::updateThreatsForPlayerInPosition(int player, const PointImpl& pos)
{
	if (!mPlayersStrategyBoards[player - 1].IsEmptyInPosition(pos))
	{
		StrategyPiece& strategyPiece = mPlayersStrategyBoards[player - 1].PeekPieceInPosition(pos);
		strategyPiece.SetIsThreatened(isThreatenedInPosition(strategyPiece, pos));
		strategyPiece.SetIsThreathening(isThreateningInPosition(strategyPiece, pos));
	}
}

// TODO: maybe not all board.
void AutoPlayerAlgorithm::updateThreats() 
{
	for (int row = 1; row <= M; row++)
	{
		for (int col = 1; col <= N; col++)
		{
			PointImpl currPos(col, row);
			updateThreatsForPlayerInPosition(mPlayer, currPos);
			updateThreatsForPlayerInPosition(mOpponent, currPos);
		}
	}
}

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
//PointImpl* AutoPlayerAlgorithm::runForYourLife(int xPos, int yPos) {
//
//	if (xPos <N && mGameBoardInfo.GetBoardInPosition(xPos + 1, yPos).IsEmpty() == true
//		&& !isThreatenedByNeighbourhood(xPos + 1, yPos, mPlayer)) {
//		return new PointImpl(xPos + 1, yPos);
//	}
//	else if (yPos < N && mGameBoardInfo.GetBoardInPosition(xPos, yPos + 1).IsEmpty() == true
//		&& !isThreatenedByNeighbourhood(xPos , yPos+1, mPlayer)) {
//		return new PointImpl(xPos, yPos + 1);
//	}
//	else if (xPos > 1 && mGameBoardInfo.GetBoardInPosition(xPos - 1, yPos).IsEmpty() == true
//		&& !isThreatenedByNeighbourhood(xPos - 1, yPos, mPlayer)) {
//		return new PointImpl(xPos - 1, yPos);
//	}
//	else if (yPos > 1 && mGameBoardInfo.GetBoardInPosition(xPos, yPos - 1).IsEmpty() == true
//		&& !isThreatenedByNeighbourhood(xPos, yPos-1, mPlayer)) {
//		return new PointImpl(xPos, yPos - 1);
//	}
//	return nullptr; //how to get rid? null?
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

// TODO: threateningPlayer
bool AutoPlayerAlgorithm::isThreatenedInPosition(const Piece& piece, const PointImpl& pos) const
{
	//Piece& piece = mPlayersStrategyBoards[mPlayer - 1].PeekPieceInPosition(xPos, yPos);
	std::vector<unique_ptr<PointImpl>> adjacentLegalPositions;
	FillAdjacentLegalPositions(pos, adjacentLegalPositions);

	int threateningPlayer;

	if (piece.GetOwnerNum() == mPlayer)
	{
		threateningPlayer = mOpponent;
	}
	else
	{
		threateningPlayer = mPlayer;
	}

	for (const unique_ptr<PointImpl>& pos : adjacentLegalPositions)
	{
		if (!mPlayersStrategyBoards[threateningPlayer - 1].IsEmptyInPosition(pos->getX(), pos->getY())) 
		{
			const StrategyPiece& neighbourPiece = mPlayersStrategyBoards[threateningPlayer - 1].PeekPieceInPosition(pos->getX(), pos->getY());

			// if (piece == false // ask why
			if (neighbourPiece.GetIsMovingPiece() && neighbourPiece.IsStrongerThan(piece)) 
			{ //TODO: WILL WORK?
				return true;
			}
		}
	}

	return false;
}

// TODO: merge functions
bool AutoPlayerAlgorithm::isThreateningInPosition(const Piece& piece, const PointImpl& pos) const
{
	//Piece& piece = mPlayersStrategyBoards[mPlayer - 1].PeekPieceInPosition(xPos, yPos);
	std::vector<unique_ptr<PointImpl>> adjacentLegalPositions;
	FillAdjacentLegalPositions(pos, adjacentLegalPositions);

	if (!piece.GetIsMovingPiece())
	{
		return false;
	}

	int threatenedPlayer;

	if (piece.GetOwnerNum() == mPlayer)
	{
		threatenedPlayer = mOpponent;
	}
	else
	{
		threatenedPlayer = mPlayer;
	}

	for (const unique_ptr<PointImpl>& pos : adjacentLegalPositions)
	{
		if (!mPlayersStrategyBoards[threatenedPlayer - 1].IsEmptyInPosition(pos->getX(), pos->getY()))
		{
			const StrategyPiece& neighbourPiece = mPlayersStrategyBoards[threatenedPlayer - 1].PeekPieceInPosition(pos->getX(), pos->getY());

			if (piece.IsStrongerThan(neighbourPiece))
			{
				return true;
			}
		}
	}

	return false;
}

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
