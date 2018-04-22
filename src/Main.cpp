#include "stdafx.h"
#include "Game.h"

int main()
{
	Game* game = new Game();
	if (game->InitGame())
	{
		game->Play();
	}

    return 0;
}
