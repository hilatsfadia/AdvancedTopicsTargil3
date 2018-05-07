#ifndef ADTO_TARGIL1_PARSER_MOVE_FILE_H
#define ADTO_TARGIL1_PARSER_MOVE_FILE_H

#include "Parser.h"

class ParserMoveFile :
	public Parser
{
protected:
	// Process the joker tokens. 
	// Checks for tokens validity.
	// Execute the joker representation change.
	//bool processLineJokerTokens(Player& player, const std::vector<std::string>& tokens, int lineNum);

	// Process a line tokens. 
	// Checks for tokens validity.
	// Execute the line.
	unique_ptr<Move> ProcessMoveLineTokens(int playerNum, const std::vector<std::string>& tokens, int lineNum);

	// In case Moves file is done (all lines were used) and the
	// opponent still have moves : the opponent (given player parameter) will still
	// move and the game will continue, 
	// the players whose Moves file was done will not move.
	//void ParseRemainingPlayerMoves(Player& player, std::ifstream& playerMoveFileStream, int lineNum);

	// Tries to split the line into tokens. 
	// Calls processLineTokens while skipping empty lines.
	//unique_ptr<Move> ProcessLine(Player& player, const std::string& line, int lineNum, const char* templateBadFormatMessage);
public:
	~ParserMoveFile();

	// Plays a the current line move of the given player.
	unique_ptr<Move> ParsePlayerMove(int playerNum, std::ifstream& playerMoveFileStream, int lineNum);

	// Handles the parsing of the moves files of the players. 
	// Each one executes a move in his turn.
	// In case of an error, or a winning move, stops playing.
	// If there is a bad format, updates the line number of the error as well as the message and winner.
	//virtual void ParsePlayersMoveFiles(Player players[], int numOfPlayers, std::vector<std::string>& playerFileNames);
};

#endif //ADTO_TARGIL1_PARSER_MOVE_FILE_H