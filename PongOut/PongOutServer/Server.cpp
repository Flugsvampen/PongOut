#include "Server.h"
#include "PacketOverloads.h"
#include "Player.h"

#include <iostream>
#include <thread>
#include <algorithm>

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
				player = AddPlayer(senderIP, senderPort);
				continue;
			}
		}
		
		// If the server is full and someone new is trying to connect, we won't let them
		if (player == nullptr)
		{
			SendMessage("\nThis server is already full!", senderIP, senderPort);
			continue;
		}
		else if (funcName == "ready")
		{
			player->SetReady(true);

			// If both players are ready
			if (players.size() >= 2 &&
				players[0]->IsReady() && players[1]->IsReady())
			{
				players[0]->SetReady(false);
				players[1]->SetReady(false);
				InitializeGame("restart");
			}
			continue;
		}
		else if (funcName == "disconnect")
		{ 
			// Removes the player (disconnects it)
			auto it = std::find(players.begin(), players.end(), player);
			players.erase(it);
			delete player;
			std::cout << "Player from " << player->GetIP().toString() << " disconnected" << std::endl;
			// If there are still players connected we tell them the game is over
			if (players.size() > 0)
			{
				player = players[0];
				sf::Packet p;
				p << "walkover";
				Send(p, player->GetIP(), player->GetPort());
			}
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


void Server::SendMessage(const std::string & message, const sf::IpAddress & ip, unsigned short port)
{
	sf::Packet p;
	p << "message" << message;
	Send(p, ip, port);
}


Player * Server::AddPlayer(const sf::IpAddress& ip, unsigned short port)
{
	// Creates new player
	Player* player = new Player(ip, port);
	players.push_back(player);

	// If maximum amount of players is connected
	if (players.size() == 2)
	{
		InitializeGame();
	}
	std::cout << "Player from " << player->GetIP().toString() << " connected" << std::endl;
	return player;
}


void Server::InitializeGame(const std::string& command)
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
		playerPackets[i] << command <<
			pos[i] << players[i]->GetNr() <<
			pos[j] << players[j]->GetNr();
	}

	// Sends the packets to the players
	for (int i = 0; i < 2; i++)
	{
		Send(playerPackets[i], players[i]->GetIP(), players[i]->GetPort());
	}
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