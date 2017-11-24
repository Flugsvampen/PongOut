#pragma once
#include <unordered_map>
#include <functional>
#include <string>
#include <vector>
#include <thread>
#include <SFML\Network.hpp>


class Server
{
	friend class Game;
public:
	Server();
	~Server();

	void Receive();

private:
	sf::UdpSocket socket;

	std::vector<class Player*> players;
	class Player* FindPlayer(const sf::IpAddress & ip, const unsigned short port);
	void Send(sf::Packet& packet, const sf::IpAddress& ip, unsigned short port);

	void SendMessage(const std::string& message, const sf::IpAddress& ip, unsigned short port);
	class Player* AddPlayer(const sf::IpAddress& ip, unsigned short port);
	void InitializeGame(const std::string& command = "initializeGame");
};

