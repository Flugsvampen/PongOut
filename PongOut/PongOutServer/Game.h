#pragma once
#include <vector>
#include <map>
#include <mutex>

#include <SFML/Network/Packet.hpp>

using StringToObjPair = std::pair<std::string, class GameObject*>;
using StringToObjMap = std::map <std::string, class GameObject*>;

class Game
{
	friend class Server;
public:
	Game();
	~Game();

	void Run();
	sf::Packet& GetFramePacket(const int index);
	std::mutex framePacketsMutex;


private:
	bool running;
	StringToObjMap objectMap;
	std::vector<class Player*> players;
	
	std::vector<sf::Packet> framePackets;

	void SendFramePackets(class Server* server);
	void AddObjectToMap(class GameObject* obj);
};

