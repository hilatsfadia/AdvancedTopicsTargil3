#ifndef ADTO_TARGIL1_PARSER_INIT_FILE_H
#define ADTO_TARGIL1_PARSER_INIT_FILE_H

//--------------------------
// ParserInitFile Interface
//--------------------------

// Handles the parsing of a positioning file that was given by the 
// player, that instructs how to put his tools on the game board.

// @author Hila Tsfadia, Jael Fafner TAU - Advanced Topics in Programming - 2018 Semester B

#include "Parser.h"

#define INVALID_PIECE_POSITION std::make_unique<PiecePositionImpl>(PointImpl(-1, -1), '0')

class ParserInitFile :
	public Parser
{
protected:
	// Get piece positions from joker tokens, and already parsed position
	bool processJokerLine(const std::vector<std::string>& tokens, PointImpl& pos, 
		std::vector<std::unique_ptr<PiecePosition>>& vectorToFill) const;
	
	// Get piece positions from non joker tokens
	bool ProcessLineTokens(const std::vector<std::string>& tokens, 
		std::vector<std::unique_ptr<PiecePosition>>& vectorToFill) const;

	// TODO:
	// Tries to split the line into tokens. 
	// Calls processLineTokens while skipping empty lines.
	bool ProcessLine(const std::string& line, 
		std::vector<std::unique_ptr<PiecePosition>>& vectorToFill) const;

public:

	// Put only an error piece position in the given vector.
	void PutErrorPositionInVector(std::vector<std::unique_ptr<PiecePosition>>& vectorToFill) const;

	// Handles the parsing of the entire positions file of one player.
	// Returns false if it has a bad format. 
	void ParsePlayerInitFile(int player, const std::string& input_file_name, 
		std::vector<std::unique_ptr<PiecePosition>>& vectorToFill) const;
};

#endif //ADTO_TARGIL1_PARSER_INIT_FILE_H
