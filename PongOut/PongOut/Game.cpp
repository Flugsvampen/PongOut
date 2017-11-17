#include "Game.h"
#include "NetworkManager.h"
#include "Player.h"
#include "PacketOverloads.h"
#include "Keyboard.h"

#include <iostream>
#include <thread>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>


Game::Game()
{
	Bind("connect", std::bind(&Game::Connect, this, std::placeholders::_1));
	Bind("message", std::bind(&Game::Message, this, std::placeholders::_1));

	Keyboard::Initialize();
}


Game::~Game()
{
}


void Game::Run()
{
	// Creates a network manager that takes care of receive and send calls
	NetworkManager* manager = new NetworkManager(*this);

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
			else if (event.type == sf::Event::KeyPressed)
			{
				Keyboard::setKeyDown(event.key.code);
			}
		}
		// Clear the whole window before rendering a new frame
		window.clear(sf::Color::Transparent);

		// Draws every drawable objects in the game
		for (auto gameObject : gameObjects)
		{
			gameObject->Update();
			window.draw(gameObject->GetShape());
		}

		// End the current frame and display its contents on screen
		window.display();
	}
}

// Wrapper for calling functions in functionMap
void Game::CallFunction(sf::Packet& packet)
{
	std::string name;

	// If data couldn't be extracted from packet or if function couldn't be found in map
	if (!(packet >> name) || !FoundInFunctionMap(name))
		return;

	// Calls function
	functionMap[name](packet);
}

// Adds a function pointer to the functionMap
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


void Game::Call(const std::string& func, sf::Packet& packet)
{
	functionMap[func](packet);
}

// Checks if the key is used in functionMap
bool Game::FoundInFunctionMap(const std::string& name)
{
	return functionMap.find(name) != functionMap.end();
}


void Game::Connect(sf::Packet& packet)
{
	std::string tag;
	sf::Color color;
	sf::Vector2f size;
	sf::Vector2f pos;
	int speed;
	packet >> tag >> color >> size >> pos >> speed;

	Player* player = new Player(tag, color, size, pos, speed);
	gameObjects.push_back(player);
}


void Game::Message(sf::Packet & packet)
{
	std::string message;
	packet >> message;
	std::cout << message << std::endl;
}


void Game::AddPlayer(sf::Packet & packet)
{
	
}


