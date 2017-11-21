#if _DEBUG
#pragma comment(lib, "sfml-system-d.lib")
#pragma comment(lib, "sfml-network-d.lib")
#pragma comment(lib, "sfml-graphics-d.lib")
#else
#pragma comment(lib, "sfml-system.lib")
#pragma comment(lib, "sfml-network.lib")
#pragma comment(lib, "sfml-graphics.lib")
#endif

#include <iostream>
#include <string>
#include "Game.h"


int main(int argc, char* argv[])
{
	std::cout << "This is the server!" << std::endl;

	Game game;
	game.Run();

	while (true);

	return 0;
}

