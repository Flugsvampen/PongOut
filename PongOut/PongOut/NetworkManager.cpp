#include "NetworkManager.h"
#include "Player.h"

#include <iostream>
#include <chrono>

const unsigned short SERVER_PORT = 8080;

NetworkManager::NetworkManager(Game& g) :
	running(true),
	game(g)
{
	socket.bind(sf::UdpSocket::AnyPort);

	Initialize();

	Player::manager = this;

	receive = new std::thread(&NetworkManager::Receive, this);
}


NetworkManager::~NetworkManager()
{
	// send signal
	running = false;
	receive->join();
	delete receive;
	receive = nullptr;
}


void NetworkManager::Receive()
{
	sf::Packet packet;
	sf::IpAddress ip;
	unsigned short port;
	std::string message;

	while (running)
	{
		// Tries to receive a message and checks if it got through
		sf::Socket::Status status = socket.receive(packet, ip, port);
		if (status != sf::Socket::Done)
			continue;

		// Calls the functionMap in the Game class
		game.CallFunction(packet);
	}
}

// Sending message via our UpdSocket
int NetworkManager::Send(sf::Packet& packet, const sf::IpAddress& ip)
{
	return socket.send(packet, ip, SERVER_PORT);
}

// Overload only taking in a packet
int NetworkManager::Send(sf::Packet& packet)
{
	return Send(packet, serverIP);
}

// Initialization for the NetworkManager, asking for a ip to connect to
void NetworkManager::Initialize()
{
	while (true)
	{
		std::cout << "Write the address of the server you want to connect to: ";
		//std::cin >> serverIP;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		serverIP = "127.0.0.1";
		sf::Packet packet;
		packet << "connect";

		if (Send(packet, serverIP) == sf::Socket::Done)
		{
			break;
		}
		else
		{
			std::cout << "Connection failed" << std::endl;
		}
	}
}


