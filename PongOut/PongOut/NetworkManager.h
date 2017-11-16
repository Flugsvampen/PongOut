#pragma once
#include <SFML\Network.hpp>
#include <thread>

class NetworkManager
{
public:	
	NetworkManager();
	~NetworkManager();

private:
	sf::UdpSocket socket;
	sf::IpAddress serverIP;
	std::thread* receive;

	bool running;

	void Receive();
	int Send(sf::Packet& packet, const sf::IpAddress& ip, const unsigned short& port);
	void Initialize();
};

