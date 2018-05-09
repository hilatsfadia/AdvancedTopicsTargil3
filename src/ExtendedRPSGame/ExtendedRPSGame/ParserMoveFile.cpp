#include "stdafx.h"
#include "ParserMoveFile.h"
#include "Game.h"
#include "Joker.h"
#include "Piece.h"
#include <vector>
#include <fstream>
#include <iostream>
#include "MoveImpl.h"

#define MOVE_LINE_TOKENS_COUNT_WITH_JOKER 8
#define MOVE_LINE_TOKENS_COUNT_WITHOUT_JOKER 4
#define FROM_X_TOKEN_NUM 0
#define FROM_Y_TOKEN_NUM 1
#define TO_X_TOKEN_NUM 2
#define TO_Y_TOKEN_NUM 3
#define J_TOKEN_NUM 4
#define JOKER_X_TOKEN_NUM 5
#define JOKER_Y_TOKEN_NUM 6
#define NEW_REP_TOKEN_NUM 7

using std::string;
using std::vector;
using std::cout;
using std::endl;

ParserMoveFile::~ParserMoveFile()
{
}

//
//bool ParserMoveFile::processLineJokerTokens(Player & player, const std::vector<std::string>& tokens, int lineNum)
//{
//	int playerNum = player.GetPlayerNum();
//
//	if (tokens[J_TOKEN_NUM] != "J:")
//	{
//		cout << "The token for joker should be J:" << endl;
//		// TODO: mGame->SetBadInputFileMessageWithWinner(playerNum, mGame->GetWinner(playerNum), lineNum, BAD_MOVE_PLAYER);
//		return false;
//	}
//
//	if (tokens[NEW_REP_TOKEN_NUM].length() != 1)
//	{
//		cout << "<NEW_REP> has to be a character" << endl;
//		// TODO: mGame->SetBadInputFileMessageWithWinner(playerNum, mGame->GetWinner(playerNum), lineNum, BAD_MOVE_PLAYER);
//		return false;
//	}
//
//	PointImpl posJoker;
//	if (!GetPositionFromChars(tokens[JOKER_X_TOKEN_NUM], tokens[JOKER_Y_TOKEN_NUM], posJoker, playerNum, lineNum))
//	{
//		// Already printed error.
//		// TODO: mGame->SetBadInputFileMessageWithWinner(playerNum, mGame->GetWinner(playerNum), lineNum, BAD_MOVE_PLAYER);
//		return false;
//	}
//
//	Piece* jokerPiece = mGame->mGameBoard.GetBoardInPosition(posJoker).GetPiece();
//	Joker* joker = dynamic_cast<Joker*>(jokerPiece);
//
//	if (joker == nullptr)
//	{
//		cout << "Joker position doesn't have a joker" << endl;
//		mGame->SetBadInputFileMessageWithWinner(playerNum, mGame->GetWinner(playerNum), lineNum, BAD_MOVE_PLAYER);
//		return false;
//	}
//
//	// Joker position that doesn't have a Joker owned by this player
//	else if (joker->GetOwner() != &player)
//	{
//		cout << "Joker position doesn't have a joker owned by this player" << endl;
//		mGame->SetBadInputFileMessageWithWinner(playerNum, mGame->GetWinner(playerNum), lineNum, BAD_MOVE_PLAYER);
//		return false;
//	}
//	else if (!ChangeJokerActualType(joker, tokens[NEW_REP_TOKEN_NUM][0]))
//	{
//		// Already printed error.
//		mGame->SetBadInputFileMessageWithWinner(playerNum, mGame->GetWinner(playerNum), lineNum, BAD_MOVE_PLAYER);
//		return false;
//	}
//
//	return true;
//}

unique_ptr<Move> ParserMoveFile::ProcessMoveLineTokens(int playerNum, const std::vector<std::string>& tokens, int lineNum)
{
	if ((tokens.size() != MOVE_LINE_TOKENS_COUNT_WITHOUT_JOKER) && (tokens.size() != MOVE_LINE_TOKENS_COUNT_WITH_JOKER))
	{
		cout << "number of tokens has to be " << MOVE_LINE_TOKENS_COUNT_WITHOUT_JOKER
			<< " or " << MOVE_LINE_TOKENS_COUNT_WITH_JOKER << endl;
		// TODO: mGame->SetBadInputFileMessageWithWinner(playerNum, mGame->GetWinner(playerNum), lineNum, BAD_MOVE_PLAYER);
		// TODO: return false;
	}

	Point* posFrom = GetPositionFromChars(tokens[FROM_X_TOKEN_NUM], tokens[FROM_Y_TOKEN_NUM], playerNum, lineNum);
	Point* posTo = GetPositionFromChars(tokens[TO_X_TOKEN_NUM], tokens[TO_Y_TOKEN_NUM], playerNum, lineNum);

	if (!posFrom || !posTo)
	{
		// Already printed error.
		// TODO: mGame->SetBadInputFileMessageWithWinner(playerNum, mGame->GetWinner(playerNum), lineNum, BAD_MOVE_PLAYER);
		// TODO: return false;
	}

	//// If it's a joker line
	//if (tokens.size() == MOVE_LINE_TOKENS_COUNT_WITH_JOKER)
	//{
	//	return processLineJokerTokens(player, tokens, lineNum);
	//}

	return std::make_unique<MoveImpl>(posFrom, posTo);
}

unique_ptr<Move> ParserMoveFile::ParsePlayerMove(int playerNum, std::ifstream& playerMoveFileStream, int lineNum)
{
	std::string line;
	std::getline(playerMoveFileStream, line);

	try
	{
		vector<string> tokens;
		GetTokensFromLine(line, tokens);

		// Skip empty lines
		if (tokens.size() != 0)
		{
			return ProcessMoveLineTokens(playerNum, tokens, lineNum);
		}
	}
	//catch (const std::exception&)
	// Souldn't get here!
	catch (...)
	{
		// Print errors if any?
		// TODO: mGame->SetBadInputFileMessageWithWinner(playerNum, mGame->GetWinner(playerNum), lineNum, BAD_MOVE_PLAYER);
		// TODO: return false; // Bad Format
	}

	return nullptr;
}

//
//void ParserMoveFile::ParseRemainingPlayerMoves(Player & player, std::ifstream & playerMoveFileStream, int lineNum)
//{
//	bool ifContinuePlay = true;
//
//	while ((ifContinuePlay) && (!playerMoveFileStream.eof()))
//	{
//		ifContinuePlay = HandlePlayerMove(player, playerMoveFileStream, lineNum);
//
//		lineNum++;
//	}
//}
//
//void ParserMoveFile::ParsePlayersMoveFiles(Player players[], int numOfPlayers, std::vector<std::string>& playerFileNames)
//{
//	//otherwise, files exist
//	bool ifContinuePlay = true;
//	int lineNum = 1;
//
//	// Moves files are not analyzed ahead, if there is a problem in a specific line the players
//	// whose line currently failed loses
//	while (ifContinuePlay)
//	{
//		// Checks if players[0] i.e player1, can move. If not, he loses the game.
//		if (players[0].GetCountOfMovingPieces() == 0)
//		{
//			// TODO: mGame->mGameOverMessage = PIECES_EATEN;
//			// TODO: mGame->mWinner = Game::Winner::Player2;
//			return;
//		}
//
//		// We can assume that there are 2 players
//		// Execute player1 move
//		ifContinuePlay = ParsePlayerMove(players[0], inFilePlayer[0], lineNum);
//
//		if (ifContinuePlay)
//		{
//			// Checks if players[1] i.e player2, can move. If not, he loses the game.
//			if (players[1].GetCountOfMovingPieces() == 0)
//			{
//				// TODO: mGame->mGameOverMessage = PIECES_EATEN;
//				// TODO: mGame->mWinner = Game::Winner::Player1;
//				return;
//			}
//
//			// Execute player2 move
//			ifContinuePlay = ParsePlayerMove(players[1], inFilePlayer[1], lineNum);
//		}
//
//		if (ifContinuePlay)
//		{
//			if (inFilePlayer[0].eof())
//			{
//				ParseRemainingPlayerMoves(players[1], inFilePlayer[1], lineNum);
//				ifContinuePlay = false;
//			}
//			else if (inFilePlayer[1].eof())
//			{
//				ParseRemainingPlayerMoves(players[0], inFilePlayer[0], lineNum);
//				ifContinuePlay = false;
//			}
//		}
//
//		lineNum++;
//	}
//
//	for (int i = 0; i < numOfPlayers; i++)
//	{
//		if (!CheckReadOK(players[i].GetPlayerNum(), inFilePlayer[i], playerFileNames[i], lineNum))
//		{
//			// Already printed error if any.
//			// TODO: mGame->SetBadInputFileMessageWithWinner(i + 1, mGame->GetWinner(i + 1), lineNum, BAD_MOVE_PLAYER);
//			return;
//		}
//
//		inFilePlayer[i].close();
//	}
//}
