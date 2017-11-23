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
	tag.append(std::to_string(ballCount++));
	MoveToOwner();
}


Ball::~Ball()
{
}


void Ball::Update(const sf::Time& dt)
{
	Move(direction * speed * dt.asSeconds());
	Bounce();

	if (lastPos == rect.getPosition())
		return;

	for (int i = 0; i < 2; i++)
		game->GetFramePacket(i) << "move" << tag << sf::Vector2f(GetPosition());


	lastPos = rect.getPosition();
}


void Ball::MoveToOwner()
{
	if (owner->GetPosition().y < 400)
		SetPosition(owner->GetPosition().x + owner->GetSize().x / 2 - rect.getSize().x / 2, owner->GetPosition().y + owner->GetSize().y);
	else if (owner->GetPosition().y > 400)
		SetPosition(owner->GetPosition().x + owner->GetSize().x / 2 - rect.getSize().x / 2, owner->GetPosition().y - owner->GetSize().y);
}


void Ball::Shoot(const float dirX)
{
	float dirY;

	if (owner->GetPosition().y < 400)
		dirY = 1;
	else
		dirY = -1;

	direction = sf::Vector2f(dirX, dirY);
}


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


void Ball::OnCollision(const GameObject& other)
{
	if (other.GetTag().find("player") != other.GetTag().npos)
	{
		direction.y *= -1;
	}
}
