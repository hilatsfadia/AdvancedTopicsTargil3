#include "stdafx.h"
#include "RPSPiece.h"

Piece::WinningPiece RPSPiece::FightWithOtherPieceType(Piece & enemy)
{
	PieceFactory::PieceType enemyPieceType = enemy.GetPieceType();

	switch (enemyPieceType)
	{
		case (PieceFactory::PieceType::Bomb):
		{
			BothPiecesLosers(enemy);
			return WinningPiece::Tie;
			break;
		}
		case (PieceFactory::PieceType::Joker):
		{
			if (enemy.Fight(*this) == WinningPiece::enemy)
			{
				return WinningPiece::ThisPiece;
			}
			else if (enemy.Fight(*this) == WinningPiece::ThisPiece)
			{
				return WinningPiece::enemy;
			}
			else
			{
				return WinningPiece::Tie;
			}
			break;
		}
		case (PieceFactory::PieceType::Flag):
		{
			enemy.LoseToPiece();
			return WinningPiece::ThisPiece;
			break;
		}
		default:
		{
			return WinningPiece::Tie;
		}
	}
}
