#include "Player.h"


Player::Player(const sf::IpAddress& ip, const unsigned short port) :
	m_ip(ip),
	m_port(port)
{
}


Player::~Player()
{
}


const sf::IpAddress& Player::GetIP() const
{
	return m_ip;
}


const unsigned short Player::GetPort() const
{
	return m_port;
}
