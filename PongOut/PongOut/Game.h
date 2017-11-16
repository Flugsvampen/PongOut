#pragma once
#include <vector>
#include <unordered_map>
#include <functional>

#include <SFML\Network\Packet.hpp>
#include <SFML\Graphics\Drawable.hpp>

using GameFunction = std::function<void(sf::Packet)>;
using StringFunctionPair = std::pair<std::string, GameFunction>;
using StringToFunctionMap = std::unordered_map<std::string, GameFunction>;

class Game
{
public:
	Game();
	~Game();

	void Run();
	static std::vector<sf::Drawable*> drawables;

private:
	static const unsigned short serverPort;
	StringToFunctionMap functionMap;

	bool Bind(const std::string& name, GameFunction func);

	void Call(const std::string& func, sf::Packet packet);
	bool FoundInFunctionMap(const std::string& name);

	void Connect(sf::Packet packet);
};

