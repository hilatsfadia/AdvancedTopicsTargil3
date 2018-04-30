#include "stdafx.h"
#include "ParserInitFile.h"
#include "Game.h"
#include "Joker.h"
#include <vector>
#include <fstream>
#include <iostream>
#include "PiecePositionImpl.h"

using std::string;
using std::vector;
using std::cout;
using std::endl;

ParserInitFile::~ParserInitFile()
{
}

bool ParserInitFile::processJokerLine(int player, const std::vector<std::string>& tokens, PointImpl& pos, std::vector<std::unique_ptr<PiecePosition>>& vectorToFill)
{
	//Piece* piece = nullptr;

	if (tokens[J_TOKEN_NUM][0] != JOKER_CHAR_PLAYER_1)
	{
		cout << "First char of joker must be 'J'. " << endl;
		return false;
	}

	if (tokens[PIECE_CHAR_TOKEN_WITH_JOKER_NUM].length() != 1)
	{
		cout << "<PIECE_CHAR> of joker given in positions file is not a character. " << endl;
		return false;
	}

	if (player == 1)
	{
		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(PiecePositionImpl(&pos, JOKER_CHAR_PLAYER_1, tokens[PIECE_CHAR_TOKEN_WITH_JOKER_NUM][0])));
	}
	else
	{
		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(PiecePositionImpl(&pos, JOKER_CHAR_PLAYER_2, tokens[PIECE_CHAR_TOKEN_WITH_JOKER_NUM][0])));
	}
}

bool ParserInitFile::ProcessLineTokens(int playerNum, const std::vector<std::string>& tokens, int lineNum, std::vector<std::unique_ptr<PiecePosition>>& vectorToFill)
{
	if ((tokens.size() != INIT_LINE_TOKENS_COUNT_WITHOUT_JOKER) && (tokens.size() != INIT_LINE_TOKENS_COUNT_WITH_JOKER))
	{
		cout << "number of tokens has to be " << INIT_LINE_TOKENS_COUNT_WITHOUT_JOKER 
			<< " or " << INIT_LINE_TOKENS_COUNT_WITH_JOKER << endl;
		return false;
	}

	PointImpl pos;
	if (!GetPositionFromChars(tokens[X_TOKEN_NUM], tokens[Y_TOKEN_NUM], pos, playerNum, lineNum))
	{
		// Already printed error.
		return false;
	}

	if (tokens[0].length() != 1)
	{
		cout << "First token given in positions file is not a character. " <<
			"It should be <PIECE_CHAR> or J" << endl;
		return false;
	}

	if (tokens.size() == INIT_LINE_TOKENS_COUNT_WITHOUT_JOKER)
	{
		//return processNonJokerLine(player, tokens, pos, vectorToFill);
		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(PiecePositionImpl(&pos, tokens[0][0])));
	}
	else
	{
		// It's a joker line
		// (tokens.size() == INIT_LINE_TOKENS_COUNT_WITH_JOKER)
		return processJokerLine(playerNum, tokens, pos, vectorToFill);
	}

	return true;
}

bool ParserInitFile::ProcessLine(int player, const std::string& line, int lineNum, const char* templateBadFormatMessage, std::vector<std::unique_ptr<PiecePosition>>& vectorToFill)
{
	try
	{
		vector<string> tokens;
		GetTokensFromLine(line, tokens);

		// Skip empty lines
		if (tokens.size() != 0)
		{
			return ProcessLineTokens(player, tokens, lineNum, vectorToFill);// Maybe Bad Format
																			// Already printed error if any.
		}
	}
	//catch (const std::exception&)
	// Souldn't get here!
	catch (...)
	{
		// Print errors if any?
		//int playerNum = player.GetPlayerNum();
		// TODO: mGame->SetBadInputFileMessageWithWinner(playerNum, mGame->GetWinner(playerNum), lineNum, templateBadFormatMessage);
		return false; // Bad Format
	}

	return true;
}

bool ParserInitFile::ParsePlayerInitFile(int player, const std::string & playerInputfileName, std::vector<std::unique_ptr<PiecePosition>>& vectorToFill)
{
	//read file
	std::ifstream inFile(playerInputfileName);
	bool isFileOK = true;

	if (!CheckOpenInputFile(inFile, playerInputfileName))
	{
		// Already printed error if any.
		// TODO: mGame->mProblematicLineOfPlayer[player.GetPlayerNum() - 1] = 0;
		return false;
	}

	//otherwise, file exists
	std::string line;
	int lineNum = 1;
	while ((std::getline(inFile, line)) && isFileOK)
	{
		if (!ProcessLine(player, line, lineNum, BAD_POS_PLAYER, vectorToFill))
		{
			// TODO: mGame->mProblematicLineOfPlayer[player.GetPlayerNum() - 1] = lineNum;

			// Already printed error if any.
			isFileOK = false;
		}

		lineNum++;
	}

	// Already printed error if any.
	isFileOK = CheckReadOK(player, inFile, playerInputfileName, lineNum);

	inFile.close();

	return isFileOK;
}
