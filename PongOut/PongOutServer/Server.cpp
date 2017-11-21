#include "Server.h"
#include "PacketOverloads.h"
#include "Player.h"
#include "Game.h"

#include <iostream>
#include <thread>

const unsigned short SERVER_PORT = 8080;
const unsigned short MAX_AMOUNT_OF_PLAYERS = 2;
const sf::Vector2u SCREEN_SIZE = sf::Vector2u(800, 800);

Server::Server(Game* g) :
	running(true),
	game(g)
{
	socket.bind(SERVER_PORT);

	Bind("connect", std::bind(&Server::Connect, this, std::placeholders::_1, std::placeholders::_2));
	Bind("move", std::bind(&Server::MovePlayer, this, std::placeholders::_1, std::placeholders::_2));

	receive = new std::thread(&Server::Receive, this);
}


Server::~Server()
{
	running = false;
	receive->join();
	delete receive;
	receive = nullptr;
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

	while (running)
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

		// Checks if the server is not full
		if(game->players.size() < MAX_AMOUNT_OF_PLAYERS)
		{ 			
			if (player == nullptr)
			{
				sf::Vector2f size = sf::Vector2f(100, 25);
				
				sf::Vector2f pos = sf::Vector2f(SCREEN_SIZE.x / 2 - size.x * 0.5f, SCREEN_SIZE.y - size.y * 1.5f);
				
				if (game->players.size() == 1)
					pos.y = size.y * 0.5f;

				// Creates new player
				player = new Player(game->players.size(), senderIP, senderPort, pos);
				game->players.push_back(player);

				if (game->players.size() == 2)
				{
					game->framePackets[0] << "addPlayer" << sf::Vector2f(game->players[1]->GetPosition());

					game->framePackets[1] << "addPlayer" << sf::Vector2f(game->players[1]->GetPosition()) << sf::Vector2f(game->players[0]->GetPosition());
				}
			}
		}
		
		// If the server is full and someone new is trying to connect, we won't let them
		if (player == nullptr)
		{
			sf::Packet p;
			p << "message" << "\nThis server is already full!";
			Send(p, senderIP, senderPort);
		}
		// Checks if the function exists
		else if (FoundInFunctionMap(funcName))
			functionMap[funcName](packet, player);

		game->SendFramePackets(this);
	}
}

// Sends message via our socket
void Server::Send(sf::Packet& packet, const sf::IpAddress& ip, unsigned short port)
{
	socket.send(packet, ip, port);
}

// Sends back a connection message to the player with data about the Player class
void Server::Connect(sf::Packet& packet, Player* player)
{
	std::cout << "Player from " << player->GetIP().toString() << " connected" << std::endl;

	// Packs all the data the player holds
	game->framePackets[player->GetNr()] << "connect" << sf::Vector2f(player->GetPosition());
}

// Moves the player that called the server
void Server::MovePlayer(sf::Packet& packet, Player* player)
{
	float direction;
	packet >> direction;
	player->Move(player->GetSpeed() * direction);

	if (player->GetPosition().x >= SCREEN_SIZE.x - player->GetSize().x)
	{
		player->SetPosition(SCREEN_SIZE.x - player->GetSize().x, player->GetPosition().y);
	}
	else if (player->GetPosition().x <= 0)
	{
		player->SetPosition(0, player->GetPosition().y);
	}

	packet.clear();

	if (game->players.size() < 2)
		return;

	int otherPlayer = 1 - player->GetNr();

	game->framePackets.at(otherPlayer) << "move" << "player2" << sf::Vector2f(player->GetPosition());
}

// Tries to find the Player at the defined ip and port
Player* Server::FindPlayer(const sf::IpAddress & ip, const unsigned short port)
{
	for (auto player : game->players)
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
