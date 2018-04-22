#include "stdafx.h"
#include "Board.h"
#include <cctype>
#include <iostream>

Board::~Board(){
    // free pieces on heap
	for (int row = 0; row < mRows; row++)
	{
		for (int col = 0; col < mColumns; col++)
		{
			delete GetBoardInPosition(row, col).GetPiece();
		}
	}
}


bool Board::PutPieceOnBoard(Piece* piece, const BoardPosition& pos) { // TODO: error handling, handle out of range + already position taken
	if ((piece == nullptr) || !CheckIfValidPosition(pos))
	{
		// TODO: ask
		// Shouldn't be nullptr.
		std::cout << "Position is out of range" << std::endl;
		return false;
	}

	Board::BoardSquare& boardSquare = GetBoardInPosition(pos);

	// if board square is empty
	if (boardSquare.IsEmpty())
	{
		boardSquare.ChangeSquarePiece(piece);
	}
	else
	{
		// Two or more PIECEs (of same player) are positioned on same location
		if (boardSquare.GetPiece()->GetOwner() == piece->GetOwner())
		{
			std::cout << "Two PIECEs of same player are positioned on same location" << std::endl;
			return false;
		}

		boardSquare.ChangeSquarePiece(boardSquare.GetPiece()->Fight(piece));
	}

	return true;
}

bool Board::IsMovePieceLegal(const Board::BoardPosition& posFrom, const Board::BoardPosition& posTo) const
{
	if (!CheckIfValidPosition(posFrom) || !CheckIfValidPosition(posTo))
	{
		// TODO: ask
		return false;
	}

	int horizontalDiff = abs(posFrom.x - posTo.x);
	int verticalDiff = abs(posFrom.y - posTo.y);
	
	bool isMoveAtMostOneSquareInAxis = (horizontalDiff <= 1) && (verticalDiff <= 1);
	bool isMoveInDiagonal = (horizontalDiff == 1) && (verticalDiff == 1);

	return isMoveAtMostOneSquareInAxis && (!isMoveInDiagonal);
}

bool Board::MovePiece(const Board::BoardPosition& posFrom, const Board::BoardPosition& posTo)
{
	if (!IsMovePieceLegal(posFrom, posTo))
	{
		std::cout << "The moving is illegal because given positions are illegal." << std::endl;
		return false;
	}

	Board::BoardSquare& boardSquareSource = GetBoardInPosition(posFrom);
	Board::BoardSquare& boardSquareDestination = GetBoardInPosition(posTo);
	Piece* pieceSource = boardSquareSource.GetPiece();

	if (!pieceSource->isMovingPiece())
	{
		std::cout << "The moving is illegal because the relevant piece cannot move." << std::endl;
		return false;
	}

	if (boardSquareDestination.IsEmpty())
	{
		boardSquareDestination.ChangeSquarePiece(pieceSource);
	}
	else
	{
		Piece* pieceDestination = boardSquareDestination.GetPiece();

		// Two or more PIECEs (of same player) are positioned on same location
		if (pieceSource->GetOwner() == pieceDestination->GetOwner())
		{
			std::cout << "The moving is illegal because the destination has a piece of the same player" << std::endl;
			return false;
		}

		boardSquareDestination.ChangeSquarePiece(pieceDestination->Fight(pieceSource));
	}
	boardSquareSource.ClearSquare();

	return true;
}

Board::BoardSquare& Board::GetBoardInPosition(int y, int x){
    return board[y-1][x-1];
}
	
Board::BoardSquare& Board::GetBoardInPosition(const BoardPosition& position){
    return GetBoardInPosition(position.y, position.x);
}

bool Board::CheckIfValidPosition(const BoardPosition& position) const
{
	return ((position.x <= mColumns) && (position.x >= 1) && 
		(position.y <= mRows) && (position.y >= 1));
}

void Board::Print(std::ostream& outFile)
{
	for (int row = 1; row <= GetRowsNum(); row++)
	{
		for (int col = 1; col <= GetColsNum(); col++)
		{
			Piece* currPiece = GetBoardInPosition(row, col).GetPiece();
			if (currPiece == nullptr)
			{
				outFile << " ";
			}
			else
			{
				outFile << *GetBoardInPosition(row, col).GetPiece();
			}
		}

		outFile << std::endl;
	}
}

int Board::GetRowsNum() const
{
	return this->mRows;
}

int Board::GetColsNum() const
{
	return this->mColumns;
}
