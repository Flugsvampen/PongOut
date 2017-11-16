#include "Game.h"
#include "NetworkManager.h"

#include <iostream>
#include <thread>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

std::vector<sf::Drawable*> Game::drawables;

Game::Game()
{
	Bind("connect", std::bind(&Game::Connect, this, sf::Packet()));
	sf::Packet packet;
	packet << "packet message";
	Call("connect", packet);
	//std::thread run = std::thread(&Game::Run, this);
	//run.join();
}


Game::~Game()
{
}


void Game::Run()
{
	// Creates a network manager that takes care of receive and send calls
	NetworkManager* manager = new NetworkManager();

	// Creates the RenderWindow
	sf::RenderWindow window(sf::VideoMode(800, 800), "PongOut - Client");

	// The main loop - ends as soon as the window is closed
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

		// Draws every drawable objects in the game
		for (auto drawable : drawables)
		{
			window.draw(*drawable);
		}

		// End the current frame and display its contents on screen
		window.display();
	}
}


bool Game::Bind(const std::string & name, GameFunction func)
{
	// Checks if the key string is already in use
	if (FoundInFunctionMap(name))
	{
		std::cout << "The key name \"" << name << "\" has already been bound to a function" << std::endl;
		return false;
	}

	// Inserts the new name and function in the map
	functionMap.insert(std::make_pair(name, func));
	return true;
}


void Game::Call(const std::string& func, sf::Packet packet)
{
	functionMap[func](packet);
}

// Checks if the key is used in functionMap
bool Game::FoundInFunctionMap(const std::string& name)
{
	return functionMap.find(name) != functionMap.end();
}


void Game::Connect(sf::Packet packet)
{
	std::string message;
	packet >> message;
	std::cout << "You connected to the server!" << message << std::endl;
}
