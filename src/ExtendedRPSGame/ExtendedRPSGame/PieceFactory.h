#ifndef ADTO_TARGIL1_PIECE_FACTORY_H
#define ADTO_TARGIL1_PIECE_FACTORY_H

#include <memory>

class Piece;
class Player;

class PieceFactory
{
private:
	PieceFactory() {}
	~PieceFactory() {}
public:
	enum class PieceType { Rock, Paper, Scissors, Bomb, Joker, Flag, Unknown };

	// Creates a new piece object according to it's given char representation.
	// Returns the created piece, or nullptr if pieceChar can't represent a piece.
	// TODO: maybe remove the owner, kepp only it's number
	static Piece* GetPieceFromChar(char pieceChar, std::shared_ptr<Player> owner = nullptr);

	static Piece* GetPieceFromChar(char pieceChar, int ownerNum);
};

#endif //ADTO_TARGIL1_PIECE_FACTORY_H