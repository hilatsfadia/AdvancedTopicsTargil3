#ifndef ADTO_TARGIL1_PIECE_FACTORY_H
#define ADTO_TARGIL1_PIECE_FACTORY_H

class Piece;

#define ROCK_UPPER_CASE_CHAR 'R'
#define PAPER_UPPER_CASE_CHAR 'P'
#define SCISSORS_UPPER_CASE_CHAR 'S'
#define BOMB_UPPER_CASE_CHAR 'B'
#define JOKER_UPPER_CASE_CHAR 'J'
#define FLAG_UPPER_CASE_CHAR 'F'

class PieceFactory
{
private:
	PieceFactory() {}
	~PieceFactory() {}
public:
	enum class PieceType { Rock, Paper, Scissors, Bomb, Joker, Flag };

	// Creates a new piece object according to it's given char representation.
	// Returns the created piece, or nullptr if pieceChar can't represent a piece.
	static Piece* GetPieceFromChar(char pieceChar);

	// Creates a new joker object.
	// Returns the created piece, or nullptr if jokerPieceChar can't represent a joker.
	static Piece* GetJokerPieceFromChar(char jokerPieceChar);
};

#endif //ADTO_TARGIL1_PIECE_FACTORY_H