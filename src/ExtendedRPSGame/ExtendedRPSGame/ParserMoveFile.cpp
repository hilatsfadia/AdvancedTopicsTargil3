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

bool ParserMoveFile::processLineJokerTokens(int playerNum, const std::vector<std::string>& tokens, int lineNum)
{
	if (tokens[J_TOKEN_NUM] != "J:")
	{
		cout << "The token for joker should be J:" << endl;
		// TODO: mGame->SetBadInputFileMessageWithWinner(playerNum, mGame->GetWinner(playerNum), lineNum, BAD_MOVE_PLAYER);
		return false;
	}

	if (tokens[NEW_REP_TOKEN_NUM].length() != 1)
	{
		cout << "<NEW_REP> has to be a character" << endl;
		// TODO: mGame->SetBadInputFileMessageWithWinner(playerNum, mGame->GetWinner(playerNum), lineNum, BAD_MOVE_PLAYER);
		return false;
	}

	Point* posJoker = GetPositionFromChars(tokens[JOKER_X_TOKEN_NUM], tokens[JOKER_Y_TOKEN_NUM], playerNum, lineNum);
	if (!posJoker)
	{
		// Already printed error.
		// TODO: mGame->SetBadInputFileMessageWithWinner(playerNum, mGame->GetWinner(playerNum), lineNum, BAD_MOVE_PLAYER);
		return false;
	}

	mCurrJokerChange = make_unique<JokerChangeImpl>(posJoker, tokens[NEW_REP_TOKEN_NUM][0]);
	mIsHavingJokerChange = true;

	return true;
}

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

	// If it's a joker line
	if (tokens.size() == MOVE_LINE_TOKENS_COUNT_WITH_JOKER)
	{
		if (!processLineJokerTokens(playerNum, tokens, lineNum))
		{
			// TODO:
		}
	}

	return make_unique<MoveImpl>(posFrom, posTo);
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

unique_ptr<JokerChange> ParserMoveFile::GetCurrJokerChange()
{
	if ((mIsHavingJokerChange) && (mCurrJokerChange != nullptr))
	{
		return std::move(mCurrJokerChange);
	}

	//TODO: look return unique_ptr<JokerChange>();

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
