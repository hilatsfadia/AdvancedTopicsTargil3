#ifndef ADTO_TARGIL1_BOARD_H
#define ADTO_TARGIL1_BOARD_H

#include <stdio.h>
#include <vector>
#include "Piece.h"
#include "Board.h"
#include "PointImpl.h"
#include <memory>

class Move;

#define BAD_INPUT_MSG "ERROR: The file has bad input. The problem: %s"
#define MISSING_FILE_MSG "ERROR: The input file is missing. Please input a file to continue"
#define CANT_OPEN_FILE_MSG "ERROR: The input file can't be opened. Please check the file, or try again"
#define BAD_FORMAT
//#define "A PIECE type appears in file more than its number"
//#define "Two or more PIECEs (of same player) are positioned on same location"
//#define "X coordinate and/or Y coordinate of one or more PIECE is not in range"
//#define "Missing flags! Flags are not positioned according to their number"

#define M 10
#define N 10

class BoardImpl : public Board {
private:
	// The board consists of M * N objects of this type.
	class BoardSquare {
	private:
		// The piece in this square if exists, otherwise nullptr.
		Piece* piece = nullptr;
	public:
		// If this square has no piece
		bool IsEmpty() 
		{
			return (piece == nullptr);
		}

		// Remove piece from this square
		void ClearSquare() {
			piece = nullptr;
		}

		// Put other piece in this square
		void ChangeSquarePiece(Piece* newPiece) {
			piece = newPiece;
		}

		// Get this square piece
		Piece* GetPiece() { return piece; }
	};

	const int mRows = N;
	const int mColumns = M;
	BoardSquare board[N][M];

public:

	~BoardImpl();

	// Tries to the piece in the given position. 
	// Returns true if the piece can be put in the position.
	// Maybe requires fight.
	bool PutPieceOnBoard(Piece* piece, const Point& pos);

	// Return true if positions are valid, and that the move is only
	// to adjacent position vertically or horizontally.
	bool IsMovePieceLegal(const Point& posFrom, const Point& posTo) const;

	// Checks if the move is legal. If so, move the piece to the new position.
	// Maybe requires fight.
	bool MovePiece(const Point& posFrom, const Point& posTo);
	// Checks if the move is legal. If so, move the piece to the new position.
	// Maybe requires fight.
	bool MovePiece(const std::unique_ptr<Move>& move);

	// Get Board in the given position, when axis values start from 1.
	BoardSquare& GetBoardInPosition(const Point& position);

	// TODO: look at const overloading
	const BoardSquare& GetBoardInPosition(const Point& position) const;

	// Get Board in (x,x), when axis values start from 1.
	BoardSquare& GetBoardInPosition(int x, int y);

	// TODO: look at const overloading
	const BoardSquare& GetBoardInPosition(int x, int y) const;

	// Checks if the position isn't out of range.
	bool CheckIfValidPosition(const Point& position) const;

	// Prints the board to the console.
	void Print(std::ostream& outFile);

	// Gets board's rows count
	int GetRowsNum() const;

	// Gets board's columns count
	int GetColsNum() const;

	virtual int getPlayer(const Point& pos) const; // 1 for player 1�s piece, 2 for 2, 0 if empty
};

#endif //ADTO_TARGIL1_BOARD_H
