#pragma comment(lib, "rpc.lib")


#if _DEBUG
#pragma comment(lib, "sfml-system-d.lib")
#pragma comment(lib, "sfml-graphics-d.lib")
#pragma comment(lib, "sfml-window-d.lib")
#else
#pragma comment(lib, "sfml-system.lib")
#pragma comment(lib, "sfml-graphics.lib")
#pragma comment(lib, "sfml-window.lib")
#endif

#include <chrono>
#include <iostream>

#include <rpc/client.h>
#include <SFML/Graphics.hpp>

int main(int argc, char* argv[])
{
	std::cout << "This is the client!" << std::endl;

	sf::RenderWindow window(sf::VideoMode(800, 600), "PongOut");
	window.setFramerateLimit(60);
	

	std::this_thread::sleep_for(std::chrono::milliseconds(20));
	rpc::client* client = new rpc::client("127.0.0.1", 8081);

	std::cout << client->call("foo").as<std::string>() << std::endl;
	
	while (window.isOpen())
	{
		// Event processing
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Request for closing the window
			if (event.type == sf::Event::Closed)
				window.close();
		}
		// Clear the whole window before rendering a new frame
		window.clear();

		// End the current frame and display its contents on screen
		window.display();
	}

	return 0;
}