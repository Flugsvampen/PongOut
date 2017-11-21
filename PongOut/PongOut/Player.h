#pragma once
#include "GameObject.h"

#include <string>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

class Player : public GameObject
{
public:
	Player(const sf::Vector2f& pos);
	~Player();

	void Update(sf::Time dt) override;

private:
	int speed;

	friend class NetworkManager;
	static class NetworkManager* manager;
};

