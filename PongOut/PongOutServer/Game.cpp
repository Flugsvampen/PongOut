#include "Game.h"
#include "Server.h"
#include "Player.h"

#include <iostream>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Time.hpp>

const float TICK_RATE = 1 / 60;

Game::Game() :
	running(true)
{
	GameObject::game = this;

	framePackets.push_back(sf::Packet());
	framePackets.push_back(sf::Packet());
}


Game::~Game()
{
}


void Game::Run()
{
	// Creates a network manager that takes care of receive and send calls
	Server* server = new Server(this);

	sf::Clock clock;
	sf::Time dt;

	// The main loop - ends as soon as the window is closed
	while (true)
	{
		if (clock.getElapsedTime().asSeconds() < TICK_RATE)
			continue;

		dt = clock.restart();

		server->Receive();

		// Draws every drawable objects in the game
		for (auto it : objectMap)
		{
			GameObject* gameObject = it.second;	
			gameObject->Update(dt);
		}

		SendFramePackets(server);
	}
}

sf::Packet & Game::GetFramePacket(const int index)
{
	return framePackets[index];
}

void Game::SendFramePackets(Server* server)
{
	for (int i = 0; i < 2; i++)
	{
		if (framePackets[i].getDataSize() > 0)
		{
			server->Send(framePackets[i], players[i]->GetIP(), players[i]->GetPort());	
			framePackets[i].clear();
		}
	}
}


// Adds the parameter object to the object map and uses its tag as the key value
void Game::AddObjectToMap(GameObject* obj)
{
	objectMap.insert(StringToObjPair(obj->GetTag(), obj));
}