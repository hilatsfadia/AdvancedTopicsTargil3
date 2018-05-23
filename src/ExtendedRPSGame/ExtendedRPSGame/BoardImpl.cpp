#include "stdafx.h"
#include "BoardImpl.h"
#include <cctype>
#include <iostream>
#include "Player.h"
#include "FightInfoImpl.h"
#include "JokerChange.h"
#include "StrategyPiece.h"

template <typename T>
bool BoardImpl<T>::PutPieceOnTempPlayerBoard(unique_ptr<T> piece, const Point& pos) { // TODO: error handling, handle out of range + already position taken
	if ((piece == nullptr) || !CheckIfValidPosition(pos))
	{
		// TODO: ask
		// Shouldn't be nullptr.
		std::cout << "Position is out of range" << std::endl;
		return false;
	}

	BoardImpl<T>::BoardSquare<T>& boardSquare = GetBoardInPosition(pos);

	// if board square is empty
	if (boardSquare.IsEmpty())
	{
		boardSquare.ChangeSquarePiece(std::move(piece));
	}
	else
	{
		// Two or more PIECEs (of same player) are positioned on same location

		//if (boardSquare.GetPiece()->GetOwner() == piece->GetOwner())
		//{
		std::cout << "Two PIECEs of same player are positioned on same location" << std::endl;
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

			if ((!player1BoardSquare.IsEmpty()) && (!player2BoardSquare.IsEmpty()))
			{
				Piece::WinningPiece winner = player1Piece.Fight(player2Piece);
				vectorToFill.push_back(std::make_unique<FightInfoImpl>(PointImpl(col, row), player1Piece.GetPieceChar(), player2Piece.GetPieceChar(), (int)winner));

				switch (winner)
				{
					case (Piece::WinningPiece::CallingObject):
					{
						boardSquare.MovePieceFromSquare(player1BoardSquare);
						break;
					}
					case (Piece::WinningPiece::OtherObject):
					{
						boardSquare.MovePieceFromSquare(player2BoardSquare);
						break;
					}
					default:
					{
						break;
					}
				}
			}
			else if (!player1BoardSquare.IsEmpty())
			{
				boardSquare.MovePieceFromSquare(player1BoardSquare);
			}
			else if (!player2BoardSquare.IsEmpty())
			{
				boardSquare.MovePieceFromSquare(player2BoardSquare);
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
T* BoardImpl<T>::GetPieceOfPlayer(const Point& position, int playerNum)
{
	if (!CheckIfValidPosition(position))
	{
		return nullptr;
	}

	BoardImpl<T>::BoardSquare<T>& boardSquareSource = GetBoardInPosition(position);

	if (boardSquareSource.IsEmpty())
	{
		std::cout << "The moving is illegal because the source position is empty." << std::endl;
		return nullptr;
	}

	T* pieceSource = boardSquareSource.GetPiece();

	// Piece position that doesn't have a piece owned by this player
	if (pieceSource->GetOwner()->GetPlayerNum() != playerNum)
	{
		std::cout << "The relevant piece is of the other player." << std::endl;
		return nullptr;
	}

	return pieceSource;
}
//
//bool BoardImpl::IsJokerChangeLegal(const JokerChange& jokerChange)
//{
//	if (!CheckIfValidPosition(jokerChange.getJokerChangePosition()))
//	{
//		return false;
//	}
//
//	return false;
//}

template <typename T>
bool BoardImpl<T>::MovePiece(const Player& player, const Point& posFrom, const Point& posTo, FightInfoImpl& toFill)
{
	BoardImpl<T>::BoardSquare<T>& boardSquareSource = GetBoardInPosition(posFrom);
	BoardImpl<T>::BoardSquare<T>& boardSquareDestination = GetBoardInPosition(posTo);

	Piece* pieceSource = GetPieceOfPlayer(posFrom, player.GetPlayerNum());

	if (pieceSource == nullptr)
	{
		std::cout << "The moving is illegal because given source position is illegal." << std::endl;
		return false;
	}

	if (!IsLegalMoveDestination(posFrom, posTo))
	{
		std::cout << "The moving is illegal because given destination position is illegal." << std::endl;
		return false;
	}

	if (!pieceSource->GetIsMovingPiece())
	{
		std::cout << "The moving is illegal because the relevant piece cannot move." << std::endl;
		return false;
	}

	if (boardSquareDestination.IsEmpty())
	{
		boardSquareDestination.MovePieceFromSquare(boardSquareSource);
	}
	else
	{
		Piece& pieceDestination = boardSquareDestination.PeekPiece();

		if (pieceSource->GetOwner() == pieceDestination.GetOwner())
		{
			std::cout << "The moving is illegal because the destination has a piece of the same player" << std::endl;
			return false;
		}

		// If we got here than there should be a fight.
		// TODO: refactor (look positioning)
		Piece::WinningPiece winningPiece = pieceDestination.Fight(*pieceSource);
		int winner;
		char sourceChar = pieceSource->GetPieceChar();
		char destinationChar = pieceDestination.GetPieceChar();

		switch (winningPiece)
		{
			case (Piece::WinningPiece::CallingObject):
			{
				winner = pieceDestination.GetOwner()->GetPlayerNum();
				boardSquareSource.ClearSquare();
				break;
			}
			case (Piece::WinningPiece::OtherObject):
			{
				winner = pieceSource->GetOwner()->GetPlayerNum();
				boardSquareDestination.MovePieceFromSquare(boardSquareSource);
				break;
			}
			default:
			{
				winner = 0;
				break;
			}
		}

		if (pieceSource->GetOwner()->GetPlayerNum() == 1)
		{
			toFill.SetFightInfoValues(posTo, sourceChar, destinationChar, winner);
		}
		else
		{
			toFill.SetFightInfoValues(posTo, destinationChar, sourceChar, winner);
		}
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
//
//template <typename T>
//BoardImpl<T>::BoardSquare<T>& BoardImpl<T>::GetBoardInPosition(int x, int y){
//    return board[y-1][x-1];
//}
//
//template <typename T>
//const BoardImpl<T>::BoardSquare<T>& BoardImpl<T>::GetBoardInPosition(int x, int y) const
//{
//	return board[y-1][x-1];
//}
//
template <typename T>
BoardImpl<T>::BoardSquare<T>& BoardImpl<T>::GetBoardInPosition(const Point& position){
    return GetBoardInPosition(position.getX(), position.getY());
}

//template <typename T>
//const BoardImpl<T>::BoardSquare<T>& BoardImpl<T>::GetBoardInPosition(const Point& position) const
//{
//	return GetBoardInPosition(position.getX(), position.getY());
//}

template <typename T>
bool BoardImpl<T>::CheckIfValidPosition(const Point& position) const
{
	return ((position.getX() <= mColumns) && (position.getX() >= 1) &&
			(position.getY() <= mRows) && (position.getY() >= 1));
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

// TODO: impl!
//template <typename T>
//void BoardImpl<T>::BoardSquare<T>::MovePieceFromSquare(BoardImpl<T>::BoardSquare<T>& other)
//{
//	this->piece = std::move(other.piece);
//	other.ClearSquare();
//}

template class BoardImpl<Piece>;
template class BoardImpl<StrategyPiece>;