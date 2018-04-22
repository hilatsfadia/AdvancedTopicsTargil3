#include "stdafx.h"
#include "Parser.h"
#include "Game.h"
#include "Joker.h"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>

using namespace std;

Parser::~Parser()
{
}


void Parser::Split(const std::string& s, char delim, std::vector<std::string>& elems)
{
	std::stringstream strSt(s);
	std::string item;

	while (std::getline(strSt, item, delim)) //we could omit the 'std::' notion from here.
	{
		elems.push_back(item); //push_back is a function of vector class, it inserted to the back of the vector.
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

void Parser::removeExtraWhitespaces(const string &input, string &output)
{
	output.clear();  // we don't want to add at the end of existing string...
	unique_copy(input.begin(), input.end(), back_insert_iterator<string>(output),
		[](char a, char b) { return isspace(a) && isspace(b); });
}

void Parser::GetTokensFromLine(const std::string& line, std::vector<std::string>& outTokens)
{
	string lineWithoutExtraSpaces(line);
	removeExtraWhitespaces(line, lineWithoutExtraSpaces);
	Trim(lineWithoutExtraSpaces);
	Split(lineWithoutExtraSpaces, ' ', outTokens);
}

bool Parser::CheckOpenInputFile(const std::ifstream& inFile, const std::string& inFileName)
{
	if ((!inFile.is_open()) || (inFile.fail())) {
		//File does not exist code here
		std::cout << "Error while opening file " + inFileName << std::endl;
		// print usage
		Game::PrintUsageMessage();
		mGame->isInputFileCannotBeOpened = true;
		return false;
	}

	return true;
}

bool Parser::processLine(Player& player, const std::string& line, int lineNum, const char* templateBadFormatMessage)
{
	try
	{
		vector<string> tokens;
		GetTokensFromLine(line, tokens);

		// Skip empty lines
		if (tokens.size() != 0)
		{
			return processLineTokens(player, tokens, lineNum);// Maybe Bad Format
			// Already printed error if any.
		}
	}
	//catch (const std::exception&)
	// Souldn't get here!
	catch (...)
	{
		// Print errors if any?
		int playerNum = player.GetPlayerNum();
		mGame->SetBadInputFileMessageWithWinner(playerNum, mGame->GetWinner(playerNum), lineNum, templateBadFormatMessage);
		return false; // Bad Format
	}

	return true;
}

bool Parser::ChangeJokerActualType(Joker* joker, char cJokerRepresantation)
{
	Piece* actualPiece = PieceFactory::GetPieceFromChar(cJokerRepresantation);
	if (actualPiece == nullptr)
	{
		cout << "PIECE_CHAR in positions file should be one of: R P S B F" << endl;
		return false;
	}

	actualPiece->SetTransparentOwner(joker->GetOwner());

	// If not a valid PIECE for a Joker
	if (!joker->SetActualPieceType(actualPiece))
	{
		cout << "PIECE_CHAR for joker can be: R P S B" << endl;
		return false;
	}

	return true;
}

bool Parser::InitJokerOwnerAndActualType(Joker* joker, char cJokerRepresantation, Player* owner)
{
	if (!joker->InitializeOwner(owner))
	{
		std::cout << "A PIECE type appears in file more than its number" << std::endl;
		return false;
	}

	if (!ChangeJokerActualType(joker, cJokerRepresantation))
	{
		// Already printed error
		return false;
	}

	return true;
}

bool Parser::CheckReadOK(Player& player, std::ifstream& inFile, const std::string& playerfileName, int lineNum)
{
	if (inFile.bad())
	{
		std::cout << "Read/writing error on i/o operation while reading file: " + playerfileName << std::endl;
		mGame->mProblematicLineOfPlayer[player.GetPlayerNum()-1] = lineNum;
		return false;
	}

	return true;
}

bool Parser::GetPositionFromChars(const std::string & posy, const std::string & posx, Board::BoardPosition& outPos, int playerNum, int lineNum)
{
	try
	{
		outPos = { stoi(posy), stoi(posx) };
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		mGame->SetBadInputFileMessageWithWinner(playerNum, mGame->GetWinner(playerNum), lineNum, BAD_MOVE_PLAYER);
		return false;
	}

	return true;
}
