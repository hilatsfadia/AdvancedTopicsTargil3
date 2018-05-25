#include "stdafx.h"
#include "FilePlayerAlgorithm.h"
#include "ParserInitFile.h"
#include "MoveImpl.h"
#include "JokerChangeImpl.h"
#include "ParserMoveFile.h"
#include "Parser.h"

using std::string;

FilePlayerAlgorithm::~FilePlayerAlgorithm()
{
	if (mMovesFileStream.is_open())
	{
		mMovesFileStream.close();
	}
}

void FilePlayerAlgorithm::getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill)
{
    ParserInitFile initFileParser;
	initFileParser.ParsePlayerInitFile(player, Parser::GetPositionsFileName(player), vectorToFill);
	mPlayerNum = player;
}

unique_ptr<Move> FilePlayerAlgorithm::getMove()
{
	if (!mMovesFileStream.is_open())
	{
		std::string movesFileName = Parser::GetMovesFileName(mPlayerNum);
		mMovesFileStream.open(movesFileName);

		if (!Parser::CheckOpenInputFile(mMovesFileStream, movesFileName)) // if files doesn't exist
		{
			Parser::PrintUsageMessage(mPlayerNum);

			// Already printed error if any.
			// TODO: mGame->SetBadInputFileMessageWithWinner(i+1, mGame->GetWinner(i+1), 0, BAD_MOVE_PLAYER);
			return nullptr;
		}
	}

	//mMovesFileLineNum++;

	return mMoveFileParser.ParseCurrMove(mMovesFileStream);
}

unique_ptr<JokerChange> FilePlayerAlgorithm::getJokerChange()
{
	return mMoveFileParser.ParseCurrJokerChange();
}


