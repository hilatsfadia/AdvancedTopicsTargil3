#include "stdafx.h"
#include "AutoPlayerAlgorithm.h"
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
#include "Player.h"
#include "MoveImpl.h"
#include "JokerChangeImpl.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iterator>

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

	initPositionsVectorOneType(vectorToFill, xPos, yPos, isToMoveForward, J, JOKER_CHAR, ROCK_CHAR);
	//initPositionsVectorOneType(vectorToFill, xPos, yPos, isToMoveForward, J, JOKER_CHAR, BOMB_CHAR);
	initPositionsVectorOneType(vectorToFill, xPos, yPos, isToMoveForward, R, ROCK_CHAR);
	initPositionsVectorOneType(vectorToFill, xPos, yPos, isToMoveForward, P, PAPER_CHAR);
	initPositionsVectorOneType(vectorToFill, xPos, yPos, isToMoveForward, S, SCISSORS_CHAR);

	//mPlayerNumMovablePieces = R + P + S;
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
			mPlayerJokerLocations.push_back(piecePos->getPosition());
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
	srand((unsigned int)time(0));
	//srand(static_cast<unsigned int>(time(0)));

	mPlayer = player; //setting the players fields
	if (mPlayer == 1) {
		mOpponent = 2;
	}
	else {
		mOpponent = 1;
	}

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
					if (mPlayersStrategyBoards[mPlayer - 1].PeekPieceInPosition(pos).GetPieceType() == PieceFactory::PieceType::Joker)
					{
						eraseJokerLocation(pos);
					}

					mPlayersStrategyBoards[mPlayer - 1].ClearBoardInPosition(pos);
				}
			}

			// Update the board of the oponnent player, by getting all it's pieces locations
			if (b.getPlayer(pos) == mOpponent)
			{
				unique_ptr<StrategyPiece> strategyPiece = make_unique<StrategyPiece>(mOpponent); //uncovered piece
				mPlayersStrategyBoards[mOpponent - 1].ClearBoardInPosition(pos); //delets the former piece in square. is it needed?
				mPlayersStrategyBoards[mOpponent - 1].PutPieceInPosition(pos, std::move(strategyPiece));
				mOpponentNumCoveredPieces++;
			}
		}
	}
}

void AutoPlayerAlgorithm::updateStrategyAccordingToFight(const FightInfo& fight)
{
	//next exercise remember to add handling to more than one flag
	int winner = fight.getWinner();

	if ((winner == TIE) || (winner == mPlayer))
	{
		// TODO: look
		// It can be empty due to not initialized in notifyOnInitialBoard
		if (!mPlayersStrategyBoards[mOpponent - 1].IsEmptyInPosition(fight.getPosition()))
		{
			if (mPlayersStrategyBoards[mOpponent - 1].PeekPieceInPosition(fight.getPosition()).GetPieceType() == PieceFactory::PieceType::Covered)
			{
				mOpponentNumCoveredPieces--;
			}
			if (mPlayersStrategyBoards[mOpponent - 1].PeekPieceInPosition(fight.getPosition()).GetIsMovingPiece())
			{
				mOpponentNumMovablePieces--;
			}
		}
	}

	if (winner == TIE) {
		ClearPlayersBoardsInPosition(fight.getPosition());
	}
	else if (winner == mOpponent) {
		mPlayersStrategyBoards[mPlayer - 1].ClearBoardInPosition(fight.getPosition()); // already done by the supplied board?
		bool oldIsMoving = mPlayersStrategyBoards[mOpponent - 1].PeekPieceInPosition(fight.getPosition()).GetIsMovingPiece();

		// Uncover also if uncovered already to handle jokers.
		mPlayersStrategyBoards[mOpponent - 1].PeekPieceInPosition(fight.getPosition()).UncoverPiece(fight.getPiece(winner));

		// If uncovering reveled it as moving
		if (!oldIsMoving && (mPlayersStrategyBoards[mOpponent - 1].PeekPieceInPosition(fight.getPosition()).GetIsMovingPiece()))
		{
			mOpponentNumMovablePieces++;
		}
	}
	else // winner == mPlayer
	{
		mPlayersStrategyBoards[mOpponent - 1].ClearBoardInPosition(fight.getPosition());
	}
}

void AutoPlayerAlgorithm::findOpponentFlags() 
{
	if (mOpponentNumCoveredPieces == F || (mOpponentNumCoveredPieces - mOpponentNumMovablePieces <= F)) 
	{ //TODO: double check that everyone is by default false
		for (int row = 1; row <= M; row++)
		{
			for (int col = 1; col <= N; col++)
			{
				if (!mPlayersStrategyBoards[mOpponent - 1].IsEmptyInPosition(col, row)) 
				{
					StrategyPiece& piece = mPlayersStrategyBoards[mOpponent - 1].PeekPieceInPosition(col, row);

					if ((piece.GetPieceType() == PieceFactory::PieceType::Covered)
						&& (!piece.GetIsMovingPiece())) // TODO: ask why is it needed
					{
						piece.UncoverPiece(FLAG_CHAR);
						mOpponentNumCoveredPieces--;
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

	findOpponentFlags();

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

void AutoPlayerAlgorithm::FillAdjacentLegalPositions(const Point& pos, std::vector<unique_ptr<PointImpl>>& vectorToFill) const
{
	int xPos = pos.getX();
	int yPos = pos.getY();
	PointImpl options[NUM_OF_ADJACENT_POSITIONS] =
	{ PointImpl(xPos, yPos + 1), PointImpl(xPos + 1, yPos), PointImpl(xPos - 1, yPos) , PointImpl(xPos, yPos - 1) };

	for (int i = 0; i < NUM_OF_ADJACENT_POSITIONS; i++)
	{
		if (BoardImpl<StrategyPiece>::CheckIfValidPosition(options[i]))
		{
			vectorToFill.push_back(make_unique<PointImpl>(options[i]));
		}
	}

	std::random_shuffle(std::begin(vectorToFill), std::end(vectorToFill));
}

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

unique_ptr<PointImpl> AutoPlayerAlgorithm::getUnoccupiedPlaceTowardsFlag(const PointImpl& from, const PointImpl& flagPos) const
{
	std::vector<unique_ptr<PointImpl>> adjacentLegalPositions;
	FillAdjacentLegalPositions(from, adjacentLegalPositions);
	const StrategyPiece& piece = mPlayersStrategyBoards[mPlayer - 1].PeekPieceInPosition(from);

	for (const unique_ptr<PointImpl>& pos : adjacentLegalPositions)
	{
		if (isRelevantDestination(piece, *pos, AutoPlayerAlgorithm::MoveType::TowardsFlag) 
			&& pos->DistanceInStepsFrom(flagPos) < from.DistanceInStepsFrom(flagPos))
		{
			return make_unique<PointImpl>(*pos);
		}
	}

	return nullptr;
}

unique_ptr<Move> AutoPlayerAlgorithm::conquerTheFlag() 
{
	std::vector<unique_ptr<PointImpl>> posVector;
	unique_ptr<PointImpl> moveTo;
	for (const PointImpl& flagPoint : mOpponentFlagLocations) {
		// If a moving piece is adjacent to the opponent flag, conquer it.
		getMovingPiecesInDistanceFromFlag(flagPoint, 1, posVector);
		if (posVector.size() != 0)
		{
			return std::make_unique<MoveImpl>(*posVector[0], flagPoint);
		}
	}

	unique_ptr<Move> move = getStrategyMove(AutoPlayerAlgorithm::MoveType::RunAway);
	if (move != nullptr) {
		return move;
	}

	for (const PointImpl& flagPoint : mOpponentFlagLocations) {
		// Get a closest piece to the flag and move it closer if possible.
		for (int d = 2; d < N - 1 + M - 1; d++) {
			getMovingPiecesInDistanceFromFlag(flagPoint, d, posVector); //TODO:is it emptyed everytime?
			//if (posVector.size() != 0) {
				for (unique_ptr<PointImpl>& pos : posVector) {
					moveTo = getUnoccupiedPlaceTowardsFlag(*pos, flagPoint);
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
	unique_ptr<Move> move = getStrategyMove(AutoPlayerAlgorithm::MoveType::RunAway);
	if (move == nullptr) {
		if (mOpponentFlagLocations.size() != 0) {
			move = conquerTheFlag();	//why would it give nullptr?
		}

		if (move == nullptr) {
			move = getStrategyMove(AutoPlayerAlgorithm::MoveType::Attack);

			if (move == nullptr) {
				move = getStrategyMove(AutoPlayerAlgorithm::MoveType::Random);
			}
		}
	}

	if (mPlayersStrategyBoards[mPlayer - 1].PeekPieceInPosition(move->getFrom()).GetPieceType() == PieceFactory::PieceType::Joker) {
		updateJokerLocation(move->getFrom(), move->getTo());
	}

	//movePieceOnInfoBoard(*move);
	mPlayersStrategyBoards[mPlayer - 1].MovePieceWithoutChecks(move->getFrom(), move->getTo());
	
	updateThreats();

	return move;
}

void AutoPlayerAlgorithm::notifyOnOpponentMove(const Move& move)
{
	if (!mPlayersStrategyBoards[mOpponent - 1].PeekPieceInPosition(move.getFrom()).GetIsMovingPiece())
	{
		mPlayersStrategyBoards[mOpponent - 1].PeekPieceInPosition(move.getFrom()).SetMovingPiece();
		mOpponentNumMovablePieces++;
	}

	mPlayersStrategyBoards[mOpponent - 1].MovePieceWithoutChecks(move.getFrom(), move.getTo());

	findOpponentFlags();
	updateThreats();
}

void AutoPlayerAlgorithm::notifyFightResult(const FightInfo & fightInfo)
{
	updateStrategyAccordingToFight(fightInfo);
	
	findOpponentFlags();
	updateThreats();
}

void AutoPlayerAlgorithm::eraseJokerLocation(const Point& jokerPos) {
	for (std::vector<PointImpl>::iterator itr = mPlayerJokerLocations.begin(); itr != mPlayerJokerLocations.end(); ) {
		if (jokerPos.getX() == itr->getX()
			&& jokerPos.getY() == itr->getY()) { //TODO:impl ==
			itr = mPlayerJokerLocations.erase(itr);
			break;
		}

		++itr;
	}
}

void AutoPlayerAlgorithm::updateJokerLocation(const Point& oldJokerPos, const Point& newJokerPos) {
	eraseJokerLocation(oldJokerPos);
	mPlayerJokerLocations.push_back(newJokerPos);
}

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

bool AutoPlayerAlgorithm::isRelevantDestination(const StrategyPiece& piece, const PointImpl& pos, MoveType moveType) const{
	// Important! don't go to an invalid place or a place with other piece of the same player
	bool isRelevantDest = BoardImpl<StrategyPiece>::CheckIfValidPosition(pos) &&
		mPlayersStrategyBoards[mPlayer - 1].IsEmptyInPosition(pos);

	switch (moveType){
		case AutoPlayerAlgorithm::MoveType::RunAway:
		case AutoPlayerAlgorithm::MoveType::TowardsFlag:
		case AutoPlayerAlgorithm::MoveType::Random: {
			isRelevantDest = isRelevantDest && (AreBothBoardsEmptyInPosition(pos) && (!isThreatenedInPosition(piece, pos)));
			break;
		}
		case AutoPlayerAlgorithm::MoveType::Attack:{
			isRelevantDest = isRelevantDest && ((!mPlayersStrategyBoards[mOpponent - 1].IsEmptyInPosition(pos))
				&& (piece.IsStrongerThan(mPlayersStrategyBoards[mOpponent - 1].PeekPieceInPosition(pos)))
				&& !isThreatenedInPosition(piece, pos));
			break;
		}
		default:{
			isRelevantDest = false;
			break;
		}
	}

	return isRelevantDest;
}

unique_ptr<PointImpl> AutoPlayerAlgorithm::getStrategyDestination(const StrategyPiece& piece, const PointImpl& from, MoveType moveType) const
{
	std::vector<unique_ptr<PointImpl>> adjacentLegalPositions;
	FillAdjacentLegalPositions(from, adjacentLegalPositions);

	for (const unique_ptr<PointImpl>& pos : adjacentLegalPositions)
	{
		if (isRelevantDestination(piece, *pos, moveType))
		{
			//return make_unique<PointImpl>(currX, currY);
			return make_unique<PointImpl>(*pos);
		}
	}

	return nullptr;
}

bool AutoPlayerAlgorithm::isPieceToMove(const StrategyPiece& strategyPiece, AutoPlayerAlgorithm::MoveType moveType)
{
	bool isRelevantPiece = strategyPiece.GetIsMovingPiece();
	switch (moveType)
	{
		case AutoPlayerAlgorithm::MoveType::RunAway:
		{
			// Joker doesn't run away, it can change it representation.
			isRelevantPiece = isRelevantPiece && strategyPiece.GetIsThreatened() 
				&& (strategyPiece.GetPieceType() != PieceFactory::PieceType::Joker);
			break;
		}
		case AutoPlayerAlgorithm::MoveType::Attack:
		{
			isRelevantPiece = isRelevantPiece && strategyPiece.GetIsThreathening();
			break;
		}
		case AutoPlayerAlgorithm::MoveType::Random:
		{
			//isRelevantPiece = isRelevantPiece && ((strategyPiece.GetStrategyPieceID() != lastMovedPieceID) || mPlayerNumMovablePieces <= 1);
			break;
		}
		default:
		{
			isRelevantPiece = false;
			break;
		}
	}

	return isRelevantPiece;
}

unique_ptr<Move> AutoPlayerAlgorithm::getStrategyMoveInRow(AutoPlayerAlgorithm::MoveType moveType, int row)
{
	for (int col = 1; col <= N; col++)
	{
		PointImpl posFrom(col, row);
		if (!mPlayersStrategyBoards[mPlayer - 1].IsEmptyInPosition(posFrom))
		{
			StrategyPiece& strategyPiece = mPlayersStrategyBoards[mPlayer - 1].PeekPieceInPosition(posFrom);

			if (isPieceToMove(strategyPiece, moveType))
			{
				unique_ptr<PointImpl> posTo = getStrategyDestination(strategyPiece, posFrom, moveType);

				if (posTo != nullptr)
				{
					lastMovedPieceID = strategyPiece.GetStrategyPieceID();
					return std::make_unique<MoveImpl>(posFrom, *posTo);
				}
			}
		}
	}

	return nullptr;
}

unique_ptr<Move> AutoPlayerAlgorithm::getStrategyMove(AutoPlayerAlgorithm::MoveType moveType){
	if (mPlayer == 1)
	{
		for (int row = 1; row <= M; row++)
		{
			unique_ptr<Move> posTo = getStrategyMoveInRow(moveType, row);
			if (posTo != nullptr){
				return posTo;
			}
		}
	}
	else
	{
		for (int row = M; row >= 1; row--)
		{
			unique_ptr<Move> posTo = getStrategyMoveInRow(moveType, row);
			if (posTo != nullptr){
				return posTo;
			}
		}
	}

	return nullptr;
}

bool AutoPlayerAlgorithm::isThreatenedInPosition(const StrategyPiece& piece, const PointImpl& pos) const
{
	//Piece& piece = mPlayersStrategyBoards[mPlayer - 1].PeekPieceInPosition(xPos, yPos);
	std::vector<unique_ptr<PointImpl>> adjacentLegalPositions;
	FillAdjacentLegalPositions(pos, adjacentLegalPositions);

	int threateningPlayer = (piece.GetOwnerNum() == mPlayer) ? mOpponent : mPlayer;

	for (const unique_ptr<PointImpl>& pos : adjacentLegalPositions)
	{
		if (!mPlayersStrategyBoards[threateningPlayer - 1].IsEmptyInPosition(*pos)) 
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
bool AutoPlayerAlgorithm::isThreateningInPosition(const StrategyPiece& piece, const PointImpl& pos) const
{
	//Piece& piece = mPlayersStrategyBoards[mPlayer - 1].PeekPieceInPosition(xPos, yPos);
	std::vector<unique_ptr<PointImpl>> adjacentLegalPositions;
	FillAdjacentLegalPositions(pos, adjacentLegalPositions);

	if (!piece.GetIsMovingPiece())
	{
		return false;
	}

	int threatenedPlayer = (piece.GetOwnerNum() == mPlayer) ? mOpponent : mPlayer;

	for (const unique_ptr<PointImpl>& pos : adjacentLegalPositions)
	{
		if (!mPlayersStrategyBoards[threatenedPlayer - 1].IsEmptyInPosition(*pos))
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

unique_ptr<JokerChange> AutoPlayerAlgorithm::getJokerChange()
{
	unique_ptr<JokerChange> jokerChange = nullptr;
	for (const Point& jokerPos : mPlayerJokerLocations) {
		StrategyPiece& strategyPiece = mPlayersStrategyBoards[mPlayer - 1].PeekPieceInPosition(jokerPos);

		if (strategyPiece.GetIsThreatened()) {
			jokerChange = changeThreatenedJoker(jokerPos);
			updateThreats();
		}
	}

	return jokerChange;
}

unique_ptr<JokerChange> AutoPlayerAlgorithm::changeThreatenedJoker(const Point& threatenedJokerPos) {
	std::vector<unique_ptr<PointImpl>> adjacentLegalPositions;
	FillAdjacentLegalPositions(threatenedJokerPos, adjacentLegalPositions);
	StrategyPiece& threatenedPiece = mPlayersStrategyBoards[mPlayer - 1].PeekPieceInPosition(threatenedJokerPos);

	for (const unique_ptr<PointImpl>& threateningPos : adjacentLegalPositions)
	{
		if (!mPlayersStrategyBoards[mOpponent - 1].IsEmptyInPosition(*threateningPos))
		{
			const StrategyPiece& threateningPiece = mPlayersStrategyBoards[mOpponent - 1].PeekPieceInPosition(*threateningPos);

			if (threateningPiece.GetIsMovingPiece() && threateningPiece.IsStrongerThan(threatenedPiece))
			{
				threatenedPiece.ChangeJokerToStronger(threateningPiece.GetActualPieceType());
				// TODO: maybe do it smarter
			}

			return std::make_unique<JokerChangeImpl>(threatenedJokerPos, threatenedPiece.GetActualPieceChar());
		}
	}

	return nullptr;
}
