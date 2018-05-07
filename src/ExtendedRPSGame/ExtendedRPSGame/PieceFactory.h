#ifndef ADTO_TARGIL1_PIECE_FACTORY_H
#define ADTO_TARGIL1_PIECE_FACTORY_H

class Piece;
class Player;

class PieceFactory
{
private:
	PieceFactory() {}
	~PieceFactory() {}
public:
	enum class PieceType { Rock, Paper, Scissors, Bomb, Joker, Flag };

	// Creates a new piece object according to it's given char representation.
	// Returns the created piece, or nullptr if pieceChar can't represent a piece.
	static Piece* GetPieceFromChar(char pieceChar, Player* owner = nullptr);
};

#endif //ADTO_TARGIL1_PIECE_FACTORY_H