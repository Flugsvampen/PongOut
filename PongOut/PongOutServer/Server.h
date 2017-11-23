#pragma once
#include <unordered_map>
#include <functional>
#include <string>
#include <vector>
#include <thread>
#include <SFML\Network.hpp>

using ServerFunction = std::function<void(sf::Packet, class Player*)>;
using StringFunctionPair = std::pair<std::string, ServerFunction>;
using StringToFunctionMap = std::unordered_map<std::string, ServerFunction>;


class Server
{
	friend class Game;
public:
	Server(class Game* g);
	~Server();

	bool Bind(const std::string& name, ServerFunction func);
	void Receive();
	void Call(const std::string& func);

private:
	sf::UdpSocket socket;
	StringToFunctionMap functionMap;

	std::thread* receive;
	bool running;
	class Game* game;

	class Player* FindPlayer(const sf::IpAddress & ip, const unsigned short port);
	bool FoundInFunctionMap(const std::string& name);
	void Send(sf::Packet& packet, const sf::IpAddress& ip, unsigned short port);
	
	/* functionMap functions */
	void Connect(sf::Packet& packet, class Player* player);
	void MovePlayer(sf::Packet& packet, class Player* player);
	void Shoot(sf::Packet& packet, class Player* player);
};

