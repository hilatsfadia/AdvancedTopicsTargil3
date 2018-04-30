#include "stdafx.h"
#include "Game.h"
#include "FilePlayerAlgorithm.h"
#include "Parser.h"

#define AUTO_ALGORITHM_STR "auto"
#define FILE_ALGORITHM_STR "file"

using std::string;

PlayerAlgorithm* getPlayerAlgoritmFromStr(string str)
{
	if (str == AUTO_ALGORITHM_STR)
	{
		//return new 
	}
	else if (str == FILE_ALGORITHM_STR)
	{
		return new FilePlayerAlgorithm();
	}

	return nullptr;
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		return 1;
	}

	string algorithmsOption = argv[1];
	std::vector<string> algorithmsStrings;
	Parser::Split(algorithmsOption, '-', algorithmsStrings);

	if (algorithmsStrings.size() != NUM_OF_PLAYERS)
	{
		return 1;
	}

	PlayerAlgorithm* player1Algorithm = getPlayerAlgoritmFromStr(algorithmsStrings[0]);
	PlayerAlgorithm* player2Algorithm = getPlayerAlgoritmFromStr(algorithmsStrings[1]);
	if ((player1Algorithm == nullptr) || (player2Algorithm == nullptr))
	{
		return 1;
	}

	Game game(*player1Algorithm, *player2Algorithm);
	game.RunGame();

	delete player1Algorithm;
	delete player2Algorithm;

    return 0;
}
