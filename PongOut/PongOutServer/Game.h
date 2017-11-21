#pragma once
#include <vector>

#include <SFML/Network/Packet.hpp>

class Game
{
	friend class Server;
public:
	Game();
	~Game();

	void Run();

private:
	std::vector<class GameObject*> objectMap;
	std::vector<class Player*> players;
	std::vector<sf::Packet> framePackets;

	void SendFramePackets(class Server* server);
};

