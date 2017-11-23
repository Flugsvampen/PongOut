#include "Ball.h"
#include "Player.h"
#include "NetworkManager.h"

int Ball::ballCount = 0;

std::string BALL_TAG = "ball";
sf::Color BALL_COLOR = sf::Color::Blue;
sf::Vector2f BALL_SIZE = sf::Vector2f(20, 20);
float BALL_SPEED = 300;

Ball::Ball(const sf::Vector2f& pos, class Player* player) :
	GameObject::GameObject(BALL_TAG, BALL_COLOR, BALL_SIZE, pos),
	speed(BALL_SPEED),
	owner(player),
	direction(sf::Vector2f(0, 0))
{
	//tag.append(std::to_string(ballCount++));
	MoveToOwner();
}


Ball::~Ball()
{
}


void Ball::Update(const sf::Time& dt)
{
	Move(direction * speed * dt.asSeconds());

	// Checks for bouncing on the side walls
	Bounce();

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

}


void Ball::OnCollision(const GameObject& other)
{
	// Flips Y-direction if a player was hit
	if (other.GetTag().find("player") != other.GetTag().npos)
	{
		direction.y *= -1;
	}
}
