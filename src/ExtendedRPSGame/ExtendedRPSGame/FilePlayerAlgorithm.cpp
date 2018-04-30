#include "stdafx.h"
#include "FilePlayerAlgorithm.h"
#include "ParserInitFile.h"
#include "MoveImpl.h"
#include "JokerChangeImpl.h"
#include <cstdio>

using std::string;

// TODO: sprintf
string FilePlayerAlgorithm::GetPositionsFileName(int playerNum)
{
	char fileName[INPUT_FILE_NAME_MAX_LEN];
	sprintf_s(fileName, PLAYER_POSITION_FILE, playerNum);
	return fileName;
}

void FilePlayerAlgorithm::getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill)
{
    ParserInitFile initFileParser;
	initFileParser.ParsePlayerInitFile(player, GetPositionsFileName(player), vectorToFill);
}

unique_ptr<Move> FilePlayerAlgorithm::getMove()
{
	// TODO: parse line from move_file
	return nullptr;
}

unique_ptr<JokerChange> FilePlayerAlgorithm::getJokerChange()
{
	// TODO: parse continuing line from move_file
	return nullptr;
}