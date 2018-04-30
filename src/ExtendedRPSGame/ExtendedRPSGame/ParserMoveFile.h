#ifndef ADTO_TARGIL1_PARSER_MOVE_FILE_H
#define ADTO_TARGIL1_PARSER_MOVE_FILE_H

#include "Parser.h"

#define MOVE_LINE_TOKENS_COUNT_WITH_JOKER 8
#define MOVE_LINE_TOKENS_COUNT_WITHOUT_JOKER 4
#define FROM_X_TOKEN_NUM 0
#define FROM_Y_TOKEN_NUM 1
#define TO_X_TOKEN_NUM 2
#define TO_Y_TOKEN_NUM 3
#define J_TOKEN_NUM 4
#define JOKER_X_TOKEN_NUM 5
#define JOKER_Y_TOKEN_NUM 6
#define NEW_REP_TOKEN_NUM 7


class ParserMoveFile :
	public Parser
{
//protected:
//	// Process the joker tokens. 
//	// Checks for tokens validity.
//	// Execute the joker representation change.
//	bool processLineJokerTokens(Player& player, const std::vector<std::string>& tokens, int lineNum);
//
//	// Process a line tokens. 
//	// Checks for tokens validity.
//	// Execute the line.
//	bool ProcessLineTokens(Player& player, const std::vector<std::string>& tokens, int lineNum) override;
//
//	// Plays a the current line move of the given player.
//	bool HandlePlayerMove(Player& player, std::ifstream& playerMoveFileStream, int lineNum);
//
//	// In case Moves file is done (all lines were used) and the
//	// opponent still have moves : the opponent (given player parameter) will still
//	// move and the game will continue, 
//	// the players whose Moves file was done will not move.
//	void ParseRemainingPlayerMoves(Player& player, std::ifstream& playerMoveFileStream, int lineNum);
//
//	// Tries to split the line into tokens. 
//	// Calls processLineTokens while skipping empty lines.
//	bool ProcessLine(Player& player, const std::string& line, int lineNum, const char* templateBadFormatMessage);
//public:
//	ParserMoveFile(Game* game) : Parser(game) {}
//	~ParserMoveFile();
//
//	// Handles the parsing of the moves files of the players. 
//	// Each one executes a move in his turn.
//	// In case of an error, or a winning move, stops playing.
//	// If there is a bad format, updates the line number of the error as well as the message and winner.
//	virtual void ParsePlayersMoveFiles(Player players[], int numOfPlayers, std::vector<std::string>& playerFileNames);
};

#endif //ADTO_TARGIL1_PARSER_MOVE_FILE_H