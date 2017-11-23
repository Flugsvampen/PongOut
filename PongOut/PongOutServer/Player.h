#include "GameObject.h"
#include <SFML/Network.hpp>


class Player : public GameObject
{
public:
	Player(const int nr, const sf::IpAddress& ip, const unsigned short port, const sf::Vector2f& pos);
	~Player();

	const sf::IpAddress& GetIP() const;
	const unsigned short GetPort() const;

	const int GetSpeed() const;
	const int GetNr() const;
	const sf::Vector2f& GetPosition() const;
	const sf::Vector2f& GetSize() const;
	class Ball* GetBall() const;

	void SetPosition(const float x, const float y);
	void SetPosition(const sf::Vector2f pos);

	void Update(const sf::Time& dt) override;
	void Shoot(int direction);

private:
	sf::IpAddress ip;
	unsigned short port;
	int playerNr;
	int speed;
	class Ball* ball;
	static int playerCount;
	bool hasShot;

};