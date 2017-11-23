#pragma once
#include "GameObject.h"

#include <string>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

class Player : public GameObject
{
public:
	Player(const sf::Vector2f& pos, const std::string& tag);
	~Player();

	void Update(const sf::Time& dt) override;
	class Ball* GetBall() const;

	void SetCanInput(bool can);

private:
	int speed;
	static int playerCount;

	class Ball* ball;
	bool canInput;
	bool hasShot;

	int CheckMoveInput();
	void ClampX();
};

