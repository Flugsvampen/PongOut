#include "GameObject.h"
#include <SFML/Network.hpp>


class Player : public GameObject
{
public:
	Player(const int nr, const sf::IpAddress& ip, const unsigned short port, const std::string& t,
		const sf::Color& color, const sf::Vector2f& size, const sf::Vector2f& pos,
		const int spd);
	~Player();

	const sf::IpAddress& GetIP() const;
	const unsigned short GetPort() const;

	const int GetSpeed() const;
	const int GetNr() const;

private:
	sf::IpAddress ip;
	unsigned short port;
	int playerNr;
	int speed;
};