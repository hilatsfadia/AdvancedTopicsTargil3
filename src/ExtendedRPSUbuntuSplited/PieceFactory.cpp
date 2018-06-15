#include "PieceFactory.h"
#include "Rock.h"
#include "Paper.h"
#include "Scissors.h"
#include "Bomb.h"
#include "Joker.h"
#include "Flag.h"
#include "Player.h"

using namespace HilaAndJaelExtendedRPS;

using std::shared_ptr;
using std::make_unique;

unique_ptr<Piece> PieceFactory::GetPieceFromChar(char pieceChar, shared_ptr<Player> owner)
{
	switch (pieceChar)
	{
		case ROCK_CHAR: {
			return make_unique<Rock>(owner);
			break;
		}
		case PAPER_CHAR: {
			return make_unique<Paper>(owner);
			break;
		}
		case SCISSORS_CHAR: {
			return make_unique<Scissors>(owner);
			break;
		}
		case BOMB_CHAR: {
			return make_unique<Bomb>(owner);
			break;
		}
		//case JOKER_UPPER_CASE_CHAR: {
		//	return make_unique<Joker(pieceChar);
		//	break;
		//}
		case FLAG_CHAR: {
			return make_unique<Flag>(owner);
			break;
		}
		default:
		{
			return nullptr;
			break;
		}
	}

	return nullptr;
}

unique_ptr<Piece> PieceFactory::GetPieceFromChar(char pieceChar, int ownerNum)
{
	switch (pieceChar)
	{
		case ROCK_CHAR: {
			return make_unique<Rock>(ownerNum);
			break;
		}
		case PAPER_CHAR: {
			return make_unique<Paper>(ownerNum);
			break;
		}
		case SCISSORS_CHAR: {
			return make_unique<Scissors>(ownerNum);
			break;
		}
		case BOMB_CHAR: {
			return make_unique<Bomb>(ownerNum);
			break;
		}
		//case JOKER_UPPER_CASE_CHAR: {
		//	return make_unique<Joker(pieceChar);
		//	break;
		//}
		case FLAG_CHAR: {
			return make_unique<Flag>(ownerNum);
			break;
		}
		default:
		{
			return nullptr;
			break;
		}
	}

	return nullptr;
}

