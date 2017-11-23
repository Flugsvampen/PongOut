#include "Player.h"
#include "Ball.h"
#include "NetworkManager.h"
#include "Keyboard.h"
#include <iostream>

int Player::playerCount = 0;

std::string PLAYER_TAG = "player";
sf::Color START_COLOR = sf::Color::Green;
sf::Vector2f START_SIZE = sf::Vector2f(100, 25);
float START_SPEED = 400;

Player::Player(const sf::Vector2f& pos) :
	GameObject::GameObject(PLAYER_TAG, START_COLOR, START_SIZE, pos),
	speed(START_SPEED),
	canInput(true),
	hasShot(false)
{
	playerCount++;
	ball = new Ball(pos, this);
}


Player::~Player()
{
}


void Player::Update(const sf::Time& dt)
{
	float direction = CheckMoveInput();
	float movement = direction * dt.asSeconds();
	Move(movement * speed);

	ClampX();
	
	if (!hasShot)
	{
		if (Keyboard::isKeyDown(sf::Keyboard::Space) && canInput)
		{
			ball->Shoot(direction);
			hasShot = true;
		}

		ball->MoveToOwner();
	}
	
	if (lastPos != rect.getPosition())
		SendMoveCommand();
}


Ball* Player::GetBall() const
{
	return ball;
}


void Player::SetCanInput(bool can)
{
	canInput = can;
}


void Player::SetHasShot(bool has)
{
	hasShot = has;
}


int Player::CheckMoveInput()
{
	if (!canInput)
		return 0;

	int input = 0;
	if (Keyboard::isKeyDown(sf::Keyboard::Left))
		input -= 1;
	if (Keyboard::isKeyDown(sf::Keyboard::Right))
		input += 1;

	return input;
}

void Player::ClampX()
{
	if (rect.getPosition().x >= 800 - rect.getSize().x)
	{
		rect.setPosition(800 - rect.getSize().x, rect.getPosition().y);
	}
	else if (rect.getPosition().x <= 0)
	{
		rect.setPosition(0, rect.getPosition().y);
	}
}


