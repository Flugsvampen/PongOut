#include "NetworkManager.h"

#include <iostream>

const unsigned short SERVER_PORT = 8080;

NetworkManager::NetworkManager() :
	running(true)
{
	socket.bind(sf::UdpSocket::AnyPort);

	Initialize();

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
	while (running)
	{
		sf::Packet packet;
		sf::IpAddress ip;
		unsigned short port;
		std::string message;

		sf::Socket::Status status = socket.receive(packet, ip, port);

		/*if (!(packet >> message) || !FoundInFunctionMap(message))
			continue;

		functionMap[message](packet);*/
	}
}

int NetworkManager::Send(sf::Packet& packet, const sf::IpAddress& ip, const unsigned short& port)
{
	return socket.send(packet, ip, port);
}

void NetworkManager::Initialize()
{
	while (true)
	{
		std::cout << "Write the address of the server you want to connect to: ";
		std::cin >> serverIP;

		sf::Packet packet;
		packet << "connect";

		if (Send(packet, serverIP, SERVER_PORT) == sf::Socket::Done)
		{
			break;
		}
		else
		{
			std::cout << "Connection failed" << std::endl;
		}
	}
}


