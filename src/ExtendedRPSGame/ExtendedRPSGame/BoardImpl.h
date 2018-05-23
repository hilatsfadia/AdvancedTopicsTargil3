#ifndef ADTO_TARGIL1_BOARD_H
#define ADTO_TARGIL1_BOARD_H

#include <stdio.h>
#include <vector>
#include "Piece.h"
#include "Board.h"
#include "PointImpl.h"
#include "FightInfoImpl.h"
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

using std::unique_ptr;

class BoardImpl : public Board {
private:
	// The board consists of M * N objects of this type.
	class BoardSquare {
	private:
		// The piece in this square if exists, otherwise nullptr.
		unique_ptr<Piece> piece = nullptr;
	public:
		// If this square has no piece
		bool IsEmpty() const
		{
			return (piece == nullptr);
		}

		// Remove piece from this square
		void ClearSquare() {
			// The assignment operator deletes the old pointer
			// if existed.
			piece = nullptr;
		}

		// Put other piece in this square
		// Steal its belongings.
		void ChangeSquarePiece(unique_ptr<Piece> newPiece) {
			// The assignment operator deletes the old pointer
			// if existed.
			piece = std::move(newPiece);
		}

		// Get this square piece
		// TODO: delete!
		Piece* GetPiece() { return piece.get(); }

		// Get this square piece
		Piece& PeekPiece() { return *piece; }

		// Get this square piece
		// TODO: maybe weak_ptr?
		const Piece& PeekPiece() const { return *piece; }

		// Init by other boardSquare
		void MovePieceFromSquare(BoardSquare& other);

		// Operator overloading for printing issues.
		friend std::ostream& operator<<(std::ostream& out, const BoardSquare& boardSquare) {
			return out << *boardSquare.piece;
		}
	};

	const int mRows = N;
	const int mColumns = M;
	BoardSquare board[N][M];

public:

	~BoardImpl();

	// Tries to the piece in the given position. 
	// Returns true if the piece can be put in the position.
	// TODO: maybe static
	bool PutPieceOnTempPlayerBoard(unique_ptr<Piece> piece, const Point& pos);

	// Combine the two players' boards to one.
	// Maybe require fights.
	void InitByTempBoards(BoardImpl& player1Board, BoardImpl& player2Board, std::vector<unique_ptr<FightInfo>>& vectorToFill);

	// Return true if positions are valid, and that the move is only
	// to adjacent position vertically or horizontally.
	bool IsLegalMoveDestination(const Point& posFrom, const Point& posTo) const;

	// Returns the piece of the given player in the given location.
	// If there is no piece of the given player in the given location, returns nullptr.
	Piece* GetPieceOfPlayer(const Point& position, int playerNum);

	//bool IsJokerChangeLegal(const JokerChange& jokerChange);

	// Checks if the move is legal. If so, move the piece to the new position.
	// Maybe requires fight.
	bool MovePiece(const Player& player, const Point& posFrom, const Point& posTo, FightInfoImpl& toFill);

	// Checks if the move is legal. If so, move the piece to the new position.
	// Maybe requires fight.
	bool MovePiece(const Player& player, const std::unique_ptr<Move>& move, FightInfoImpl& toFill);

	// It is the caller responsibility to call CheckIfValidPosition before this function 
	void PutPieceInPosition(const Point& position, unique_ptr<Piece> piece);

	// Get Board in the given position, when axis values start from 1.
	// Assumes position is legal.
	BoardSquare& GetBoardInPosition(const Point& position);

	// TODO: look at const overloading
	// Get Board in the given position, when axis values start from 1.
	// Assumes position is legal.
	const BoardSquare& GetBoardInPosition(const Point& position) const;

	// Get Board in (x,x), when axis values start from 1.
	BoardSquare& GetBoardInPosition(int x, int y);

	// TODO: look at const overloading
	const BoardSquare& GetBoardInPosition(int x, int y) const;

	// Checks if the position isn't out of range.
	bool CheckIfValidPosition(const Point& position) const;

	// Prints the board to the console.
	void Print(std::ostream& outFile) const;

	// Gets board's rows count
	int GetRowsNum() const;

	// Gets board's columns count
	int GetColsNum() const;

	virtual int getPlayer(const Point& pos) const; // 1 for player 1�s piece, 2 for 2, 0 if empty
};

#endif //ADTO_TARGIL1_BOARD_H
