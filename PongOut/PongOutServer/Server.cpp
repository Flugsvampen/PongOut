#include "Server.h"
#include "Player.h"
#include "PacketOverloads.h"

#include <iostream>

const unsigned short SERVER_PORT = 8080;
const unsigned short MAX_AMOUNT_OF_PLAYERS = 2;
const sf::Vector2u SCREEN_SIZE = sf::Vector2u(800, 800);

Server::Server()
{
	socket.bind(SERVER_PORT);

	Bind("connect", std::bind(&Server::Connect, this, std::placeholders::_1, std::placeholders::_2));
	Bind("move", std::bind(&Server::MovePlayer, this, std::placeholders::_1, std::placeholders::_2));
}


Server::~Server()
{
}

// Inserts a function at the given name in the functionMap
bool Server::Bind(const std::string& name, ServerFunction func)
{
	// Checks if the key string is already in use
	if (FoundInFunctionMap(name))
	{
		std::cout << "The key name \"" << name << "\" has already been bound to a function" << std::endl;
		return false;
	}

	// Inserts the new name and function in the map
	functionMap.insert(StringFunctionPair(name, func));
	return true;
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
		socket.receive(packet, senderIP, senderPort);

		// Checks if we could extract the funcName from packet
		if (!(packet >> funcName))
		{
			std::cerr << "Message from " << senderIP.toString() << " was not valid!" << std::endl;
			continue;
		}

		// Checks if the address and port trying to connect is already in use
		Player* player = FindPlayer(senderIP, senderPort);
		int otherPlayer = 1 - player->GetNr();
		// Checks if the server is not full
		if(players.size() < MAX_AMOUNT_OF_PLAYERS)
		{ 			
			if (player == nullptr)
			{
				sf::Vector2f size = sf::Vector2f(100, 25);
				
				sf::Vector2f pos = sf::Vector2f(SCREEN_SIZE.x / 2 - size.x * 0.5f, SCREEN_SIZE.y - size.y * 1.5f);
				
				if (players.size() == 1)
					pos.y = size.y * 1.5f;

				// Creates new player
				player = new Player(players.size() - 1, senderIP, senderPort, "player", sf::Color::Green, 
					size, pos, 25);
				players.push_back(player);

				if (players.size() == 2)
				{
					sf::Packet packet;
					packet << "addPlayer" << sf::Vector2f(players[1]->GetPosition());
					Send(packet, players[0]->GetIP(), players[0]->GetPort());

					packet << "addPlayer" << sf::Vector2f(players[0]->GetPosition());
					Send(packet, players[1]->GetIP(), players[1]->GetPort());
				}
			}
		}
		
		// If the server is full and someone new is trying to connect, we won't let them
		if (player == nullptr)
		{
			sf::Packet p;
			p << "message" << "This server is already full!";
			Send(p, senderIP, senderPort);
			continue;
		}

		// Checks if the function exists
		if (!FoundInFunctionMap(funcName))
			continue;
		
		sf::Packet answerPacket = sf::Packet(functionMap[funcName](packet, player));

		// Checks if we need to send any data back to the client
		if(packet.getDataSize() <= 0)
			continue;

		Send(answerPacket, senderIP, senderPort);
	}
}

// Sends message via our socket
void Server::Send(sf::Packet& packet, const sf::IpAddress& ip, unsigned short port)
{
	socket.send(packet, ip, port);
}

// Sends back a connection message to the player with data about the Player class
sf::Packet Server::Connect(sf::Packet& packet, Player* player)
{
	std::cout << "Player from " << player->GetIP().toString() << " connected" << std::endl;

	// Packs all the data the player holds
	sf::Packet p;
	p << "connect" << player->GetTag() << sf::Color(player->GetColor()) <<
		sf::Vector2f(player->GetSize()) << sf::Vector2f(player->GetPosition()) <<
		player->GetSpeed();
	return p;
}

// Moves the player that called the server
sf::Packet Server::MovePlayer(sf::Packet& packet, class Player* player)
{
	int direction;
	packet >> direction;
	player->Move(player->GetSpeed() * direction);
	std::cout << player->GetPosition().x << std::endl;

	packet.clear();
	return packet;
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

// Checks if the key is used in functionMap
bool Server::FoundInFunctionMap(const std::string& name)
{
	return functionMap.find(name) != functionMap.end();
}
