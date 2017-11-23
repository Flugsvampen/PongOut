#pragma once
#include <unordered_map>
#include <map>
#include <functional>

#include <SFML\Network\Packet.hpp>
#include <SFML\Graphics\Drawable.hpp>

using GameFunction = std::function<void(sf::Packet&)>;
using StringFunctionPair = std::pair<std::string, GameFunction>;
using StringToFunctionMap = std::unordered_map<std::string, GameFunction>;

using StringToObjPair = std::pair<std::string, class GameObject*>;
using StringToObjMap = std::map <std::string, class GameObject*>;

class Game
{
public:
	Game();
	~Game();

	void Run();
	void CallFunction(sf::Packet& packet);

	static sf::Packet framePacket;

private:
	StringToFunctionMap functionMap;
	StringToObjMap objectMap;

	bool Bind(const std::string& name, GameFunction func);
	bool FoundInFunctionMap(const std::string& name);
	GameObject* FindInObjectMap(const std::string& name);
	friend class Player;

	void AddObjectToMap(class GameObject* obj);

	/* functionMap methods */
	void Message(sf::Packet& packet);
	void InitializeGame(sf::Packet& packet);
	void MoveObject(sf::Packet& packet);

	class Player* player;
};

