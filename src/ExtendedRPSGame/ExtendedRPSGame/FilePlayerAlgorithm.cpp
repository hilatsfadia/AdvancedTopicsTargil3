#include "stdafx.h"
#include "FilePlayerAlgorithm.h"
#include "ParserInitFile.h"
#include "MoveImpl.h"
#include "JokerChangeImpl.h"

void FilePlayerAlgorithm::getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill)
{
    //ParserInitFile initFileParser(this);
    //TODO: complete parsing
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