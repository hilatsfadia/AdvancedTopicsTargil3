#ifndef ADTO_TARGIL1_PARSER_MOVE_FILE_H
#define ADTO_TARGIL1_PARSER_MOVE_FILE_H

//--------------------------
// ParserMoveFile Interface
//--------------------------

// Handles the parsing of a moves file that was given by the player, 
// that instructs how to move his tools on the game board, in the flow of the game.

// @author Hila Tsfadia, Jael Fafner TAU - Advanced Topics in Programming - 2018 Semester B

#include "Parser.h"

#define INVALID_JOKER_CHANGE make_unique<JokerChangeImpl>(PointImpl(-1, -1), '5')

class ParserMoveFile :
	public Parser
{
private:
	std::vector<std::string> currMoveLineTokens;

protected:
	

	// Process a line tokens. 
	// Checks for tokens validity.
	// Execute the line.
	unique_ptr<Move> ProcessMoveLineTokens();

	// In case Moves file is done (all lines were used) and the
	// opponent still have moves : the opponent (given player parameter) will still
	// move and the game will continue, 
	// the players whose Moves file was done will not move.
	//void ParseRemainingPlayerMoves(Player& player, std::ifstream& playerMoveFileStream, int lineNum);

public:
	// Handle line move of the given player.
	// The given stream points to the relevant line.
	// Tries to split the line into tokens. 
	// Calls processLineTokens while skipping empty lines.
	unique_ptr<Move> ParseCurrMove(std::ifstream& playerMoveFileStream);

	// Process the joker tokens. 
	// Checks for tokens validity.
	// Execute the joker representation change.
	unique_ptr<JokerChange> ParseCurrJokerChange();
};

#endif //ADTO_TARGIL1_PARSER_MOVE_FILE_H