#include "GameObject.h"


GameObject::GameObject(const std::string& t, const sf::Color& color, const sf::Vector2f& size, const sf::Vector2f& pos) :
	tag(t),
	rect(sf::RectangleShape(size))
{
	rect.setFillColor(color);
	rect.setSize(size);
	rect.setPosition(pos);
}


GameObject::~GameObject()
{
}


bool GameObject::CheckCollision(sf::RectangleShape other)
{
	return rect.getGlobalBounds().intersects(other.getGlobalBounds());
}


const sf::Vector2f & GameObject::GetSize() const
{
	return rect.getSize();
}


const sf::Vector2f & GameObject::GetPosition() const
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


void GameObject::Move(const sf::Vector2f & movement)
{
	rect.setPosition(rect.getPosition() + movement);
}


void GameObject::Move(float x, float y)
{
	Move(sf::Vector2f(x, y));
}


void GameObject::Update()
{
}
