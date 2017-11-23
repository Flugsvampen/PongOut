#include "Game.h"
#include "NetworkManager.h"
#include "Player.h"
#include "Ball.h"
#include "PacketOverloads.h"
#include "Keyboard.h"

#include <iostream>
#include <thread>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>


const int TICK_RATE = 1 / 60;

Game::Game()
{
	/* Binds the functionMap methods */
	Bind("message", std::bind(&Game::Message, this, std::placeholders::_1));
	Bind("addPlayer", std::bind(&Game::AddPlayer, this, std::placeholders::_1));
	Bind("move", std::bind(&Game::MoveObject, this, std::placeholders::_1));

	Keyboard::Initialize();
}


Game::~Game()
{
}


void Game::Run()
{
	// Creates a network manager that takes care of receive and send calls
	NetworkManager* manager = new NetworkManager(this);

	// Creates the RenderWindow
	sf::RenderWindow window(sf::VideoMode(800, 800), "PongOut - Client");
	window.setFramerateLimit(60);
	sf::Clock clock;
	sf::Time dt;

	// The main loop - ends as soon as the window is closed
	while (window.isOpen())
	{
		if (clock.getElapsedTime().asSeconds() < TICK_RATE)
			continue;

		dt = clock.restart();

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
			else if (event.type == sf::Event::KeyReleased)
			{
				Keyboard::setKeyUp(event.key.code);
			}
			else if (event.type == sf::Event::LostFocus || event.type == sf::Event::GainedFocus)
				Keyboard::clearKeys();
		}
		// Clear the whole window before rendering a new frame
		window.clear(sf::Color::Black);

		// Draws every drawable objects in the game
		for (auto it : objectMap)
		{
			GameObject* obj = it.second;

			obj->Update(dt);

			for (auto it2 : objectMap)
			{ 
				obj->CheckCollision(*it2.second);
			}
			obj->UpdateLastPosition();
			window.draw(obj->GetShape());
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

// Adds the parameter object to the object map and uses its tag as the key value
void Game::AddObjectToMap(GameObject* obj)
{
	objectMap.insert(StringToObjPair(obj->GetTag(), obj));
}


// Checks if the key is used in functionMap
bool Game::FoundInFunctionMap(const std::string& name)
{
	return functionMap.find(name) != functionMap.end();
}


GameObject * Game::FindInObjectMap(const std::string & name)
{
	auto it = objectMap.find(name);
	if (it == objectMap.end())
		return nullptr;

	return it->second;
}


void Game::Message(sf::Packet & packet)
{
	std::string message;
	packet >> message;
	std::cout << message << std::endl;
}


void Game::AddPlayer(sf::Packet & packet)
{
	int playerNr;
	sf::Vector2f pos;

	packet >> pos >> playerNr;

	player = new Player(pos);
	player->SetTag(player->GetTag() + std::to_string(playerNr));
	player->GetBall()->SetTag(player->GetBall()->GetTag() + std::to_string(playerNr));
		
	AddObjectToMap(player->GetBall());
	AddObjectToMap(player);

	packet >> pos >> playerNr;

	// We make the player from the other client a GameObject so they can't interact with this client
	GameObject* otherPlayer = new GameObject("player" + std::to_string(playerNr), sf::Color::Green, sf::Vector2f(100, 25), pos);
	// Same as with the other player
	GameObject* ball = new GameObject("ball" + std::to_string(playerNr), sf::Color::Blue, sf::Vector2f(20, 20), sf::Vector2f(0, -100));

	AddObjectToMap(otherPlayer);
	AddObjectToMap(ball);
		
}


void Game::MoveObject(sf::Packet& packet)
{
	std::string objName;
	sf::Vector2f position;
	if (!(packet >> objName >> position))
		return;
	
	GameObject* obj = FindInObjectMap(objName);

	obj->SetPosition(position);
}


void Game::Shoot(sf::Packet & packet)
{
	std::string objName;
	if (!(packet >> objName))
		return;

	Player* player = dynamic_cast<Player*>(FindInObjectMap(objName));
	player->SetHasShot(true);
}
