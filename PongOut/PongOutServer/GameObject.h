#pragma once
#include <string>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>

class GameObject
{
public:
	GameObject(const std::string& t, const sf::Color& color, const sf::Vector2f& size, const sf::Vector2f& pos);
	~GameObject();

	virtual bool CheckCollision(sf::RectangleShape other);

	const sf::Vector2f& GetSize() const;
	const sf::Vector2f& GetPosition() const;
	const sf::Color& GetColor() const;
	const sf::RectangleShape& GetShape() const;
	const std::string& GetTag() const;

	void Move(const sf::Vector2f& movement);
	void Move(float x = 0, float y = 0);

	virtual void Update();

protected:
	sf::RectangleShape rect;
	std::string tag;
};



