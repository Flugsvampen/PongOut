#pragma once
#include "GameObject.h"

#include <vector>
#include <unordered_map>
#include <functional>

#include <SFML\Network\Packet.hpp>
#include <SFML\Graphics\Drawable.hpp>

using GameFunction = std::function<void(sf::Packet&)>;
using StringFunctionPair = std::pair<std::string, GameFunction>;
using StringToFunctionMap = std::unordered_map<std::string, GameFunction>;

class Game
{
public:
	Game();
	~Game();

	void Run();
	void CallFunction(sf::Packet& packet);

private:
	StringToFunctionMap functionMap;
	std::vector<GameObject*> gameObjects;

	bool Bind(const std::string& name, GameFunction func);
	bool FoundInFunctionMap(const std::string& name);
	friend class Player;
	void Call(const std::string& func, sf::Packet& packet);

	/* functionMap methods */
	void Connect(sf::Packet& packet);
	void Message(sf::Packet& packet);
	void AddPlayer(sf::Packet& packet);
};

