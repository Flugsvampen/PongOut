#include "Player.h"


Player::Player(const int nr, const sf::IpAddress& ip, const unsigned short port, const std::string& t,
	const sf::Color& color, const sf::Vector2f& size, const sf::Vector2f& pos,
	const int spd) :
	GameObject::GameObject(t, color, size, pos),
	speed(spd),
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
