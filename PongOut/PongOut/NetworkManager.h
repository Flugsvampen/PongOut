#pragma once
#include <SFML\Network.hpp>
#include <thread>

class NetworkManager
{
public:	
	NetworkManager(class Game* g);
	~NetworkManager();

private:
	friend class Player;

	sf::UdpSocket socket;
	sf::IpAddress serverIP;
	std::thread* receive;

	class Game* game;

	bool running;

	void Receive();
	int Send(sf::Packet& packet, const sf::IpAddress& ip);
	int Send(sf::Packet& packet);
	void Initialize();
};

