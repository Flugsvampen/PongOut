#include "Ball.h"
#include "Player.h"
#include "NetworkManager.h"
#include "Game.h"

int Ball::ballCount = 0;

const int OOB_DAMAGE = 1;
const std::string BALL_TAG = "ball";
const sf::Color BALL_COLOR = sf::Color::Blue;
const sf::Vector2f BALL_SIZE = sf::Vector2f(20, 20);
const float BALL_SPEED = 600;

const float MAX_BOUNCE_ANGLE = (180 / 3.14159265359) * 75;

Ball::Ball(const sf::Vector2f& pos, class Player* player) :
	GameObject::GameObject(BALL_TAG, BALL_COLOR, BALL_SIZE, pos),
	speed(BALL_SPEED),
	owner(player),
	direction(sf::Vector2f(0, 0))
{
	//tag.append(std::to_string(ballCount++));
	MoveToOwner();
	startPos = rect.getPosition();
}


Ball::~Ball()
{
}


void Ball::Update(const sf::Time& dt)
{
	Move(direction * speed * dt.asSeconds());

	// Checks for bouncing on the side walls
	Bounce();

	// Checks if the ball went outside the screen
	CheckOOB();

	// If the position was changed we send the position to the other client
	if (lastPos != rect.getPosition())
		SendMoveCommand();
}

// Sets position based on where the owner is
void Ball::MoveToOwner()
{
	// If the player is on the bottom
	if (owner->GetPosition().y < 400)
		SetPosition(owner->GetPosition().x + owner->GetSize().x / 2 - rect.getSize().x / 2, owner->GetPosition().y + owner->GetSize().y);
	// If the player is on the top
	else if (owner->GetPosition().y > 400)
		SetPosition(owner->GetPosition().x + owner->GetSize().x / 2 - rect.getSize().x / 2, owner->GetPosition().y - owner->GetSize().y);
}


void Ball::Shoot(const float dirX)
{
	float dirY;

	// Sets Y-direction based on if the player is on the top or bottom
	if (owner->GetPosition().y < 400)
		dirY = 1;
	else
		dirY = -1;

	// Sets direction so it will begin to move
	direction = sf::Vector2f(dirX, dirY);
}

void Ball::SetDirection(const sf::Vector2f & dir)
{
	direction = dir;
}

// Flips X-direction and sets position to align on the hit wall
void Ball::Bounce()
{
	if (rect.getPosition().x < 0)
	{
		rect.setPosition(0, rect.getPosition().y);
		direction.x *= -1;
	}
	else if (rect.getPosition().x > 800 - rect.getSize().x)
	{
		rect.setPosition(800 - rect.getSize().x, rect.getPosition().y);
		direction.x *= -1;
	}
}


void Ball::CheckOOB()
{
	std::vector<GameObject*> playerObjects = game->GetPlayerObjects();
	for (auto object : playerObjects)
	{
		// If the ball went outside the top or bottom of the screen
		if ((GetPosition().y >= 800 - rect.getSize().y && object->GetPosition().y > 400) ||	
			(GetPosition().y <= 0 && object->GetPosition().y < 400))
		{
			direction = sf::Vector2f(0, 0);

			// Resets the owners hasShot variable
			owner->SetHasShot(false);

			// Checks if the object that has to take damage was the owner of the ball
			if (object->GetTag() == owner->GetTag())
			{
				// The player takes damage
				owner->TakeDamage(OOB_DAMAGE);
			}
			else
			{
				// Sends damage command to other client
				SendDamageCommand(OOB_DAMAGE, object->GetTag());
			}

			// Moves the ball back to its owner
			if(owner->GetHp() > 0)
				MoveToOwner();
		}
	}
}


void Ball::OnCollision(const GameObject& other)
{
	// Flips Y-direction if a player was hit
	if (other.GetTag().find("player") != other.GetTag().npos)
	{
		// Saves some variables from the ball and the other object
		float ballWidth = rect.getSize().x;
		float ballCenterX = GetPosition().x + ballWidth / 2;
		float otherWidth = other.GetSize().x;
		float otherCenterX = other.GetPosition().x + otherWidth / 2;

		// Length of direction vector
		float directionLength = sqrt(pow(direction.x, 2) + pow(direction.y, 2));

		// The balls' position relative to the middle of the object it's colliding with
		// Normalized between (-1, 1)
		float posX = (ballCenterX - otherCenterX) / (otherWidth / 2);

		// The new x-direction is calculated with the relative posX
		direction.x = directionLength * posX;

		direction.y *= -1;

		speed *= 1.05f;
	}
}