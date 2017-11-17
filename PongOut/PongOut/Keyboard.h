#pragma once

#include <SFML\Window.hpp>

class Keyboard
{
public:
	static void Initialize();

	static void setKeyDown(sf::Keyboard::Key key);
	static bool isKeyDown(sf::Keyboard::Key key);
	static void clearKeys();
private:
	static bool* keys;
};