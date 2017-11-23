#include "Player.h"
#include "Ball.h"

int Player::playerCount = 0;

std::string PLAYER_TAG = "player";
sf::Color START_COLOR = sf::Color::Green;
sf::Vector2f START_SIZE = sf::Vector2f(100, 25);
float START_SPEED = 400;

Player::Player(const int nr, const sf::IpAddress& ip, const unsigned short port, const sf::Vector2f& pos) :
	GameObject::GameObject(PLAYER_TAG, START_COLOR, START_SIZE, pos),
	playerNr(nr),
	speed(START_SPEED),
	ip(ip),
	port(port)
{
	tag.append(std::to_string(playerCount++));
	ball = new Ball(sf::Vector2f(0, 0), this);
	hasShot = false;
}


Player::~Player()
{
}


void Player::Update(const sf::Time& dt)
{
	if (!hasShot)
		ball->MoveToOwner();
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


const sf::Vector2f& Player::GetPosition() const
{
	return rect.getPosition();
}


const sf::Vector2f & Player::GetSize() const
{
	return rect.getSize();
}


Ball* Player::GetBall() const
{
	return ball;
}


void Player::SetPosition(const float x, const float y)
{
	rect.setPosition(x, y);
}


void Player::SetPosition(const sf::Vector2f pos)
{
	rect.setPosition(pos);
}


void Player::Shoot(int direction)
{
	
}
