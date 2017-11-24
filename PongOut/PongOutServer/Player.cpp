#include "Player.h"

int Player::playerCount = 0;

Player::Player(const sf::IpAddress& ip, const unsigned short port) :
	ip(ip),
	port(port),
	isReady(false)
{
	playerNr = playerCount++;
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


const int Player::GetNr() const
{
	return playerNr;
}


const bool Player::IsReady() const
{
	return isReady;
}


void Player::SetReady(bool ready)
{
	isReady = ready;
}
