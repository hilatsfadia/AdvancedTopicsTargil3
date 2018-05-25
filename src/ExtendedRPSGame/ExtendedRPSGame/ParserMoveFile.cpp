#include "stdafx.h"
#include "ParserMoveFile.h"
#include "Game.h"
#include "Joker.h"
#include "Piece.h"
#include <vector>
#include <fstream>
#include <iostream>
#include "MoveImpl.h"
#include "JokerChangeImpl.h"

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
using std::make_unique;

unique_ptr<JokerChange> ParserMoveFile::ParseCurrJokerChange()
{
	// If no change is requested
	if (currMoveLineTokens.size() != MOVE_LINE_TOKENS_COUNT_WITH_JOKER)
	{
		return nullptr;
	}

	if (currMoveLineTokens[J_TOKEN_NUM] != "J:")
	{
		cout << "Moves file: " << "The token for joker should be J:" << endl;
		// TODO: mGame->SetBadInputFileMessageWithWinner(playerNum, mGame->GetWinner(playerNum), lineNum, BAD_MOVE_PLAYER);
		return INVALID_JOKER_CHANGE;
	}

	if (currMoveLineTokens[NEW_REP_TOKEN_NUM].length() != 1)
	{
		cout << "Moves file: " << "<NEW_REP> has to be a character" << endl;
		// TODO: mGame->SetBadInputFileMessageWithWinner(playerNum, mGame->GetWinner(playerNum), lineNum, BAD_MOVE_PLAYER);
		return INVALID_JOKER_CHANGE;
	}

	unique_ptr<PointImpl> posJoker = GetPositionFromChars(currMoveLineTokens[JOKER_X_TOKEN_NUM], currMoveLineTokens[JOKER_Y_TOKEN_NUM]);
	if (!posJoker)
	{
		// Already printed error.
		// TODO: mGame->SetBadInputFileMessageWithWinner(playerNum, mGame->GetWinner(playerNum), lineNum, BAD_MOVE_PLAYER);
		return INVALID_JOKER_CHANGE;
	}

	// TODO: maybe JokerChangeImpl has to have pointer by value
	return make_unique<JokerChangeImpl>(*posJoker, currMoveLineTokens[NEW_REP_TOKEN_NUM][0]);
}

unique_ptr<Move> ParserMoveFile::ProcessMoveLineTokens()
{
	if ((currMoveLineTokens.size() != MOVE_LINE_TOKENS_COUNT_WITHOUT_JOKER) && (currMoveLineTokens.size() != MOVE_LINE_TOKENS_COUNT_WITH_JOKER))
	{
		cout << "Moves file: " << "number of tokens in each line has to be " << MOVE_LINE_TOKENS_COUNT_WITHOUT_JOKER
			<< " or " << MOVE_LINE_TOKENS_COUNT_WITH_JOKER << endl;
		return nullptr;
	}

	unique_ptr<PointImpl> posFrom = GetPositionFromChars(currMoveLineTokens[FROM_X_TOKEN_NUM], currMoveLineTokens[FROM_Y_TOKEN_NUM]);
	unique_ptr<PointImpl> posTo = GetPositionFromChars(currMoveLineTokens[TO_X_TOKEN_NUM], currMoveLineTokens[TO_Y_TOKEN_NUM]);

	if (!posFrom || !posTo)
	{
		// Already printed error.
		return nullptr;
	}

	return make_unique<MoveImpl>(*posFrom, *posTo);
}

unique_ptr<Move> ParserMoveFile::ParseCurrMove(std::ifstream& playerMoveFileStream)
{
	std::string line;
	std::getline(playerMoveFileStream, line);

	try
	{
		currMoveLineTokens.clear();
		GetTokensFromLine(line, currMoveLineTokens);

		// Skip empty lines
		if (currMoveLineTokens.size() != 0)
		{
			return ProcessMoveLineTokens();
		}
	}
	// Souldn't get here!
	catch (...)
	{
		// Print errors if any?
		return nullptr; // Bad Format
	}

	return nullptr;
}

//void ParserMoveFile::ParsePlayersMoveFiles(Player players[], int numOfPlayers, std::vector<std::string>& playerFileNames)
//{
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
