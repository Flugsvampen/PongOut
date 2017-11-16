#include "Player.h"


Player::Player(const sf::IpAddress& ip, const unsigned short port) :
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
