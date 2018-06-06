#include "BoardImpl.h"
#include <iostream>
#include "Player.h"
#include "FightInfoImpl.h"
#include "JokerChange.h"
#include "StrategyPiece.h"

// Type Alias
using WinningPiece = Piece::WinningPiece;

template <typename T>
bool BoardImpl<T>::PutPieceOnSinglePlayerBoard(unique_ptr<T> piece, const Point& pos) { // TODO: error handling, handle out of range + already position taken
	if ((piece == nullptr) || !CheckIfValidPosition(pos))
	{
		// TODO: ask. Shouldn't be nullptr.
		// Written in the forum to put in comment
		//std::cout << "Position is out of range" << std::endl;
		return false;
	}

	BoardImpl<T>::BoardSquare<T>& boardSquare = GetBoardInPosition(pos);

	// if board square is empty
	if (boardSquare.IsEmpty()){
		boardSquare.ChangeSquarePiece(std::move(piece));
	}
	else{
		// Two or more PIECEs (of same player) are positioned on same location

		//if (boardSquare.GetPiece()->GetOwner() == piece->GetOwner())
		//{
		// Written in the forum to put in comment
		//std::cout << "Two PIECEs of same player are positioned on same location" << std::endl;
		return false;
		//}

		//boardSquare.ChangeSquarePiece(piece);
	}

	return true;
}

template <typename T>
void BoardImpl<T>::InitByTempBoards(BoardImpl<T>& player1Board, BoardImpl<T>& player2Board, std::vector<unique_ptr<FightInfo>>& vectorToFill)
{
	for (int row = 1; row <= GetRowsNum(); row++)
	{
		for (int col = 1; col <= GetColsNum(); col++)
		{
			BoardSquare<T>& player1BoardSquare = player1Board.GetBoardInPosition(col, row);
			Piece& player1Piece = player1BoardSquare.PeekPiece();
			BoardSquare<T>& player2BoardSquare = player2Board.GetBoardInPosition(col, row);
			Piece& player2Piece = player2BoardSquare.PeekPiece();
			BoardSquare<T>& boardSquare = this->GetBoardInPosition(col, row);

			if ((!player1BoardSquare.IsEmpty()) && (!player2BoardSquare.IsEmpty())){
				WinningPiece winner = player1Piece.Fight(player2Piece);
				vectorToFill.push_back(std::make_unique<FightInfoImpl>(PointImpl(col, row), player1Piece.GetActualPieceChar(), player2Piece.GetActualPieceChar(), (int)winner));
				switch (winner){
					case (WinningPiece::CallingObject):{
						boardSquare.StealPieceFromSquare(player1BoardSquare);
						break;
					}
					case (WinningPiece::OtherObject):{
						boardSquare.StealPieceFromSquare(player2BoardSquare);
						break;
					}
					default:{
						break;
					}
				}
			}
			else if (!player1BoardSquare.IsEmpty()){
				boardSquare.StealPieceFromSquare(player1BoardSquare);
			}
			else if (!player2BoardSquare.IsEmpty()){
				boardSquare.StealPieceFromSquare(player2BoardSquare);
			}
		}
	}
}

template <typename T>
bool BoardImpl<T>::IsLegalMoveDestination(const Point& posFrom, const Point& posTo) const
{
	if (!CheckIfValidPosition(posTo))
	{
		// TODO: ask
		return false;
	}

	int verticalDiff = abs(posFrom.getY() - posTo.getY());
	int horizontalDiff = abs(posFrom.getX() - posTo.getX());
	
	bool isMoveAtMostOneSquareInAxis = (verticalDiff <= 1) && (horizontalDiff <= 1);
	bool isMoveInDiagonal = (verticalDiff == 1) && (horizontalDiff == 1);

	return isMoveAtMostOneSquareInAxis && (!isMoveInDiagonal);
}

template <typename T>
T* BoardImpl<T>::GetPiece(const Point& position)
{
	return GetBoardInPosition(position).GetPiece();
}

template<typename T>
bool BoardImpl<T>::CheckGetPieceOfPlayer(const Point& position, int playerNum) const
{
	if (!CheckIfValidPosition(position))
	{
		return false;
	}

	const BoardImpl<T>::BoardSquare<T>& boardSquareSource = GetBoardInPosition(position);

	if (boardSquareSource.IsEmpty())
	{
		// Written in the forum to put in comment
		//std::cout << "The moving is illegal because the source position is empty." << std::endl;
		return false;
	}

	const T& pieceSource = boardSquareSource.PeekPiece();

	// Piece position that doesn't have a piece owned by this player
	if (pieceSource.GetOwner()->GetPlayerNum() != playerNum)
	{
		// Written in the forum to put in comment
		//std::cout << "The relevant piece is of the other player." << std::endl;
		return false;
	}

	return true;
}

// Written in the forum to put printing to console in comment
template<typename T>
bool BoardImpl<T>::ChecksIfMoveIsLegal(int playerNum, const Point& posFrom, const Point& posTo) const
{
	if ((!CheckIfValidPosition(posFrom)) || (!CheckIfValidPosition(posTo))){
		return false;
	}

	if (!CheckGetPieceOfPlayer(posFrom, playerNum)){
		return false;
	}

	const Piece& pieceAttacker = PeekPieceInPosition(posFrom);

	if (!IsLegalMoveDestination(posFrom, posTo)){
		//std::cout << "The moving is illegal because given destination position is illegal." << std::endl;
		return false;
	}
	if (!pieceAttacker.GetIsMovingPiece()){
		//std::cout << "The moving is illegal because the relevant piece cannot move." << std::endl;
		return false;
	}

	const BoardImpl<T>::BoardSquare<T>& boardSquareAttackee = GetBoardInPosition(posTo);

	if (!boardSquareAttackee.IsEmpty()){
		const Piece& pieceAttackee = boardSquareAttackee.PeekPiece();
		if (pieceAttacker.GetOwner() == pieceAttackee.GetOwner()){
			//std::cout << "The moving is illegal because the destination has a piece of the same player" << std::endl;
			return false;
		}
	}

	return true;
}

template<typename T>
int BoardImpl<T>::HandleFight(BoardImpl<T>::BoardSquare<T>& boardSquareAttacker, 
	BoardImpl<T>::BoardSquare<T>& boardSquareAttackee)
{
	Piece& pieceAttacker = boardSquareAttacker.PeekPiece();
	Piece& pieceAttackee = boardSquareAttackee.PeekPiece();
	WinningPiece winningPiece = pieceAttacker.Fight(pieceAttackee);
	int winner;

	switch (winningPiece)
	{
		case (WinningPiece::CallingObject):{
			winner = pieceAttacker.GetOwner()->GetPlayerNum();
			boardSquareAttackee.StealPieceFromSquare(boardSquareAttacker);
			break;
		}
		case (WinningPiece::OtherObject):{
			winner = pieceAttackee.GetOwner()->GetPlayerNum();
			boardSquareAttacker.ClearSquare();
			break;
		}
		case (WinningPiece::Tie):{
			winner = 0;
			boardSquareAttacker.ClearSquare();
			boardSquareAttackee.ClearSquare();
			break;
		}
		default:{
			winner = 0;
			break;
		}
	}

	return winner;
}

template <typename T>
bool BoardImpl<T>::MovePiece(const Player& player, const Point& posFrom, const Point& posTo, FightInfoImpl& toFill)
{
	if (!ChecksIfMoveIsLegal(player.GetPlayerNum(), posFrom, posTo)){
		return false;
	}

	BoardImpl<T>::BoardSquare<T>& boardSquareAttacker = GetBoardInPosition(posFrom);
	BoardImpl<T>::BoardSquare<T>& boardSquareAttackee = GetBoardInPosition(posTo);

	if (boardSquareAttackee.IsEmpty()){
		boardSquareAttackee.StealPieceFromSquare(boardSquareAttacker);
	}
	else
	{// If we got here than there should be a fight.
		const Piece& pieceAttacker = boardSquareAttacker.PeekPiece();
		const Piece& pieceAttackee = boardSquareAttackee.PeekPiece();
		char attackerChar = pieceAttacker.GetActualPieceChar();
		char attackeeChar = pieceAttackee.GetActualPieceChar();
		char player1Char, player2Char;

		if (pieceAttacker.GetOwner()->GetPlayerNum() == 1){
			player1Char = attackerChar;
			player2Char = attackeeChar;
		}
		else{
			player1Char = attackeeChar;
			player2Char = attackerChar;
		}

		int winner = HandleFight(boardSquareAttacker, boardSquareAttackee);
		toFill.SetFightInfoValues(posTo, player1Char, player2Char, winner);
	}

	return true;
}

template <typename T>
bool BoardImpl<T>::MovePiece(const Player& player, const unique_ptr<Move>& move, FightInfoImpl& toFill)
{
	if (move != nullptr)
	{
		return MovePiece(player, move->getFrom(), move->getTo(), toFill);
	}
	else
	{
		return false;
	}
}

template <typename T>
BoardImpl<T>::BoardSquare<T>& BoardImpl<T>::GetBoardInPosition(const Point& position){
    return GetBoardInPosition(position.getX(), position.getY());
}

template <typename T>
bool BoardImpl<T>::CheckIfValidPosition(const Point& position)
{
	return ((position.getX() <= M) && (position.getX() >= 1) &&
			(position.getY() <= N) && (position.getY() >= 1));
}

template <typename T>
void BoardImpl<T>::Print(std::ostream& outFile) const
{
	for (int row = 1; row <= GetRowsNum(); row++)
	{
		for (int col = 1; col <= GetColsNum(); col++)
		{
			if (GetBoardInPosition(col, row).IsEmpty())
			{
				outFile << " ";
			}
			else
			{
				outFile << GetBoardInPosition(col, row);
			}
		}

		outFile << std::endl;
	}
}

template <typename T>
int BoardImpl<T>::getPlayer(const Point& pos) const
{
	const BoardSquare<T>& square = GetBoardInPosition(pos);
	if (square.IsEmpty())
	{
		return 0;
	}

	return square.PeekPiece().GetOwner()->GetPlayerNum();
}

template class BoardImpl<Piece>;
template class BoardImpl<StrategyPiece>;