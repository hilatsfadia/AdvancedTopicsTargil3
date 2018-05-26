#include "stdafx.h"
#include "ParserInitFile.h"
#include "Game.h"
#include "Joker.h"
#include <vector>
#include <fstream>
#include <iostream>
#include "PiecePositionImpl.h"

#define INIT_LINE_TOKENS_COUNT_WITH_JOKER 4
#define INIT_LINE_TOKENS_COUNT_WITHOUT_JOKER 3
//#define PIECE_CHAR_TOKEN_WITHOUT_JOKER_NUM 0
#define X_TOKEN_NUM 1
#define Y_TOKEN_NUM 2
#define J_TOKEN_NUM 0
#define PIECE_CHAR_TOKEN_WITH_JOKER_NUM 3

using std::string;
using std::vector;
using std::cout;
using std::endl;

bool ParserInitFile::processJokerLine(const std::vector<std::string>& tokens, PointImpl& pos, 
	std::vector<std::unique_ptr<PiecePosition>>& vectorToFill) const
{
	//Piece* piece = nullptr;

	if (tokens[J_TOKEN_NUM][0] != JOKER_CHAR)
	{
		cout << "Init file: " << "First char of joker must be 'J'. " << endl;
		return false;
	}

	if (tokens[PIECE_CHAR_TOKEN_WITH_JOKER_NUM].length() != 1)
	{
		cout << "Init file: " << "<PIECE_CHAR> of joker given in positions file is not a character. " << endl;
		return false;
	}

	vectorToFill.push_back(std::make_unique<PiecePositionImpl>(pos, JOKER_CHAR, tokens[PIECE_CHAR_TOKEN_WITH_JOKER_NUM][0]));

	return true;
}

bool ParserInitFile::ProcessLineTokens(const std::vector<std::string>& tokens, 
	std::vector<std::unique_ptr<PiecePosition>>& vectorToFill) const
{
	if ((tokens.size() != INIT_LINE_TOKENS_COUNT_WITHOUT_JOKER) && (tokens.size() != INIT_LINE_TOKENS_COUNT_WITH_JOKER))
	{
		cout << "Init file: " << "number of tokens has to be " << INIT_LINE_TOKENS_COUNT_WITHOUT_JOKER
			<< " or " << INIT_LINE_TOKENS_COUNT_WITH_JOKER << endl;
		return false;
	}

	unique_ptr<PointImpl> pos = GetPositionFromChars(tokens[X_TOKEN_NUM], tokens[Y_TOKEN_NUM]);
	if (!pos)
	{
		// Already printed error.
		return false;
	}

	if (tokens[0].length() != 1)
	{
		cout << "Init file: " << "First token given in positions file is not a character. " <<
			"It should be <PIECE_CHAR> or J" << endl;
		return false;
	}

	if (tokens.size() == INIT_LINE_TOKENS_COUNT_WITHOUT_JOKER)
	{
		//return processNonJokerLine(player, tokens, pos, vectorToFill);
		vectorToFill.push_back(std::make_unique<PiecePositionImpl>(*pos, tokens[0][0]));
	}
	else
	{
		// It's a joker line
		// (tokens.size() == INIT_LINE_TOKENS_COUNT_WITH_JOKER)
		return processJokerLine(tokens, *pos, vectorToFill);
	}

	return true;
}

bool ParserInitFile::ProcessLine(const std::string& line, 
	std::vector<std::unique_ptr<PiecePosition>>& vectorToFill) const
{
	try
	{
		vector<string> tokens;
		GetTokensFromLine(line, tokens);

		// Skip empty lines
		if (tokens.size() != 0)
		{
			return ProcessLineTokens(tokens, vectorToFill);// Maybe Bad Format
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

void ParserInitFile::PutErrorPositionInVector(std::vector<std::unique_ptr<PiecePosition>>& vectorToFill) const
{
	vectorToFill.clear();
	vectorToFill.push_back(INVALID_PIECE_POSITION);
}

// TODO: maybe pass the stream
void ParserInitFile::ParsePlayerInitFile(int player, const std::string & playerInputfileName, 
	std::vector<std::unique_ptr<PiecePosition>>& vectorToFill) const
{
	//read file
	std::ifstream inFile(playerInputfileName);

	if (!CheckOpenInputFile(inFile, playerInputfileName))
	{
		PrintUsageMessage(player);
		PutErrorPositionInVector(vectorToFill);
		return;
	}
	//otherwise, file exists

	std::string line;
	while (std::getline(inFile, line))
	{
		if (!ProcessLine(line, vectorToFill))
		{
			PutErrorPositionInVector(vectorToFill);
			inFile.close();
			return;
		}
	}

	// Already printed error if any.
	if (!CheckReadOK(inFile, playerInputfileName))
	{
		PutErrorPositionInVector(vectorToFill);
	}

	inFile.close();
}
