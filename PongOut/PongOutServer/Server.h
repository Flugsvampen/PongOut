#pragma once
#include <unordered_map>
#include <functional>
#include <string>
#include <vector>
#include <SFML\Network.hpp>

using ServerFunction = std::function<sf::Packet(sf::Packet)>;
using StringFunctionPair = std::pair<std::string, ServerFunction>;
using StringToFunctionMap = std::unordered_map<std::string, ServerFunction>;


class Server
{
public:
	Server();
	~Server();

	bool Bind(const std::string& name, ServerFunction func);
	void Receive();
	void Call(const std::string& func);

private:
	sf::UdpSocket socket;
	StringToFunctionMap functionMap;
	std::vector<class Player*> players;
	Player* FindPlayer(const sf::IpAddress& ip, const unsigned short port);

	bool FoundInFunctionMap(const std::string& name);
	void Send(sf::Packet& packet, const sf::IpAddress& ip, unsigned short port);
};

