#pragma once
#include "GameObject.h"

#include <string>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

class Player : public GameObject
{
public:
	Player(const std::string& t, const sf::Color& color, const sf::Vector2f& size, const sf::Vector2f& pos, const int spd);
	~Player();

	void Update() override;

private:
	int speed;

	friend class NetworkManager;
	static class NetworkManager* manager;
};

