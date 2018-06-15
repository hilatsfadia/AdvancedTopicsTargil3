#ifndef ADTO_TARGIL1_PIECE_FACTORY_H
#define ADTO_TARGIL1_PIECE_FACTORY_H

//--------------------------
// PieceFactory Class
//--------------------------

// A class that creates new instances of type piece.
// @author Hila Tsfadia, Jael Fafner TAU - Advanced Topics in Programming - 2018 Semester B

#include <memory>

namespace HilaAndJaelExtendedRPS
{
	class Piece;
	class Player;

	class PieceFactory
	{
	private:
		PieceFactory() {}
	public:
		enum class PieceType { Rock, Paper, Scissors, Bomb, Joker, Flag, Covered };

		// Creates a new piece object according to it's given char representation. (not joker)
		// Returns the created piece, or nullptr if pieceChar can't represent a piece.
		// TODO: maybe remove the owner, kepp only it's number
		static std::unique_ptr<Piece> GetPieceFromChar(char pieceChar, std::shared_ptr<Player> owner = nullptr);
		static std::unique_ptr<Piece> GetPieceFromChar(char pieceChar, int ownerNum);
	};
}
#endif //ADTO_TARGIL1_PIECE_FACTORY_H