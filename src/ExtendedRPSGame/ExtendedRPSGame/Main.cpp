#include "stdafx.h"
#include "Game.h"
#include "FilePlayerAlgorithm.h"
#include "Parser.h"
#include <iostream>
#include "AutoPlayerAlgorithm.h"

#define COMMAND_LINE_AUTO_ALGORITHM_STR "auto"
#define COMMAND_LINE_FILE_ALGORITHM_STR "file"
#define COMMNAD_LINE_VS_STR "-vs-"

using std::string;
using std::cout;
using std::endl;

void PrintUsage()
{
	cout << "Usage of the program: you should give a parameter of one of the following options: "
		<< COMMAND_LINE_AUTO_ALGORITHM_STR << COMMNAD_LINE_VS_STR << COMMAND_LINE_FILE_ALGORITHM_STR << endl
		<< COMMAND_LINE_FILE_ALGORITHM_STR << COMMNAD_LINE_VS_STR << COMMAND_LINE_AUTO_ALGORITHM_STR << endl
		<< COMMAND_LINE_AUTO_ALGORITHM_STR << COMMNAD_LINE_VS_STR << COMMAND_LINE_AUTO_ALGORITHM_STR << endl
		<< COMMAND_LINE_FILE_ALGORITHM_STR << COMMNAD_LINE_VS_STR << COMMAND_LINE_FILE_ALGORITHM_STR << endl;
}

// Creates a player algorithm object according to the given str.
PlayerAlgorithm* getPlayerAlgoritmFromStr(string str)
{
	if (str == COMMAND_LINE_AUTO_ALGORITHM_STR)
	{
		return new AutoPlayerAlgorithm();
	}
	else if (str == COMMAND_LINE_FILE_ALGORITHM_STR)
	{
		return new FilePlayerAlgorithm();
	}

	return nullptr;
}

int main(int argc, char *argv[])
{
	// Number of parameters is illegal
	if (argc != 2)
	{
		PrintUsage();
		return 1;
	}

	string algorithmsOption = argv[1];
	std::vector<string> algorithmsStrings;
	Parser::Split(algorithmsOption, '-', algorithmsStrings);

	// Unknown parameter
	if (algorithmsStrings.size() != NUM_OF_PLAYERS+1)
	{
		PrintUsage();
		return 1;
	}

	// Unknown parameter
	if (algorithmsStrings[1] != "vs")
	{
		PrintUsage();
		return 1;
	}

	PlayerAlgorithm* player1Algorithm = getPlayerAlgoritmFromStr(algorithmsStrings[0]);
	PlayerAlgorithm* player2Algorithm = getPlayerAlgoritmFromStr(algorithmsStrings[2]);

	// Unknown parameter
	if ((player1Algorithm == nullptr) || (player2Algorithm == nullptr))
	{
		PrintUsage();
		return 1;
	}

	Game game(player1Algorithm, player2Algorithm);
	game.RunGame();

	delete player1Algorithm;
	delete player2Algorithm;

    return 0;
}
