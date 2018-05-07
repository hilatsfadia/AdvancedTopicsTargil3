#include "stdafx.h"
#include "FilePlayerAlgorithm.h"
#include "ParserInitFile.h"
#include "MoveImpl.h"
#include "JokerChangeImpl.h"
#include <cstdio>
//#include <vector>
//#include <fstream>
//#include "MoveImpl.h"
#include "ParserMoveFile.h"
#include "Parser.h"

#define INPUT_FILE_NAME_MAX_LEN 100
#define PLAYER_POSITION_FILE "player%d.rps_board"
#define PLAYER_MOVE_FILE "player%d.rps_moves"

using std::string;
//using std::vector;
//using std::cout;
//using std::endl;

// TODO: sprintf
string FilePlayerAlgorithm::GetPositionsFileName(int playerNum)
{
	char fileName[INPUT_FILE_NAME_MAX_LEN];
	sprintf_s(fileName, PLAYER_POSITION_FILE, playerNum);
	return fileName;
}

string FilePlayerAlgorithm::GetMovesFileName(int playerNum)
{
	char fileName[INPUT_FILE_NAME_MAX_LEN];
	sprintf_s(fileName, PLAYER_MOVE_FILE, playerNum);
	return fileName;
}

void FilePlayerAlgorithm::getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill)
{
    ParserInitFile initFileParser;
	initFileParser.ParsePlayerInitFile(player, GetPositionsFileName(player), vectorToFill);

	// TODO: ask
	mPlayerNum = player;
	std::string movesFileName = GetMovesFileName(player);
	mMovesFileStream.open(movesFileName);

	if (!Parser::CheckOpenInputFile(mMovesFileStream, movesFileName))
	{
		// Already printed error if any.
		// TODO: mGame->SetBadInputFileMessageWithWinner(i+1, mGame->GetWinner(i+1), 0, BAD_MOVE_PLAYER);
		// TODO: mGame->mProblematicLineOfPlayer[player.GetPlayerNum() - 1] = 0;
		// TODO: return false;
	}

	//otherwise, files exist
}

unique_ptr<Move> FilePlayerAlgorithm::getMove()
{
	mMovesFileLineNum++;

	ParserMoveFile moveFileParser;
	return moveFileParser.ParsePlayerMove(mPlayerNum, mMovesFileStream, mMovesFileLineNum);
}

unique_ptr<JokerChange> FilePlayerAlgorithm::getJokerChange()
{
	// TODO: parse continuing line from move_file
	return nullptr;
}


