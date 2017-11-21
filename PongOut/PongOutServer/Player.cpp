#include "Player.h"


std::string PLAYER_TAG = "player";
sf::Color START_COLOR = sf::Color::Green;
sf::Vector2f START_SIZE = sf::Vector2f(100, 25);
float START_SPEED = 25;

Player::Player(const int nr, const sf::IpAddress& ip, const unsigned short port, const sf::Vector2f& pos) :
	GameObject::GameObject(PLAYER_TAG, START_COLOR, START_SIZE, pos),
	playerNr(nr),
	speed(START_SPEED),
	ip(ip),
	port(port)
{

}


Player::~Player()
{
}


const sf::IpAddress& Player::GetIP() const
{
	return ip;
}


const unsigned short Player::GetPort() const
{
	return port;
}

const int Player::GetSpeed() const
{
	return speed;
}


const int Player::GetNr() const
{
	return playerNr;
}
