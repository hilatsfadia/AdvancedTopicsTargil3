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

bool BoardImpl::PutPieceOnTempPlayerBoard(Piece* piece, const Point& pos) { // TODO: error handling, handle out of range + already position taken
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
		boardSquare.ChangeSquarePiece(piece);
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
			Piece* player1Piece = player1Board.GetBoardInPosition(col, row).GetPiece();
			Piece* player2Piece = player2Board.GetBoardInPosition(col, row).GetPiece();
			BoardSquare& boardSquare = this->GetBoardInPosition(col, row);

			if ((player1Piece != nullptr) && (player2Piece != nullptr))
			{
				Piece* winningPiece = player1Piece->Fight(player2Piece);
				PointImpl* pos = new PointImpl(col, row);

				int winner = (winningPiece == nullptr) ? 0 : winningPiece->GetOwner()->GetPlayerNum();
				vectorToFill.push_back(std::make_unique<FightInfoImpl>(pos, player1Piece->GetPieceChar(), player2Piece->GetPieceChar(), winner));
				boardSquare.ChangeSquarePiece(winningPiece);
			}
			else if (player1Piece != nullptr)
			{
				boardSquare.ChangeSquarePiece(player1Piece);
			}
			else if (player2Piece != nullptr)
			{
				boardSquare.ChangeSquarePiece(player2Piece);
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

bool BoardImpl::MovePiece(const Player& player, const Point& posFrom, const Point& posTo, FightInfo* toFill)
{
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

	BoardImpl::BoardSquare& boardSquareDestination = GetBoardInPosition(posTo);

	if (boardSquareDestination.IsEmpty())
	{
		boardSquareDestination.ChangeSquarePiece(pieceSource);
	}
	else
	{
		Piece* pieceDestination = boardSquareDestination.GetPiece();

		if (pieceSource->GetOwner() == pieceDestination->GetOwner())
		{
			std::cout << "The moving is illegal because the destination has a piece of the same player" << std::endl;
			return false;
		}

		// If we got here than there should be a fight.
		// TODO: refactor (look positioning)
		Piece* winningPiece = pieceDestination->Fight(pieceSource);
		int winner = (winningPiece == nullptr) ? 0 : winningPiece->GetOwner()->GetPlayerNum();

		toFill = new FightInfoImpl(&posTo, pieceSource->GetPieceChar(), pieceDestination->GetPieceChar(), winner);
		boardSquareDestination.ChangeSquarePiece(winningPiece);
	}

	GetBoardInPosition(posFrom).ClearSquare();

	return true;
}

bool BoardImpl::MovePiece(const Player& player, const unique_ptr<Move>& move, FightInfo* toFill)
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
			const Piece* currPiece = GetBoardInPosition(col, row).GetPiece();
			if (currPiece == nullptr)
			{
				outFile << " ";
			}
			else
			{
				//Piece* temp = GetBoardInPosition(col, row).GetPiece();
				outFile << *GetBoardInPosition(col, row).GetPiece();
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
	BoardSquare square = GetBoardInPosition(pos);
	if (square.IsEmpty())
	{
		return 0;
	}

	return square.GetPiece()->GetOwner()->GetPlayerNum();
}