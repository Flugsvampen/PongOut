#include "Player.h"
#include "NetworkManager.h"
#include "Keyboard.h"

NetworkManager* Player::manager = nullptr;

std::string PLAYER_TAG = "player";
sf::Color START_COLOR = sf::Color::Green;
sf::Vector2f START_SIZE = sf::Vector2f(100, 25);
float START_SPEED = 25;

Player::Player(const sf::Vector2f& pos) :
	GameObject::GameObject(PLAYER_TAG, START_COLOR, START_SIZE, pos),
	speed(START_SPEED)
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


