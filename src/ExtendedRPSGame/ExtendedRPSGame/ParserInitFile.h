#ifndef ADTO_TARGIL1_PARSER_INIT_FILE_H
#define ADTO_TARGIL1_PARSER_INIT_FILE_H

#include "Parser.h"

#define INIT_LINE_TOKENS_COUNT_WITH_JOKER 4
#define INIT_LINE_TOKENS_COUNT_WITHOUT_JOKER 3
//#define PIECE_CHAR_TOKEN_WITHOUT_JOKER_NUM 0
#define X_TOKEN_NUM 1
#define Y_TOKEN_NUM 2
#define J_TOKEN_NUM 0
#define PIECE_CHAR_TOKEN_WITH_JOKER_NUM 3
#define JOKER_CHAR_PLAYER_1 'J'
#define JOKER_CHAR_PLAYER_2 'j'

class ParserInitFile :
	public Parser
{
protected:
	// Put a joker piece in given position. 
	// Get detailed about positioning from the given tokens, and checks for tokens validity.
	bool processJokerLine(Player& player, const std::vector<std::string>& tokens, PointImpl& pos);

	// Put a non joker piece in given position.  
	// Get detailed about positioning from the given tokens, and checks for tokens validity.
	bool processNonJokerLine(Player& player, const std::vector<std::string>& tokens, PointImpl& pos);

	// Process a line tokens. 
	// Checks for tokens validity.
	// Execute the line.
	bool processLineTokens(Player& player, const std::vector<std::string>& tokens, int lineNum) override;
public:
	ParserInitFile(Game* game) : Parser(game) {}
	~ParserInitFile();

	// Handles the parsing of the entire positions file of one player.
	// Returns false if it has a bad format. 
	// (In this case, updates the line number of the error).
	virtual bool ParsePlayerInitFile(Player& player, const std::string& input_file_name);
};

#endif //ADTO_TARGIL1_PARSER_INIT_FILE_H
