#include "Game.h"
#include "Server.h"
#include "Player.h"
#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

Game::Game()
{
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

	// The main loop - ends as soon as the window is closed
	while (true)
	{
		// Draws every drawable objects in the game
		for (auto gameObject : objectMap)
		{
			gameObject->Update();
		}

		//SendFramePackets(server);
	}
}

void Game::SendFramePackets(Server* server)
{
	for (int i = 0; i < players.size(); i++)
	{
		if (framePackets[i].getDataSize() > 0)
		{
			server->Send(framePackets[i], players[i]->GetIP(), players[i]->GetPort());
			framePackets[i].clear();
		}
	}
}
