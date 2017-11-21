#include "Keyboard.h"

bool* Keyboard::keys = 0;

void Keyboard::Initialize()
{
	keys = new bool[sf::Keyboard::KeyCount];
	for (size_t i = 0; i < sf::Keyboard::KeyCount; i++)
	{
		keys[i] = false;
	}
}

void Keyboard::setKeyDown(sf::Keyboard::Key key)
{
	keys[key] = true;
}

void Keyboard::setKeyUp(sf::Keyboard::Key key)
{
	keys[key] = false;
}

bool Keyboard::isKeyDown(sf::Keyboard::Key key)
{
	return keys[key];
}

void Keyboard::clearKeys()
{
	for (size_t i = 0; i < sf::Keyboard::KeyCount; i++)
	{
		keys[i] = false;
	}
}