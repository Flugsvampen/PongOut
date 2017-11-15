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

#include <chrono>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "Player.h"

int main(int argc, char* argv[])
{
	std::cout << "This is the client!" << std::endl;

	sf::RenderWindow window(sf::VideoMode(800, 600), "PongOut");
	window.setFramerateLimit(60);

	Player player;

	player.Initialize();

	while (window.isOpen())
	{
		// Event processing
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Request for closing the window
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		// Clear the whole window before rendering a new frame
		window.clear();

		// End the current frame and display its contents on screen
		window.display();
	}

	return 0;
}