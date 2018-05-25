#ifndef ADTO_TARGIL1_BOARD_H
#define ADTO_TARGIL1_BOARD_H

//--------------------------
// BoardImpl Interface
//--------------------------

// Generates and handles the board which the game is played upon.

// @author Hila Tsfadia, Jael Fafner TAU - Advanced Topics in Programming - 2018 Semester B

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

template <typename T>
class BoardImpl : public Board {
private:
	// The board consists of M * N objects of this type.
	template <typename T>
	class BoardSquare {
	private:
		// The piece in this square if exists, otherwise nullptr.
		unique_ptr<T> piece = nullptr;
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
		void ChangeSquarePiece(unique_ptr<T> newPiece) {
			// The assignment operator deletes the old pointer
			// if existed.
			piece = std::move(newPiece);
		}

		// Get this square piece
		// TODO: delete!
		T* GetPiece() { return piece.get(); }

		// Get this square piece
		// TODO! change name!
		T& PeekPiece() { return *piece; }

		// Get this square piece
		// TODO: maybe weak_ptr?
		const T& PeekPiece() const { return *piece; }

		// Init by other boardSquare
		void StealPieceFromSquare(BoardSquare<T>& other)
		{
			this->piece = std::move(other.piece);
			other.ClearSquare();
		}

		// Operator overloading for printing issues.
		friend std::ostream& operator<<(std::ostream& out, const BoardSquare<T>& boardSquare) {
			return out << *boardSquare.piece;
		}
	};

	const int mRows = M;
	const int mColumns = N;
	BoardSquare<T> board[M][N];

	// Get Board in the given position, when axis values start from 1.
	// Assumes position is legal.
	BoardSquare<T>& GetBoardInPosition(const Point& position);
	//{
	//	return GetBoardInPosition(position.getX(), position.getY());
	//}

	// TODO: look at const overloading
	// Get Board in the given position, when axis values start from 1.
	// Assumes position is legal.
	const BoardSquare<T>& GetBoardInPosition(const Point& position) const
	{
		return GetBoardInPosition(position.getX(), position.getY());
	}

	// Get Board in (x,x), when axis values start from 1.
	BoardSquare<T>& GetBoardInPosition(int x, int y)
	{
		return board[y - 1][x - 1];
	}

	// TODO: look at const overloading
	const BoardSquare<T>& GetBoardInPosition(int x, int y) const
	{
		return board[y - 1][x - 1];
	}

public:

	// Tries to the piece in the given position. 
	// Returns true if the piece can be put in the position.
	// TODO: maybe static
	bool PutPieceOnSinglePlayerBoard(unique_ptr<T> piece, const Point& pos);

	// Combine the two players' boards to one.
	// Maybe require fights.
	void InitByTempBoards(BoardImpl<T>& player1Board, BoardImpl<T>& player2Board, std::vector<unique_ptr<FightInfo>>& vectorToFill);

	// Return true if positions are valid, and that the move is only
	// to adjacent position vertically or horizontally.
	bool IsLegalMoveDestination(const Point& posFrom, const Point& posTo) const;

	// Returns the piece of the given player in the given location.
	// If there is no piece of the given player in the given location, returns nullptr.
	T* GetPieceOfPlayer(const Point& position, int playerNum);

	//bool IsJokerChangeLegal(const JokerChange& jokerChange);

	// Checks if the move is legal. If so, move the piece to the new position.
	// Maybe requires fight.
	bool MovePiece(const Player& player, const Point& posFrom, const Point& posTo, FightInfoImpl& toFill);

	// Checks if the move is legal. If so, move the piece to the new position.
	// Maybe requires fight.
	bool MovePiece(const Player& player, const std::unique_ptr<Move>& move, FightInfoImpl& toFill);

	void MovePieceWithoutChecks(const Point& source, const Point& target)
	{
		GetBoardInPosition(target).StealPieceFromSquare(GetBoardInPosition(source));
	}

	bool IsEmptyInPosition(const Point& position) const
	{
		return IsEmptyInPosition(position.getX(), position.getY());
	}

	bool IsEmptyInPosition(int x, int y) const
	{
		return GetBoardInPosition(x, y).IsEmpty();
	}

	// Clear the board in the given position from any piece.
	void ClearBoardInPosition(const Point& position)
	{ 
		GetBoardInPosition(position).ClearSquare(); 
	}

	// It is the caller responsibility to call CheckIfValidPosition before this function 
	void PutPieceInPosition(const Point& position, unique_ptr<T> piece) 
	{ 
		GetBoardInPosition(position).ChangeSquarePiece(std::move(piece));
	}

	T& PeekPieceInPosition(const Point& position) 
	{
		return PeekPieceInPosition(position.getX(), position.getY());
	}

	const T& PeekPieceInPosition(const Point& position) const
	{
		return PeekPieceInPosition(position.getX(), position.getY());
	}

	T& PeekPieceInPosition(int x, int y)
	{
		return GetBoardInPosition(x, y).PeekPiece();
	}

	const T& PeekPieceInPosition(int x, int y) const
	{
		return GetBoardInPosition(x, y).PeekPiece();
	}

	// Checks if the position isn't out of range.
	static bool CheckIfValidPosition(const Point& position);

	// Prints the board to the console.
	void Print(std::ostream& outFile) const;

	// Gets board's rows count
	int GetRowsNum() const
	{ return this->mRows; }

	// Gets board's columns count
	int GetColsNum() const
	{ return this->mColumns; }

	virtual int getPlayer(const Point& pos) const; // 1 for player 1�s piece, 2 for 2, 0 if empty
};

#endif //ADTO_TARGIL1_BOARD_H
