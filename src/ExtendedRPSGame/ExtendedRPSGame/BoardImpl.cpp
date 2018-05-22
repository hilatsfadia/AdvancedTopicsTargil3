#include "stdafx.h"
#include "BoardImpl.h"
#include <cctype>
#include <iostream>
#include "Player.h"
#include "FightInfoImpl.h"
#include "JokerChange.h"

BoardImpl::~BoardImpl(){
	// TODO: using unique ptr
 //   // free pieces on heap
	//for (int row = 1; row <= mRows; row++)
	//{
	//	for (int col = 1; col <= mColumns; col++)
	//	{
	//		delete GetBoardInPosition(row, col).GetPiece();
	//		//GetBoardInPosition(row, col).ChangeSquarePiece(nullptr);
	//	}
	//}
}

bool BoardImpl::PutPieceOnTempPlayerBoard(unique_ptr<Piece> piece, const Point& pos) { // TODO: error handling, handle out of range + already position taken
	if ((piece == nullptr) || !CheckIfValidPosition(pos))
	{
		// TODO: ask
		// Shouldn't be nullptr.
		std::cout << "Position is out of range" << std::endl;
		return false;
	}

	BoardImpl::BoardSquare& boardSquare = GetBoardInPosition(pos);

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

void BoardImpl::InitByTempBoards(BoardImpl& player1Board, BoardImpl& player2Board, std::vector<unique_ptr<FightInfo>>& vectorToFill)
{
	for (int row = 1; row <= GetRowsNum(); row++)
	{
		for (int col = 1; col <= GetColsNum(); col++)
		{
			BoardSquare& player1BoardSquare = player1Board.GetBoardInPosition(col, row);
			Piece& player1Piece = player1BoardSquare.PeekPiece();
			BoardSquare& player2BoardSquare = player2Board.GetBoardInPosition(col, row);
			Piece& player2Piece = player2BoardSquare.PeekPiece();
			BoardSquare& boardSquare = this->GetBoardInPosition(col, row);

			if ((!player1BoardSquare.IsEmpty()) && (!player2BoardSquare.IsEmpty()))
			{
				Piece::WinningPiece winner = player1Piece.Fight(player2Piece);
				vectorToFill.push_back(std::make_unique<FightInfoImpl>(PointImpl(col, row), player1Piece.GetPieceChar(), player2Piece.GetPieceChar(), (int)winner));

				switch (winner)
				{
					case (Piece::WinningPiece::ThisPiece):
					{
						boardSquare.MovePieceFromSquare(player1BoardSquare);
						break;
					}
					case (Piece::WinningPiece::enemy):
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

bool BoardImpl::IsLegalMoveDestination(const Point& posFrom, const Point& posTo) const
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

Piece* BoardImpl::GetPieceOfPlayer(const Point& position, int playerNum)
{
	if (!CheckIfValidPosition(position))
	{
		return nullptr;
	}

	BoardImpl::BoardSquare& boardSquareSource = GetBoardInPosition(position);

	if (boardSquareSource.IsEmpty())
	{
		std::cout << "The moving is illegal because the source position is empty." << std::endl;
		return nullptr;
	}

	Piece* pieceSource = boardSquareSource.GetPiece();

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

bool BoardImpl::MovePiece(const Player& player, const Point& posFrom, const Point& posTo, FightInfoImpl& toFill)
{
	BoardImpl::BoardSquare& boardSquareSource = GetBoardInPosition(posFrom);
	BoardImpl::BoardSquare& boardSquareDestination = GetBoardInPosition(posTo);

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

		switch (winningPiece)
		{
			case (Piece::WinningPiece::ThisPiece):
			{
				winner = pieceDestination.GetOwner()->GetPlayerNum();
				break;
			}
			case (Piece::WinningPiece::enemy):
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

		toFill.SetFightInfoValues(posTo, pieceSource->GetPieceChar(), pieceDestination.GetPieceChar(), winner);
	}

	GetBoardInPosition(posFrom).ClearSquare();

	return true;
}

bool BoardImpl::MovePiece(const Player& player, const unique_ptr<Move>& move, FightInfoImpl& toFill)
{
	return MovePiece(player, move->getFrom(), move->getTo(), toFill);
}

BoardImpl::BoardSquare& BoardImpl::GetBoardInPosition(int x, int y){
    return board[y-1][x-1];
}

const BoardImpl::BoardSquare& BoardImpl::GetBoardInPosition(int x, int y) const
{
	return board[y-1][x-1];
}

BoardImpl::BoardSquare& BoardImpl::GetBoardInPosition(const Point& position){
    return GetBoardInPosition(position.getX(), position.getY());
}

const BoardImpl::BoardSquare& BoardImpl::GetBoardInPosition(const Point& position) const
{
	return GetBoardInPosition(position.getX(), position.getY());
}

bool BoardImpl::CheckIfValidPosition(const Point& position) const
{
	return ((position.getX() <= mColumns) && (position.getX() >= 1) &&
			(position.getY() <= mRows) && (position.getY() >= 1));
}

void BoardImpl::Print(std::ostream& outFile) const
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

int BoardImpl::GetRowsNum() const
{
	return this->mRows;
}

int BoardImpl::GetColsNum() const
{
	return this->mColumns;
}

int BoardImpl::getPlayer(const Point& pos) const
{
	const BoardSquare& square = GetBoardInPosition(pos);
	if (square.IsEmpty())
	{
		return 0;
	}

	return square.PeekPiece().GetOwner()->GetPlayerNum();
}

void BoardImpl::BoardSquare::MovePieceFromSquare(BoardSquare & other)
{
	this->piece = std::move(other.piece);
	other.ClearSquare();
}

