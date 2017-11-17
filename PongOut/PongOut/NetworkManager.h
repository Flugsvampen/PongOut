#pragma once
#include "Game.h"

#include <SFML\Network.hpp>
#include <thread>

class NetworkManager
{
public:	
	NetworkManager(Game& g);
	~NetworkManager();

private:
	friend class Player;

	sf::UdpSocket socket;
	sf::IpAddress serverIP;
	std::thread* receive;

	Game& game;

	bool running;

	void Receive();
	int Send(sf::Packet& packet, const sf::IpAddress& ip);
	int Send(sf::Packet& packet);
	void Initialize();
};

