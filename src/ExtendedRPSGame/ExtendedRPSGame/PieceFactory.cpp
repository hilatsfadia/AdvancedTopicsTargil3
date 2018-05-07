#include "stdafx.h"
#include "PieceFactory.h"
#include "Rock.h"
#include "Paper.h"
#include "Scissors.h"
#include "Bomb.h"
#include "Joker.h"
#include "Flag.h"
#include "Player.h"

Piece* PieceFactory::GetPieceFromChar(char pieceChar, Player* owner)
{
	Piece* toReturn = nullptr;

	switch (pieceChar)
	{
		case ROCK_CHAR: {
			toReturn = new Rock(owner);
			break;
		}
		case PAPER_CHAR: {
			toReturn = new Paper(owner);
			break;
		}
		case SCISSORS_CHAR: {
			toReturn = new Scissors(owner);
			break;
		}
		case BOMB_CHAR: {
			toReturn = new Bomb(owner);
			break;
		}
		//case JOKER_UPPER_CASE_CHAR: {
		//	toReturn = new Joker(pieceChar);
		//	break;
		//}
		case FLAG_CHAR: {
			toReturn = new Flag(owner);
			break;
		}
		default:
		{
			toReturn = nullptr;
			break;
		}
	}

	return toReturn;
}

