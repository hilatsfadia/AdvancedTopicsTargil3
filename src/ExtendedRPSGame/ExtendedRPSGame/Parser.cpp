#include "stdafx.h"
#include "Parser.h"
#include "Game.h"
#include "Joker.h"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>

#define PLAYER_POSITION_FILE "player%d.rps_board"
#define PLAYER_MOVE_FILE "player%d.rps_moves"

using namespace std;

void Parser::Split(const std::string& s, char delim, std::vector<std::string>& outElems)
{
	std::stringstream strSt(s);
	std::string item;

	while (std::getline(strSt, item, delim)) //we could omit the 'std::' notion from here.
	{
		outElems.push_back(item); //push_back is a function of vector class, it inserted to the back of the vector.
	}
}

inline void Parser::Ltrim(std::string& s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !std::isspace(ch);
	}));
}

inline void Parser::Rtrim(std::string& s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

inline void Parser::Trim(std::string& s) {
	Ltrim(s);
	Rtrim(s);
}

void Parser::RemoveExtraWhitespaces(const string &input, string &output)
{
	output.clear();  // we don't want to add at the end of existing string...
	unique_copy(input.begin(), input.end(), back_insert_iterator<string>(output),
		[](char a, char b) { return isspace(a) && isspace(b); });
}

void Parser::GetTokensFromLine(const std::string& line, std::vector<std::string>& outTokens) const
{
	string lineWithoutExtraSpaces(line);
	RemoveExtraWhitespaces(line, lineWithoutExtraSpaces);
	Trim(lineWithoutExtraSpaces);
	Split(lineWithoutExtraSpaces, ' ', outTokens);
}

bool Parser::CheckOpenInputFile(const std::ifstream& inFile, const std::string& inFileName)
{
	if ((!inFile.is_open()) || (inFile.fail())) {
		//File does not exist code here
		std::cout << "Error while opening file " + inFileName << std::endl;
		// print usage
		// TODO: Game::PrintUsageMessage();
		// TODO: mGame->isInputFileCannotBeOpened = true;
		return false;
	}

	return true;
}

bool Parser::CheckReadOK(const std::ifstream& inFile, const std::string& playerfileName) const
{
	if (inFile.bad())
	{
		std::cout << "Read/writing error on i/o operation while reading file: " + playerfileName << std::endl;
		// TODO: mGame->mProblematicLineOfPlayer[player.GetPlayerNum()-1] = lineNum;
		return false;
	}

	return true;
}

unique_ptr<PointImpl> Parser::GetPositionFromChars(const std::string & posx, const std::string & posy)
{
	try
	{
		return std::make_unique<PointImpl>( stoi(posx), stoi(posy) );
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		// TODO: mGame->SetBadInputFileMessageWithWinner(playerNum, mGame->GetWinner(playerNum), lineNum, BAD_MOVE_PLAYER);
		return nullptr;
	}
}

 // TODO: fileUsage message
void Parser::PrintUsageMessage(int playerNum)
{
	cout << "Usage of the program has to include in the running directory, those files: " +
		GetPositionsFileName(playerNum) << ", " << GetMovesFileName(playerNum) 
		<< ", because it's a file player " << endl;
}

// TODO: sprintf
string Parser::GetPositionsFileName(int playerNum)
{
	char fileName[INPUT_FILE_NAME_MAX_LEN];
	sprintf_s(fileName, PLAYER_POSITION_FILE, playerNum);
	return fileName;
}

string Parser::GetMovesFileName(int playerNum)
{
	char fileName[INPUT_FILE_NAME_MAX_LEN];
	sprintf_s(fileName, PLAYER_MOVE_FILE, playerNum);
	return fileName;
}