#include "Server.h"
#include "PacketOverloads.h"
#include "Player.h"

#include <iostream>
#include <thread>

const unsigned short SERVER_PORT = 8080;
const unsigned short MAX_AMOUNT_OF_PLAYERS = 2;
const sf::Vector2f SCREEN_SIZE = sf::Vector2f(800, 800);
const sf::Vector2f PLAYER_SIZE = sf::Vector2f(100, 25);

Server::Server()
{
	socket.bind(SERVER_PORT);
}


Server::~Server()
{
}

// This is where the server socket receives data from the client
void Server::Receive()
{
	sf::Packet packet;
	sf::IpAddress senderIP;
	unsigned short senderPort;
	std::string funcName;

	while (true)
	{
		// Receives data from the client
		if (socket.receive(packet, senderIP, senderPort) != sf::Socket::Done)
			return;

		// Checks if we could extract the funcName from packet
		if (!(packet >> funcName))
		{
			std::cerr << "Message from " << senderIP.toString() << " was not valid!" << std::endl;
			continue;
		}

		// Checks if the address and port trying to connect is already in use
		Player* player = FindPlayer(senderIP, senderPort);

		// Checks if the server is not full
		if(players.size() < MAX_AMOUNT_OF_PLAYERS)
		{ 			
			if (player == nullptr)
			{
				// Creates new player
				player = new Player(senderIP, senderPort);
				players.push_back(player);

				// If maximum amount of players is connected
				if (players.size() == 2)
				{
					// Calculates positions for players
					sf::Vector2f pos[2] = { sf::Vector2f(SCREEN_SIZE.x / 2 - PLAYER_SIZE.x * 0.5f,
						SCREEN_SIZE.y - PLAYER_SIZE.y * 1.5f),
						sf::Vector2f(SCREEN_SIZE.x / 2 - PLAYER_SIZE.x * 0.5f,
							PLAYER_SIZE.y * 0.5f) };

					sf::Packet playerPackets[2] = { sf::Packet(), sf::Packet() };

					// Feeds data into the two playerPackets
					for (int i = 0; i < 2; i++)
					{
						int j = 1 - i;
						playerPackets[i] << "initializeGame" << 
							pos[i] << players[i]->GetNr() << 
							pos[j] << players[j]->GetNr();
					}

					// Sends the packets to the players
					for (int i = 0; i < 2; i++)
					{
						Send(playerPackets[i], players[i]->GetIP(), players[i]->GetPort());
					}
				}
				std::cout << "Player from " << player->GetIP().toString() << " connected" << std::endl;
				continue;
			}
		}
		
		// If the server is full and someone new is trying to connect, we won't let them
		if (player == nullptr)
		{
			sf::Packet p;
			p << "message" << "\nThis server is already full!";
			Send(p, senderIP, senderPort);
			
			continue;
		}

		// Sends received data to the other player
		Player* otherPlayer = players[1 - player->GetNr()];
		Send(packet, otherPlayer->GetIP(), otherPlayer->GetPort());
	}
}

// Sends message via our socket
void Server::Send(sf::Packet& packet, const sf::IpAddress& ip, unsigned short port)
{
	socket.send(packet, ip, port);
}

// Tries to find the Player at the defined ip and port
Player* Server::FindPlayer(const sf::IpAddress & ip, const unsigned short port)
{
	for (auto player : players)
	{
		if (ip == player->GetIP() && port == player->GetPort())
			return player;
	}

	return nullptr;
}