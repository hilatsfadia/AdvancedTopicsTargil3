#include "RSPPlayer_309962264.h"
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
#include "AlgorithmRegistration.h"

#define NUM_OF_CORNERS 4

REGISTER_ALGORITHM(309962264)

using std::make_unique;
// Type Alias
using PieceType = PieceFactory::PieceType;

void RSPPlayer_309962264::UpdateColumnNumber(int & xPos, bool isToMoveRight) const
{
	if (isToMoveRight)
	{
		xPos++;
	}
	else
	{
		xPos--;
	}
}


void RSPPlayer_309962264::initPositionsVectorOneType(std::vector<unique_ptr<PiecePosition>>& vectorToFill,
	int& xPos, int& yPos, bool isToMoveRight, int count, char typeChar, char jokerReper) const
{
	for (int i = 0; i < count; i++) {
		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(PointImpl(xPos, yPos), typeChar, jokerReper));
		if (yPos == M) {
			yPos = 1;
			UpdateColumnNumber(xPos, isToMoveRight);
		}
		else
		{
			yPos += 2;
		}
	}

}


void RSPPlayer_309962264::initPositionsVectorCorners(std::vector<unique_ptr<PiecePosition>>& vectorToFill,
	char typeChar, char jokerReper) const
{
	PointImpl options[NUM_OF_CORNERS] =
	{ PointImpl(1, 1), PointImpl(1, M), PointImpl(N, 1) , PointImpl(N, M) };

	for (int i = 0; i < NUM_OF_CORNERS; i++)
	{
		vectorToFill.push_back(make_unique<PiecePositionImpl>(options[i], typeChar, jokerReper));
	}

}

void RSPPlayer_309962264::initPositionsVector(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill) const
{
	PointImpl point;
	int yPos = (M / 2) - 1;
	int xPos = (player == FIRST_PLAYER_NUM) ? 1 : N;
	bool isToMoveRight = (player == FIRST_PLAYER_NUM) ? true : false;
	initPositionsVectorOneType(vectorToFill, xPos, yPos, isToMoveRight, F, FLAG_CHAR);
	if (B > 0)
	{
		yPos = (M / 2) - 2;
		initPositionsVectorOneType(vectorToFill, xPos, yPos, isToMoveRight, B / 2, BOMB_CHAR);
		yPos = (M / 2);
		initPositionsVectorOneType(vectorToFill, xPos, yPos, isToMoveRight, (B - (B / 2)), BOMB_CHAR);
		yPos = (M / 2) - 1;
		UpdateColumnNumber(xPos, isToMoveRight);
		if (J > 1) {
			initPositionsVectorOneType(vectorToFill, xPos, yPos, isToMoveRight, 1, JOKER_CHAR, BOMB_CHAR);
		}
	}
	UpdateColumnNumber(xPos, isToMoveRight);
	yPos = 2;
	initPositionsVectorOneType(vectorToFill, xPos, yPos, isToMoveRight, S, SCISSORS_CHAR);
	if (J > 1) {
		initPositionsVectorOneType(vectorToFill, xPos, yPos, isToMoveRight, J - 1, JOKER_CHAR, SCISSORS_CHAR);
	}
	initPositionsVectorOneType(vectorToFill, xPos, yPos, isToMoveRight, R, ROCK_CHAR);
	initPositionsVectorCorners(vectorToFill, PAPER_CHAR);
	if (P > NUM_OF_CORNERS) {
		initPositionsVectorOneType(vectorToFill, xPos, yPos, isToMoveRight, P - NUM_OF_CORNERS, PAPER_CHAR);
	}
}


void RSPPlayer_309962264::initTheAlgorithmPlayerBoard(int player, const std::vector<unique_ptr<PiecePosition>>& vectorToFill)
{
	for (const std::unique_ptr<PiecePosition>& piecePos : vectorToFill) {
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

		unique_ptr<StrategyPiece> strategyPiece = make_unique<StrategyPiece>(mPlayer);
		strategyPiece->UncoverPiece(std::move(uncoveredPiece));
		mPlayersStrategyBoards[player - 1].PutPieceInPosition(piecePos->getPosition(), std::move(strategyPiece));
	}
}

void RSPPlayer_309962264::getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill)
{
	try {
		std::random_device rd;
		mRandGen.seed(rd());
	}
	catch (...) {
		// If there was a problem, don't seed the random gen.
	}

	mPlayer = player; //setting the players fields
	if (mPlayer == FIRST_PLAYER_NUM) {
		mOpponent = SECOND_PLAYER_NUM;
	}
	else {
		mOpponent = FIRST_PLAYER_NUM;
	}

	initPositionsVector(player, vectorToFill);
	initTheAlgorithmPlayerBoard(player, vectorToFill);
}

void RSPPlayer_309962264::ClearPlayersBoardsInPosition(const Point& pos)
{
	mPlayersStrategyBoards[mPlayer - 1].ClearBoardInPosition(pos);
	mPlayersStrategyBoards[mOpponent - 1].ClearBoardInPosition(pos);
}

void RSPPlayer_309962264::updateStrategyAccordingToBoard(const Board & b)
{
	for (int row = 1; row <= M; row++)
	{
		for (int col = 1; col <= N; col++)
		{
			PointImpl pos = PointImpl(col, row);

			// Update the board of the player of this algorithm, by removing
			// eaten pieces.
			//if (b.getPlayer(pos) != mPlayer) // it's mOpponent or 0
			//{
			//	if (!mPlayersStrategyBoards[mPlayer - 1].IsEmptyInPosition(pos))
			//	{
			//		if (mPlayersStrategyBoards[mPlayer - 1].PeekPieceInPosition(pos).GetPieceType() == PieceType::Joker)
			//		{
			//			eraseJokerLocation(pos);
			//		}

			//		mPlayersStrategyBoards[mPlayer - 1].ClearBoardInPosition(pos);
			//	}
			//}

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

void RSPPlayer_309962264::updateJokerLocationsAccordingToFight(const FightInfo& fight, int winner)
{
	if ((winner == TIE) || (winner == mOpponent))
	{
		if (!mPlayersStrategyBoards[mPlayer - 1].IsEmptyInPosition(fight.getPosition())) { // Shouldn't be empty
			if (mPlayersStrategyBoards[mPlayer - 1].PeekPieceInPosition(fight.getPosition()).GetPieceType() == PieceType::Joker)
			{
				eraseJokerLocation(fight.getPosition());
			}

			mPlayersStrategyBoards[mPlayer - 1].ClearBoardInPosition(fight.getPosition());
		}
	}
}

void RSPPlayer_309962264::updateStrategyAccordingToFight(const FightInfo& fight)
{
	//next exercise remember to add handling to more than one flag
	int winner = fight.getWinner();

	// It can be empty due to not initialized in notifyOnInitialBoard
	if (!mPlayersStrategyBoards[mOpponent - 1].IsEmptyInPosition(fight.getPosition())){
		if (mPlayersStrategyBoards[mOpponent - 1].PeekPieceInPosition(fight.getPosition()).GetPieceType() == PieceType::Covered){
			mOpponentNumCoveredPieces--;
		}
	}

	updateJokerLocationsAccordingToFight(fight, winner);

	if (winner == TIE) {
		ClearPlayersBoardsInPosition(fight.getPosition());
	}
	else if (winner == mOpponent) {
		mPlayersStrategyBoards[mPlayer - 1].ClearBoardInPosition(fight.getPosition()); // already done by the supplied board?
		if ((mPlayersStrategyBoards[mOpponent - 1].PeekPieceInPosition(fight.getPosition()).GetPieceType() == PieceType::Covered) && 
			(mPlayersStrategyBoards[mOpponent - 1].PeekPieceInPosition(fight.getPosition()).GetIsMovingPiece()))
		{
			mOpponentNumCoveredMovablePieces--;
		}
		// It can't be empty due to not initialized in notifyOnInitialBoard, because opponent is the winner
		// Uncover also if uncovered already to handle jokers.
		mPlayersStrategyBoards[mOpponent - 1].PeekPieceInPosition(fight.getPosition()).UncoverPiece(fight.getPiece(winner));
	}
	else // winner == mPlayer
	{
		mPlayersStrategyBoards[mOpponent - 1].ClearBoardInPosition(fight.getPosition());
	}
}

void RSPPlayer_309962264::findOpponentFlags() 
{
	if (mOpponentNumCoveredPieces == F || (mOpponentNumCoveredPieces - mOpponentNumCoveredMovablePieces <= F))
	{ //TODO: double check that everyone is by default false
		for (int row = 1; row <= M; row++)
		{
			for (int col = 1; col <= N; col++)
			{
				if (!mPlayersStrategyBoards[mOpponent - 1].IsEmptyInPosition(col, row)) 
				{
					StrategyPiece& piece = mPlayersStrategyBoards[mOpponent - 1].PeekPieceInPosition(col, row);

					if ((piece.GetPieceType() == PieceType::Covered)
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

void RSPPlayer_309962264::notifyOnInitialBoard(const Board & b, const std::vector<unique_ptr<FightInfo>>& fights)
{
	updateStrategyAccordingToBoard(b);

	for (const std::unique_ptr<FightInfo>& fight : fights)
	{
		updateStrategyAccordingToFight(*fight);
	}

	findOpponentFlags();

	updateThreats();
}

bool RSPPlayer_309962264::AreBothBoardsEmptyInPosition(int x, int y) const
{
	return (mPlayersStrategyBoards[mPlayer - 1].IsEmptyInPosition(x, y) &&
		mPlayersStrategyBoards[mOpponent - 1].IsEmptyInPosition(x, y));
}

bool RSPPlayer_309962264::AreBothBoardsEmptyInPosition(const Point& pos) const
{
	return AreBothBoardsEmptyInPosition(pos.getX(), pos.getY());
}

void RSPPlayer_309962264::FillAdjacentLegalPositions(const Point& pos, std::vector<unique_ptr<PointImpl>>& vectorToFill) const
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

	//std::random_shuffle(std::begin(vectorToFill), std::end(vectorToFill));
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
void RSPPlayer_309962264::getMovingPiecesInDistanceFromFlag(const PointImpl &flag_pos, 
	int distance, std::vector<unique_ptr<PointImpl>>& posVectorToFill) const
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

unique_ptr<PointImpl> RSPPlayer_309962264::getUnoccupiedPlaceTowardsFlag(const PointImpl& from, const PointImpl& flagPos) const
{
	std::vector<unique_ptr<PointImpl>> adjacentLegalPositions;
	FillAdjacentLegalPositions(from, adjacentLegalPositions);
	const StrategyPiece& piece = mPlayersStrategyBoards[mPlayer - 1].PeekPieceInPosition(from);

	for (const unique_ptr<PointImpl>& pos : adjacentLegalPositions)
	{
		if (isRelevantDestination(piece, *pos, RSPPlayer_309962264::MoveType::TowardsFlag) 
			&& pos->DistanceInStepsFrom(flagPos) < from.DistanceInStepsFrom(flagPos))
		{
			return make_unique<PointImpl>(*pos);
		}
	}

	return nullptr;
}

unique_ptr<Move> RSPPlayer_309962264::conquerTheFlag()
{
	std::vector<unique_ptr<PointImpl>> posVector;
	unique_ptr<PointImpl> moveTo;
	for (const PointImpl& flagPoint : mOpponentFlagLocations) {
		// If a moving piece is adjacent to the opponent flag, conquer it.
		getMovingPiecesInDistanceFromFlag(flagPoint, 1, posVector);
		if (posVector.size() != 0){
			return std::make_unique<MoveImpl>(*posVector[0], flagPoint);
		}
	}

	unique_ptr<Move> move = getStrategyMove(RSPPlayer_309962264::MoveType::RunAway);
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

	return nullptr;
}

unique_ptr<Move> RSPPlayer_309962264::getMove()
{
	//findFlag(); // Why is it needed here?
	unique_ptr<Move> move = getStrategyMove(RSPPlayer_309962264::MoveType::RunAway);
	if (move == nullptr) {
		if (mOpponentFlagLocations.size() != 0) {
			move = conquerTheFlag();	//why would it give nullptr?
		}
		if (move == nullptr) {
			move = getStrategyMove(RSPPlayer_309962264::MoveType::Attack);
			if (move == nullptr) {
				move = getStrategyMove(RSPPlayer_309962264::MoveType::NotThreatened);
				if (move == nullptr) {
					move = getStrategyMove(RSPPlayer_309962264::MoveType::Random);
				}
			}
		}
	}

	if (move != nullptr) {
		if (mPlayersStrategyBoards[mPlayer - 1].PeekPieceInPosition(move->getFrom()).GetPieceType() == PieceType::Joker) {
			updateJokerLocation(move->getFrom(), move->getTo());
		}

		mPlayersStrategyBoards[mPlayer - 1].MovePieceWithoutChecks(move->getFrom(), move->getTo());

		updateThreats();
	}

	return move;
}

void RSPPlayer_309962264::notifyOnOpponentMove(const Move& move)
{
	if (!mPlayersStrategyBoards[mOpponent - 1].PeekPieceInPosition(move.getFrom()).GetIsMovingPiece())
	{
		mPlayersStrategyBoards[mOpponent - 1].PeekPieceInPosition(move.getFrom()).SetMovingPiece();
		mOpponentNumCoveredMovablePieces++;
	}

	mPlayersStrategyBoards[mOpponent - 1].MovePieceWithoutChecks(move.getFrom(), move.getTo());

	findOpponentFlags();
	updateThreats();
}

void RSPPlayer_309962264::notifyFightResult(const FightInfo & fightInfo)
{
	updateStrategyAccordingToFight(fightInfo);
	
	findOpponentFlags();
	updateThreats();
}

void RSPPlayer_309962264::eraseJokerLocation(const Point& jokerPos) {
	for (std::vector<PointImpl>::iterator itr = mPlayerJokerLocations.begin(); itr != mPlayerJokerLocations.end(); ) {
		if (jokerPos.getX() == itr->getX()
			&& jokerPos.getY() == itr->getY()) { //TODO:impl ==
			itr = mPlayerJokerLocations.erase(itr);
			break;
		}

		++itr;
	}
}

void RSPPlayer_309962264::updateJokerLocation(const Point& oldJokerPos, const Point& newJokerPos) {
	eraseJokerLocation(oldJokerPos);
	mPlayerJokerLocations.push_back(newJokerPos);
}

void RSPPlayer_309962264::updateThreatsForPlayerInPosition(int player, const PointImpl& pos)
{
	if (!mPlayersStrategyBoards[player - 1].IsEmptyInPosition(pos))
	{
		StrategyPiece& strategyPiece = mPlayersStrategyBoards[player - 1].PeekPieceInPosition(pos);
		strategyPiece.SetIsThreatened(isThreatenedInPosition(strategyPiece, pos));
		strategyPiece.SetIsThreathening(isThreateningInPosition(strategyPiece, pos));
	}
}

// TODO: maybe not all board.
void RSPPlayer_309962264::updateThreats() 
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

bool RSPPlayer_309962264::isRelevantDestination(const StrategyPiece& piece, const PointImpl& dest, MoveType moveType) const{
	// Important! don't go to an invalid place or a place with other piece of the same player
	bool isRelevantDest = BoardImpl<StrategyPiece>::CheckIfValidPosition(dest) &&
		mPlayersStrategyBoards[mPlayer - 1].IsEmptyInPosition(dest);

	switch (moveType){
		case RSPPlayer_309962264::MoveType::RunAway:
		case RSPPlayer_309962264::MoveType::TowardsFlag:
		case RSPPlayer_309962264::MoveType::NotThreatened: {
			isRelevantDest = isRelevantDest && (AreBothBoardsEmptyInPosition(dest) && (!isThreatenedInPosition(piece, dest)));
			break;
		}
		case RSPPlayer_309962264::MoveType::Attack:{
			isRelevantDest = isRelevantDest && ((!mPlayersStrategyBoards[mOpponent - 1].IsEmptyInPosition(dest))
				&& (piece.IsStrongerThan(mPlayersStrategyBoards[mOpponent - 1].PeekPieceInPosition(dest)))
				&& !isThreatenedInPosition(piece, dest));
			break;
		}
		case RSPPlayer_309962264::MoveType::Random: {
			break;
		}
		default:{
			isRelevantDest = false;
			break;
		}
	}

	return isRelevantDest;
}

unique_ptr<PointImpl> RSPPlayer_309962264::getStrategyDestination(const StrategyPiece& piece, const PointImpl& from, MoveType moveType) const
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

bool RSPPlayer_309962264::isPieceToMove(const StrategyPiece& strategyPiece, RSPPlayer_309962264::MoveType moveType) const
{
	bool isRelevantPiece = strategyPiece.GetIsMovingPiece();
	switch (moveType)
	{
		case RSPPlayer_309962264::MoveType::RunAway:
		{
			// Joker doesn't run away, it can change it representation.
			isRelevantPiece = isRelevantPiece && strategyPiece.GetIsThreatened() 
				&& (strategyPiece.GetPieceType() != PieceType::Joker);
			break;
		}
		case RSPPlayer_309962264::MoveType::Attack:
		{
			isRelevantPiece = isRelevantPiece && strategyPiece.GetIsThreathening();
			break;
		}
		case RSPPlayer_309962264::MoveType::NotThreatened:
		case RSPPlayer_309962264::MoveType::Random:
		{
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

unique_ptr<Move> RSPPlayer_309962264::getStrategyMoveInPosition(RSPPlayer_309962264::MoveType moveType, int row, int col) const
{
	PointImpl posFrom(col, row);
	if (!mPlayersStrategyBoards[mPlayer - 1].IsEmptyInPosition(posFrom))
	{
		const StrategyPiece& strategyPiece = mPlayersStrategyBoards[mPlayer - 1].PeekPieceInPosition(posFrom);

		if (isPieceToMove(strategyPiece, moveType))
		{
			unique_ptr<PointImpl> posTo = getStrategyDestination(strategyPiece, posFrom, moveType);

			if (posTo != nullptr)
			{
				//lastMovedPieceID = strategyPiece.GetStrategyPieceID();
				return std::make_unique<MoveImpl>(posFrom, *posTo);
			}
		}
	}

	return nullptr;
}

unique_ptr<Move> RSPPlayer_309962264::getStrategyMove(RSPPlayer_309962264::MoveType moveType) {
	std::vector<int> rows, cols;

	for (int row = 1; row <= M; row++){
		rows.push_back(row);
	}
	for (int col = 1; col <= M; col++){
		cols.push_back(col);
	}

	std::shuffle(rows.begin(), rows.end(), mRandGen);

	for (int row : rows)
	{
		std::shuffle(cols.begin(), cols.end(), mRandGen);

		for (int col : cols)
		{
			unique_ptr<Move> posTo = getStrategyMoveInPosition(moveType, row, col);
			if (posTo != nullptr) {
				return posTo;
			}
		}
	}

	return nullptr;
}

bool RSPPlayer_309962264::isThreatenedInPosition(const StrategyPiece& piece, const PointImpl& pos) const
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
bool RSPPlayer_309962264::isThreateningInPosition(const StrategyPiece& piece, const PointImpl& pos) const
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

unique_ptr<JokerChange> RSPPlayer_309962264::getJokerChange()
{
	unique_ptr<JokerChange> jokerChange = nullptr;
	for (const Point& jokerPos : mPlayerJokerLocations) {
		if (!mPlayersStrategyBoards[mPlayer - 1].IsEmptyInPosition(jokerPos)) {
			StrategyPiece& strategyPiece = mPlayersStrategyBoards[mPlayer - 1].PeekPieceInPosition(jokerPos);

			if (strategyPiece.GetIsThreatened()) {
				jokerChange = changeThreatenedJoker(jokerPos);
				updateThreats();
			}
		}
	}

	return jokerChange;
}

unique_ptr<JokerChange> RSPPlayer_309962264::changeThreatenedJoker(const Point& threatenedJokerPos){
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
