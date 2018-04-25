#include "stdafx.h"
#include "PieceFactory.h"
#include "Rock.h"
#include "Paper.h"
#include "Scissors.h"
#include "Bomb.h"
#include "Joker.h"
#include "Flag.h"

Piece* PieceFactory::GetPieceFromChar(char pieceChar)
{
	char charUpper = toupper(pieceChar);
	Piece* toReturn = nullptr;

	switch (charUpper)
	{
		case ROCK_UPPER_CASE_CHAR: {
			toReturn = new Rock(pieceChar);
			break;
		}
		case PAPER_UPPER_CASE_CHAR: {
			toReturn = new Paper(pieceChar);
			break;
		}
		case SCISSORS_UPPER_CASE_CHAR: {
			toReturn = new Scissors(pieceChar);
			break;
		}
		case BOMB_UPPER_CASE_CHAR: {
			toReturn = new Bomb(pieceChar);
			break;
		}
		//case JOKER_UPPER_CASE_CHAR: {
		//	toReturn = new Joker(pieceChar);
		//	break;
		//}
		case FLAG_UPPER_CASE_CHAR: {
			toReturn = new Flag(pieceChar);
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

Piece * PieceFactory::GetJokerPieceFromChar(char jokerPieceChar)
{
	if (toupper(jokerPieceChar) != JOKER_UPPER_CASE_CHAR)
	{
		return nullptr;
	}

	return new Joker(jokerPieceChar);
}
