#if _DEBUG
#pragma comment(lib, "sfml-system-d.lib")
#pragma comment(lib, "sfml-graphics-d.lib")
#pragma comment(lib, "sfml-window-d.lib")
#pragma comment(lib, "sfml-network-d.lib")
#else
#pragma comment(lib, "sfml-system.lib")
#pragma comment(lib, "sfml-graphics.lib")
#pragma comment(lib, "sfml-window.lib")
#pragma comment(lib, "sfml-network.lib")
#endif

#include "Game.h"
#include <thread>
#include <chrono>

int main(int argc, char* argv[])
{
	std::this_thread::sleep_for(std::chrono::seconds(2));
	Game game;
	game.Run();

	return 0;
}