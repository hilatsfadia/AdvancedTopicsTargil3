#include "stdafx.h"
#include "ParserInitFile.h"
#include "Game.h"
#include "Joker.h"
#include <vector>
#include <fstream>
#include <iostream>

using std::string;
using std::vector;
using std::cout;
using std::endl;

ParserInitFile::~ParserInitFile()
{
}

bool ParserInitFile::processJokerLine(Player & player, const std::vector<std::string>& tokens, Board::BoardPosition& pos)
{
	Piece* piece = nullptr;

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

	// actualPiece shouldn't have an owner! because we don't want to 
	// count it as one of the player's pieces.
	if (player.GetPlayerNum() == 1)
	{
		piece = PieceFactory::GetJokerPieceFromChar(JOKER_CHAR_PLAYER_1);
	}
	else
	{
		piece = PieceFactory::GetJokerPieceFromChar(JOKER_CHAR_PLAYER_2);
	}

	if (piece == nullptr)
	{
		// Shouldn't happen
		return false;
	}

	Joker* joker = dynamic_cast<Joker*>(piece);
	//if (!joker)
	if (joker == nullptr)
	{
		// Shouldn't happen
		return false;
	}

	if (!InitJokerOwnerAndActualType(joker, tokens[PIECE_CHAR_TOKEN_WITH_JOKER_NUM][0], &player))
	{
		// Already printed error.
		return false;
	}

	// checks if X coordinate and/or Y coordinate of one or more PIECE is not in range
	// Already printed error if any.
	return mGame->mGameBoard.PutPieceOnBoard(piece, pos);
}

bool ParserInitFile::processNonJokerLine(Player & player, const std::vector<std::string>& tokens, Board::BoardPosition & pos)
{
	Piece* piece = PieceFactory::GetPieceFromChar(tokens[0][0]);
	if (piece == nullptr)
	{
		cout << "PIECE_CHAR in positions file should be one of: R P S B F" << endl;
		return false;
	}

	if (!piece->InitializeOwner(&player))
	{
		cout << "A PIECE type appears in file more than its number" << endl;
		return false;
	}

	// checks if X coordinate and/or Y coordinate of one or more PIECE is not in range
	// Already printed error if any.
	return mGame->mGameBoard.PutPieceOnBoard(piece, pos);
}

bool ParserInitFile::processLineTokens(Player& player, const std::vector<std::string>& tokens, int lineNum)
{
	int playerNum = player.GetPlayerNum();

	if ((tokens.size() != INIT_LINE_TOKENS_COUNT_WITHOUT_JOKER) && (tokens.size() != INIT_LINE_TOKENS_COUNT_WITH_JOKER))
	{
		cout << "number of tokens has to be " << INIT_LINE_TOKENS_COUNT_WITHOUT_JOKER 
			<< " or " << INIT_LINE_TOKENS_COUNT_WITH_JOKER << endl;
		return false;
	}

	Board::BoardPosition pos;
	if (!GetPositionFromChars(tokens[Y_TOKEN_NUM], tokens[X_TOKEN_NUM], pos, playerNum, lineNum))
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
		return processNonJokerLine(player, tokens, pos);
	}

	// It's a joker line
	// (tokens.size() == INIT_LINE_TOKENS_COUNT_WITH_JOKER)
	return processJokerLine(player, tokens, pos);
}

bool ParserInitFile::ParsePlayerInitFile(Player& player, const std::string & playerInputfileName)
{
	//read file
	std::ifstream inFile(playerInputfileName);
	bool isFileOK = true;

	if (!CheckOpenInputFile(inFile, playerInputfileName))
	{
		// Already printed error if any.
		mGame->mProblematicLineOfPlayer[player.GetPlayerNum() - 1] = 0;
		return false;
	}

	//otherwise, file exists
	std::string line;
	int lineNum = 1;
	while ((std::getline(inFile, line)) && isFileOK)
	{
		if (!processLine(player, line, lineNum, BAD_POS_PLAYER))
		{
			mGame->mProblematicLineOfPlayer[player.GetPlayerNum()-1] = lineNum;

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
