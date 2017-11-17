#include "Player.h"
#include "NetworkManager.h"
#include "Keyboard.h"

NetworkManager* Player::manager = nullptr;

Player::Player(const std::string& t, const sf::Color& color, const sf::Vector2f& size, const sf::Vector2f& pos, const int spd) :
	GameObject::GameObject(t, color, size, pos),
	speed(spd)
{
}


Player::~Player()
{
}


void Player::Update()
{
	int direction = 0;
	if (Keyboard::isKeyDown(sf::Keyboard::Left))
		direction -= 1;
	if (Keyboard::isKeyDown(sf::Keyboard::Right))
		direction += 1;
	Move(direction * speed);

	Keyboard::clearKeys();

	if (direction == 0)
		return;

	sf::Packet packet;
	packet << "move" << direction;

	manager->Send(packet);
}


