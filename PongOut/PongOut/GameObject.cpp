#include "GameObject.h"
#include "NetworkManager.h"

// Is needed to send data from socket
NetworkManager* GameObject::manager = nullptr;

GameObject::GameObject(const std::string& t, const sf::Color& color, const sf::Vector2f& size, const sf::Vector2f& pos) :
	tag(t),
	rect(sf::RectangleShape(size))
{
	rect.setFillColor(color);
	rect.setSize(size);
	rect.setPosition(pos);
	lastPos = rect.getPosition();
}


GameObject::~GameObject()
{
}

// Gets called every frame
void GameObject::Update(const sf::Time& dt)
{
}


bool GameObject::CheckCollision(const GameObject& other)
{
	bool collided = rect.getGlobalBounds().intersects(other.GetShape().getGlobalBounds());

	if (collided)
	{
		OnCollision(other);
	}

	return collided;
}

// Gets called from CheckCollision
void GameObject::OnCollision(const GameObject& other)
{
}


void GameObject::SendMoveCommand()
{
	sf::Packet packet;
	packet << "move" << tag << sf::Vector2f(GetPosition());
	manager->Send(packet);
}


const sf::Vector2f & GameObject::GetSize() const
{
	return rect.getSize();
}


const sf::Vector2f& GameObject::GetPosition() const
{
	return rect.getPosition();
}


const sf::Color & GameObject::GetColor() const
{
	return rect.getFillColor();
}


const sf::RectangleShape & GameObject::GetShape() const
{
	return rect;
}


const std::string & GameObject::GetTag() const
{
	return tag;
}


void GameObject::SetTag(const std::string & newTag)
{
	tag = newTag;
}

void GameObject::SetPosition(const sf::Vector2f & position)
{
	rect.setPosition(position);
}


void GameObject::SetPosition(const float x, const float y)
{
	rect.setPosition(x, y);
}


void GameObject::Move(const sf::Vector2f & movement)
{
	rect.setPosition(rect.getPosition() + movement);
}


void GameObject::Move(float x, float y)
{
	Move(sf::Vector2f(x, y));
}

void GameObject::UpdateLastPosition()
{
	lastPos = rect.getPosition();
}
