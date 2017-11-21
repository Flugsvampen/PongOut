#include "Player.h"
#include "NetworkManager.h"
#include "Keyboard.h"
#include <iostream>

NetworkManager* Player::manager = nullptr;

std::string PLAYER_TAG = "player";
sf::Color START_COLOR = sf::Color::Green;
sf::Vector2f START_SIZE = sf::Vector2f(100, 25);
float START_SPEED = 400;

Player::Player(const sf::Vector2f& pos) :
	GameObject::GameObject(PLAYER_TAG, START_COLOR, START_SIZE, pos),
	speed(START_SPEED)
{
}


Player::~Player()
{
}


void Player::Update(sf::Time dt)
{
	int direction = 0;
	if (Keyboard::isKeyDown(sf::Keyboard::Left))
		direction -= 1;
	if (Keyboard::isKeyDown(sf::Keyboard::Right))
		direction += 1;
	Move(direction * speed * dt.asSeconds());

	if (rect.getPosition().x >= 800 - rect.getSize().x)
	{
		rect.setPosition(800 - rect.getSize().x, rect.getPosition().y);
	}
	else if (rect.getPosition().x <= 0)
	{
		rect.setPosition(0, rect.getPosition().y);
	}

	if (direction == 0)
		return;

	sf::Packet packet;
	packet << "move" << direction * dt.asSeconds();
	
	manager->Send(packet);
}


