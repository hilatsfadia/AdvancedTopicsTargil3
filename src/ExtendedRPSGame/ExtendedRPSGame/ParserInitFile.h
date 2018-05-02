#ifndef ADTO_TARGIL1_PARSER_INIT_FILE_H
#define ADTO_TARGIL1_PARSER_INIT_FILE_H

#include "Parser.h"

// TODO:
#define JOKER_CHAR_PLAYER_1 'J'
#define JOKER_CHAR_PLAYER_2 'j'

class ParserInitFile :
	public Parser
{
protected:
	// Put a joker piece in given position. 
	// Get detailed about positioning from the given tokens, and checks for tokens validity.
	bool processJokerLine(int player, const std::vector<std::string>& tokens, Point* pos, std::vector<std::unique_ptr<PiecePosition>>& vectorToFill);
	
	// Process a line tokens. 
	// Checks for tokens validity.
	// Execute the line.
	bool ProcessLineTokens(int player, const std::vector<std::string>& tokens, int lineNum, std::vector<std::unique_ptr<PiecePosition>>& vectorToFill);

	// TODO:
	// Tries to split the line into tokens. 
	// Calls processLineTokens while skipping empty lines.
	bool ProcessLine(int player, const std::string& line, int lineNum, const char* templateBadFormatMessage, std::vector<std::unique_ptr<PiecePosition>>& vectorToFill);

public:
	ParserInitFile(Game* game) : Parser(game) {}
	ParserInitFile() {}
	~ParserInitFile();

	// Handles the parsing of the entire positions file of one player.
	// Returns false if it has a bad format. 
	// (In this case, updates the line number of the error).
	virtual bool ParsePlayerInitFile(int player, const std::string& input_file_name, std::vector<std::unique_ptr<PiecePosition>>& vectorToFill);
};

#endif //ADTO_TARGIL1_PARSER_INIT_FILE_H
