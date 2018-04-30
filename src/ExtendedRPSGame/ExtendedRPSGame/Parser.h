#ifndef ADTO_TARGIL1_PARSER_H
#define ADTO_TARGIL1_PARSER_H

#include <string>
#include <vector>
#include "Player.h"
#include "BoardImpl.h"
#include "PointImpl.h"
#include "PiecePosition.h"
#include <memory>

class Game;
class Joker;

class Parser
{
protected:
	// The game for which the parser works.
	Game* mGame;

	// trim from start (in place)
	static inline void Ltrim(std::string& s);

	// trim from end (in place)
	static inline void Rtrim(std::string& s);

	// trim from both ends (in place)
	static inline void Trim(std::string &s);

	// Removes duplicates of white spaces
	static void RemoveExtraWhitespaces(const std::string &input, std::string &output);

	// Splits the line into tokens, given in the out parameter,
	// ignoring extra white spaces.
	void GetTokensFromLine(const std::string& line, std::vector<std::string>& outTokens);

	// Returns true if and only if the file was opened successfully. Otherwise, prints
	// usage message and updates the problematic line number.
	bool CheckOpenInputFile(const std::ifstream& inFile, const std::string& inFileName);

	// Process the line tokens according to the information of what kind of file it is.
	// Checks for tokens validity.
	// Execute the line.
	// Implemented in sons.
	virtual bool ProcessLineTokens(Player& player, const std::vector<std::string>& tokens, int lineNum) { return true; }
	//virtual bool ProcessLineTokens(Player& player, const std::vector<std::string>& tokens, int lineNum) = 0;

	// Tries to split the line into tokens. 
	// Calls processLineTokens while skipping empty lines.
	//bool ProcessLine(Player& player, const std::string& line, int lineNum, const char* templateBadFormatMessage, std::vector<std::unique_ptr<PiecePosition>>& vectorToFill);

	// Returns true if and only if the state of inFile is not bad. If bad, prints
	// relevant message and updates the problematic line number.
	bool CheckReadOK(int player, std::ifstream& inFile, const std::string& playerfileName, int lineNum);

	bool GetPositionFromChars(const std::string& posy, const std::string& posx, PointImpl& outPos, int playerNum, int lineNum);
public:
	Parser(Game* game) : mGame(game) {}
	Parser() {}
	~Parser();

	//splits string s to a vector
	static void Split(const std::string& s, char delim, std::vector<std::string>& outElems);
};

#endif //ADTO_TARGIL1_PARSER_H