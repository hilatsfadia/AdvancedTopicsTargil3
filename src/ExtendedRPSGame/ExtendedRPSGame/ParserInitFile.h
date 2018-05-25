#ifndef ADTO_TARGIL1_PARSER_INIT_FILE_H
#define ADTO_TARGIL1_PARSER_INIT_FILE_H

//--------------------------
// ParserInitFile Interface
//--------------------------

// Handles the parsing of a positioning file that was given by the 
// player, that instructs how to put his tools on the game board.

// @author Hila Tsfadia, Jael Fafner TAU - Advanced Topics in Programming - 2018 Semester B

#include "Parser.h"

class ParserInitFile :
	public Parser
{
protected:
	// Put a joker piece in given position. 
	// Get detailed about positioning from the given tokens, and checks for tokens validity.
	bool processJokerLine(const std::vector<std::string>& tokens, PointImpl& pos, std::vector<std::unique_ptr<PiecePosition>>& vectorToFill);
	
	// Process a line tokens. 
	// Checks for tokens validity.
	// Execute the line.
	bool ProcessLineTokens(const std::vector<std::string>& tokens, std::vector<std::unique_ptr<PiecePosition>>& vectorToFill);

	// TODO:
	// Tries to split the line into tokens. 
	// Calls processLineTokens while skipping empty lines.
	bool ProcessLine(const std::string& line, std::vector<std::unique_ptr<PiecePosition>>& vectorToFill);

public:
	// Handles the parsing of the entire positions file of one player.
	// Returns false if it has a bad format. 
	// (In this case, updates the line number of the error).
	void ParsePlayerInitFile(int player, const std::string& input_file_name, std::vector<std::unique_ptr<PiecePosition>>& vectorToFill);
};

#endif //ADTO_TARGIL1_PARSER_INIT_FILE_H
