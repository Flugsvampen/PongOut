#include "Player.h"
#include "Ball.h"
#include "NetworkManager.h"
#include "Keyboard.h"
#include "Game.h"
#include <iostream>

int Player::playerCount = 0;

const std::string PLAYER_TAG = "player";
const sf::Color START_COLOR = sf::Color::Green;
const sf::Vector2f START_SIZE = sf::Vector2f(100, 25);
const float START_SPEED = 400;
const int START_HP = 5;

Player::Player(const sf::Vector2f& pos) :
	GameObject::GameObject(PLAYER_TAG, START_COLOR, START_SIZE, pos),
	speed(START_SPEED),
	canInput(true),
	hasShot(false)
{
	playerCount++;
	ball = new Ball(pos, this);
	hp = START_HP;
}


Player::~Player()
{
}


void Player::Update(const sf::Time& dt)
{
	if (!canInput)
		return;

	// Checks from left/right input
	float direction = CheckMoveInput();
	float movement = direction * dt.asSeconds();

	// Moves the player with the input value
	Move(movement * speed);

	ClampX();
	
	if (!hasShot)
	{
		// Shoots ball if Space is pressed
		if (Keyboard::isKeyDown(sf::Keyboard::Space) && canInput)
		{
			ball->Shoot(direction);
			hasShot = true;
		}

		// Makes the ball follow the player
		ball->MoveToOwner();
	}
	
	// If there was any movement
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


void Player::SetHp(int health)
{
	hp = health;
}


int Player::CheckMoveInput()
{
	int input = 0;
	if (Keyboard::isKeyDown(sf::Keyboard::Left))
		input -= 1;
	if (Keyboard::isKeyDown(sf::Keyboard::Right))
		input += 1;

	return input;
}

// Clamps the player so it stays within the screen
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

// Changes the player's hp and color
void Player::TakeDamage(int damage)
{
	hp -= damage;
	// Sets the players color based on how much hp they have
	sf::Color c = sf::Color((255 / START_HP) * START_HP - hp, (255 / START_HP) * hp, 0, 255);
	rect.setFillColor(c);

	if (hp > 0)
	{
		// Sends the new color value to the other client
		SendColorCommand();
	}
	if (hp <= 0)
	{
		game->SetWonOrLostGame(true);
	}
}


