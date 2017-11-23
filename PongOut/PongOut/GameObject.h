#pragma once
#include <string>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>

class GameObject
{
public:
	GameObject(const std::string& t, const sf::Color& color, const sf::Vector2f& size, const sf::Vector2f& pos);
	~GameObject();

	virtual void Update(const sf::Time& dt);
	virtual bool CheckCollision(const GameObject& other);
	const sf::Vector2f& GetSize() const;
	const sf::Vector2f& GetPosition() const;
	const sf::Color& GetColor() const;
	const sf::RectangleShape& GetShape() const;
	const std::string& GetTag() const;

	void SetTag(const std::string& newTag);
	void SetPosition(const sf::Vector2f& position);
	void SetPosition(const float x, const float y);
	void Move(const sf::Vector2f& movement);
	void Move(float x = 0, float y = 0);


protected:
	friend class NetworkManager;
	static class NetworkManager* manager;
	virtual void OnCollision(const GameObject& other);
	sf::RectangleShape rect;
	std::string tag;
	sf::Vector2f lastPos;
};

