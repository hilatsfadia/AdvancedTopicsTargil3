#ifndef ADTO_TARGIL1_BOARD_H
#define ADTO_TARGIL1_BOARD_H

#include <stdio.h>
#include <vector>
#include "Piece.h"

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

// TODO: name
class ConcreteBoard {
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
	// Represents a position in the board.
	struct BoardPosition { 
		int y;
		int x;

		BoardPosition() : y(1), x(1) {}
		BoardPosition(int y, int x) : y(y), x(x) {}
	};

	~ConcreteBoard();

	// Tries to the piece in the given position. 
	// Returns true if the piece can be put in the position.
	// Maybe requires fight.
	bool PutPieceOnBoard(Piece* piece, const BoardPosition& pos);

	// Return true if positions are valid, and that the move is only
	// to adjacent position vertically or horizontally.
	bool IsMovePieceLegal(const BoardPosition& posFrom, const BoardPosition& posTo) const;

	// Checks if the move is legal. If so, move the piece to the new position.
	// Maybe requires fight.
	bool MovePiece(const BoardPosition& posFrom, const BoardPosition& posTo);

	// Get Board in the given position, when axis values start from 1.
	BoardSquare& GetBoardInPosition(const BoardPosition& position);

	// Get Board in (x,y), when axis values start from 1.
	BoardSquare& GetBoardInPosition(int x, int y);

	// Checks if the position isn't out of range.
	bool CheckIfValidPosition(const BoardPosition& position) const;

	// Prints the board to the console.
	void Print(std::ostream& outFile);

	// Gets board's rows count
	int GetRowsNum() const;

	// Gets board's columns count
	int GetColsNum() const;
};

#endif //ADTO_TARGIL1_BOARD_H
