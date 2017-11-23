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
	font.loadFromFile("trench100free.otf");
	winLoseText = sf::Text("", font);
	winLoseText.setOrigin(0.5f, 0.5f);
	winLoseText.setPosition(400, 400);

	GameObject::game = this;

	/* Binds the functionMap methods */
	Bind("message", std::bind(&Game::Message, this, std::placeholders::_1));
	Bind("initializeGame", std::bind(&Game::InitializeGame, this, std::placeholders::_1));
	Bind("move", std::bind(&Game::MoveObject, this, std::placeholders::_1));
	Bind("changeColor", std::bind(&Game::ChangeColor, this, std::placeholders::_1));
	Bind("damage", std::bind(&Game::TakeDamage, this, std::placeholders::_1));
	Bind("win", std::bind(&Game::WinGame, this, std::placeholders::_1));
	
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

		// Updates all objects
		for (auto it : objectMap)
		{
			GameObject* obj = it.second;

			obj->Update(dt);

			// Checks collision with every other object
			for (auto it2 : objectMap)
			{ 
				obj->CheckCollision(*it2.second);
			}

			obj->UpdateLastPosition();
			window.draw(obj->GetShape());
		}

		if (winLoseText.getString() != "")
			window.draw(winLoseText);
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


std::vector<class GameObject*> Game::GetPlayerObjects()
{
	return playerObjects;
}


void Game::SetWinLoseText(const std::string & text, const sf::Color& color = sf::Color::White)
{
	winLoseText.setFillColor(color);
	winLoseText.setString(sf::String(text));
}

void Game::LoseGame()
{
	player->SetCanInput(false);
	for (auto it : objectMap)
	{
		GameObject* obj = it.second;
		std::string tag = obj->GetTag();
		if(tag.find("player") != tag.npos || tag.find("ball") != tag.npos)
			obj->SetColor(sf::Color::Black);
	}

	SendWinCommand();

	SetWinLoseText("You Lost...", sf::Color::Red);
}

void Game::SetNetworkManager(NetworkManager * m)
{
	manager = m;
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

// Searches for object and returns it
GameObject* Game::FindInObjectMap(const std::string & name)
{
	auto it = objectMap.find(name);
	if (it == objectMap.end())
		return nullptr;

	return it->second;
}

// Prints a message
void Game::Message(sf::Packet & packet)
{
	std::string message;
	packet >> message;
	std::cout << message << std::endl;
}

// Adds all game objects to the game
void Game::InitializeGame(sf::Packet & packet)
{
	int playerNr;
	sf::Vector2f pos;

	packet >> pos >> playerNr;

	// A player is created and within the player constructor we create a ball
	player = new Player(pos);
	player->SetTag(player->GetTag() + std::to_string(playerNr));
	player->GetBall()->SetTag(player->GetBall()->GetTag() + std::to_string(playerNr));

	// We add the player and ball to the objectMap
	AddObjectToMap(player->GetBall());
	AddObjectToMap(player);

	packet >> pos >> playerNr;

	// We make the player from the other client a GameObject so they can't interact with this client
	GameObject* otherPlayer = new GameObject("player" + std::to_string(playerNr), sf::Color::Green, sf::Vector2f(100, 25), pos);

	// Same as with the other player
	GameObject* ball = new GameObject("ball" + std::to_string(playerNr), sf::Color::Blue, sf::Vector2f(20, 20), sf::Vector2f(0, -100));

	// We add the "fake" ball and player to the objectMap
	AddObjectToMap(otherPlayer);
	AddObjectToMap(ball);
		
	// Adds the player and "fake" player in a vector
	playerObjects.push_back(player);
	playerObjects.push_back(otherPlayer);
}

// Sets the position of the object found with the tag in the packet
void Game::MoveObject(sf::Packet& packet)
{
	std::string objName;
	sf::Vector2f position;
	if (!(packet >> objName >> position))
		return;
	
	GameObject* obj = FindInObjectMap(objName);
	if (obj == nullptr)
		return;

	obj->SetPosition(position);
}

// Sets the color of the object found with the tag in the packet
void Game::ChangeColor(sf::Packet & packet)
{
	std::string objName;
	sf::Color color;

	packet >> objName >> color;

	GameObject* obj = FindInObjectMap(objName);
	if (obj == nullptr)
		return;

	obj->SetColor(color);
}

// Makes the object found with the tag in the packet take damage
void Game::TakeDamage(sf::Packet & packet)
{
	std::string objName;
	int damage;

	packet >> objName >> damage;

	GameObject* obj = FindInObjectMap(objName);
	if (obj == nullptr)
		return;

	if (objName.find("player") != objName.npos)
		dynamic_cast<Player*>(obj)->TakeDamage(damage);
	else 
		obj->TakeDamage(damage);
}

// Tells the player that it won
void Game::WinGame(sf::Packet& packet)
{
	player->SetCanInput(false);
	for (auto it : objectMap)
	{
		GameObject* obj = it.second;
		std::string tag = obj->GetTag();
		if (tag.find("player") != tag.npos || tag.find("ball") != tag.npos)
			obj->SetColor(sf::Color::Black);
	}

	winLoseText.setString("You Won!");
}


void Game::SendWinCommand()
{
	sf::Packet packet;
	packet << "win";
	manager->Send(packet);
}