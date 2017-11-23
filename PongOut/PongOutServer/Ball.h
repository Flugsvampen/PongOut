#pragma once
#include "GameObject.h"

class Ball : public GameObject
{
public:
	Ball(const sf::Vector2f& pos, class Player* player);
	~Ball();

	void Update(const sf::Time& dt) override;

	void MoveToOwner();
	void Shoot(const float dir);

private:
	void Bounce();
	void OnCollision(const GameObject& other) override;

	float speed;
	sf::Vector2f direction;
	class Player* owner;
	static int ballCount;
};