#include "Server.h"
#include "Player.h"

#include <iostream>

const unsigned short SERVER_PORT = 8080;
const unsigned short MAX_AMOUNT_OF_PLAYERS = 2;

Server::Server()
{
	socket.bind(SERVER_PORT);
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

				/* DO PLAYER SPAWNING STUFF */
				if (funcName == "connect")
				{
					std::cout << "Player from " << senderIP.toString() << " connected" << std::endl;

					sf::Packet packet;
					packet << "connect";

					Send(packet, senderIP, senderPort);
				}
				continue;
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
		
		sf::Packet answerPacket = functionMap[funcName](packet);

		// Checks if we need to send any data back to the client
		if(packet.getDataSize() <= 0)
			continue;

		Send(answerPacket, senderIP, senderPort);
	}
}


void Server::Send(sf::Packet& packet, const sf::IpAddress& ip, unsigned short port)
{
	socket.send(packet, ip, port);
}


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
