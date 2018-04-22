#include "stdafx.h"
#include "ParserMoveFile.h"
#include "Game.h"
#include "Joker.h"
#include "Piece.h"
#include <vector>
#include <fstream>
#include <iostream>

using std::string;
using std::vector;
using std::cout;
using std::endl;

ParserMoveFile::~ParserMoveFile()
{
}

bool ParserMoveFile::processLineJokerTokens(Player & player, const std::vector<std::string>& tokens, int lineNum)
{
	int playerNum = player.GetPlayerNum();

	if (tokens[J_TOKEN_NUM] != "J:")
	{
		cout << "The token for joker should be J:" << endl;
		mGame->SetBadInputFileMessageWithWinner(playerNum, mGame->GetWinner(playerNum), lineNum, BAD_MOVE_PLAYER);
		return false;
	}

	if (tokens[NEW_REP_TOKEN_NUM].length() != 1)
	{
		cout << "<NEW_REP> has to be a character" << endl;
		mGame->SetBadInputFileMessageWithWinner(playerNum, mGame->GetWinner(playerNum), lineNum, BAD_MOVE_PLAYER);
		return false;
	}

	Board::BoardPosition posJoker;
	if (!GetPositionFromChars(tokens[JOKER_Y_TOKEN_NUM], tokens[JOKER_X_TOKEN_NUM], posJoker, playerNum, lineNum))
	{
		// Already printed error.
		mGame->SetBadInputFileMessageWithWinner(playerNum, mGame->GetWinner(playerNum), lineNum, BAD_MOVE_PLAYER);
		return false;
	}

	Piece* jokerPiece = mGame->mGameBoard.GetBoardInPosition(posJoker).GetPiece();
	Joker* joker = dynamic_cast<Joker*>(jokerPiece);

	if (joker == nullptr)
	{
		cout << "Joker position doesn't have a joker" << endl;
		mGame->SetBadInputFileMessageWithWinner(playerNum, mGame->GetWinner(playerNum), lineNum, BAD_MOVE_PLAYER);
		return false;
	}

	// Joker position that doesn't have a Joker owned by this player
	else if (joker->GetOwner() != &player)
	{
		cout << "Joker position doesn't have a joker owned by this player" << endl;
		mGame->SetBadInputFileMessageWithWinner(playerNum, mGame->GetWinner(playerNum), lineNum, BAD_MOVE_PLAYER);
		return false;
	}
	else if (!ChangeJokerActualType(joker, tokens[NEW_REP_TOKEN_NUM][0]))
	{
		// Already printed error.
		mGame->SetBadInputFileMessageWithWinner(playerNum, mGame->GetWinner(playerNum), lineNum, BAD_MOVE_PLAYER);
		return false;
	}

	return true;
}

bool ParserMoveFile::processLineTokens(Player& player, const std::vector<std::string>& tokens, int lineNum)
{
	int playerNum = player.GetPlayerNum();

	if ((tokens.size() != MOVE_LINE_TOKENS_COUNT_WITHOUT_JOKER) && (tokens.size() != MOVE_LINE_TOKENS_COUNT_WITH_JOKER))
	{
		cout << "number of tokens has to be " << MOVE_LINE_TOKENS_COUNT_WITHOUT_JOKER
			<< " or " << MOVE_LINE_TOKENS_COUNT_WITH_JOKER << endl;
		mGame->SetBadInputFileMessageWithWinner(playerNum, mGame->GetWinner(playerNum), lineNum, BAD_MOVE_PLAYER);
		return false;
	}

	Board::BoardPosition posFrom;
	Board::BoardPosition posTo;
	if ((!GetPositionFromChars(tokens[FROM_Y_TOKEN_NUM], tokens[FROM_X_TOKEN_NUM], posFrom, playerNum, lineNum)) ||
		(!GetPositionFromChars(tokens[TO_Y_TOKEN_NUM], tokens[TO_X_TOKEN_NUM], posTo, playerNum, lineNum)))
	{
		// Already printed error.
		mGame->SetBadInputFileMessageWithWinner(playerNum, mGame->GetWinner(playerNum), lineNum, BAD_MOVE_PLAYER);
		return false;
	}

	if (!mGame->mGameBoard.MovePiece(posFrom, posTo))
	{
		// Already printed error.
		mGame->SetBadInputFileMessageWithWinner(playerNum, mGame->GetWinner(playerNum), lineNum, BAD_MOVE_PLAYER);
		return false;
	}

	if (mGame->CheckGameOverAfterMove() != Game::Winner::None)
	{
		// The input is ok, no need to print anything.
		return false;
	}

	// If it's a joker line
	if (tokens.size() == MOVE_LINE_TOKENS_COUNT_WITH_JOKER)
	{
		return processLineJokerTokens(player, tokens, lineNum);
	}

	return true;
}

bool ParserMoveFile::HandlePlayerMove(Player& player, std::ifstream& playerMoveFileStream, int lineNum)
{
	std::string line;
	std::getline(playerMoveFileStream, line);

	if (!processLine(player, line, lineNum, BAD_MOVE_PLAYER))
	{
		// Already printed error if any.
		return false;
	}

	return true;
}

void ParserMoveFile::ParseRemainingPlayerMoves(Player & player, std::ifstream & playerMoveFileStream, int lineNum)
{
	bool ifContinuePlay = true;

	while ((ifContinuePlay) && (!playerMoveFileStream.eof()))
	{
		ifContinuePlay = HandlePlayerMove(player, playerMoveFileStream, lineNum);

		lineNum++;
	}
}

void ParserMoveFile::ParsePlayersMoveFiles(Player players[], int numOfPlayers, std::vector<std::string>& playerFileNames)
{
	std::vector<std::ifstream> inFilePlayer;
	//read file
	for (int i = 0; i < numOfPlayers; i++)
	{
		inFilePlayer.push_back(std::ifstream(playerFileNames[i]));

		if (!CheckOpenInputFile(inFilePlayer[i], playerFileNames[i]))
		{
			// Already printed error if any.
			mGame->SetBadInputFileMessageWithWinner(i+1, mGame->GetWinner(i+1), 0, BAD_MOVE_PLAYER);
			return;
		}
	}

	//otherwise, files exist
	bool ifContinuePlay = true;
	int lineNum = 1;

	// Moves files are not analyzed ahead, if there is a problem in a specific line the players
	// whose line currently failed loses
	while (ifContinuePlay)
	{
		// Checks if players[0] i.e player1, can move. If not, he loses the game.
		if (players[0].GetCountOfMovingPieces() == 0)
		{
			mGame->mGameOverMessage = PIECES_EATEN;
			mGame->mWinner = Game::Winner::Player2;
			return;
		}

		// We can assume that there are 2 players
		// Execute player1 move
		ifContinuePlay = HandlePlayerMove(players[0], inFilePlayer[0], lineNum);

		if (ifContinuePlay)
		{
			// Checks if players[1] i.e player2, can move. If not, he loses the game.
			if (players[1].GetCountOfMovingPieces() == 0)
			{
				mGame->mGameOverMessage = PIECES_EATEN;
				mGame->mWinner = Game::Winner::Player1;
				return;
			}

			// Execute player2 move
			ifContinuePlay = HandlePlayerMove(players[1], inFilePlayer[1], lineNum);
		}

		if (ifContinuePlay)
		{
			if (inFilePlayer[0].eof())
			{
				ParseRemainingPlayerMoves(players[1], inFilePlayer[1], lineNum);
				ifContinuePlay = false;
			}
			else if (inFilePlayer[1].eof())
			{
				ParseRemainingPlayerMoves(players[0], inFilePlayer[0], lineNum);
				ifContinuePlay = false;
			}
		}

		lineNum++;
	}

	for (int i = 0; i < numOfPlayers; i++)
	{
		if (!CheckReadOK(players[i], inFilePlayer[i], playerFileNames[i], lineNum))
		{
			// Already printed error if any.
			mGame->SetBadInputFileMessageWithWinner(i + 1, mGame->GetWinner(i + 1), lineNum, BAD_MOVE_PLAYER);
			return;
		}

		inFilePlayer[i].close();
	}
}