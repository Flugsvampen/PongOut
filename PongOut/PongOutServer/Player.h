#include <SFML\Network.hpp>


class Player
{
public:
	Player(const sf::IpAddress& ip, const unsigned short port);
	~Player();

	const sf::IpAddress& GetIP() const;
	const unsigned short GetPort() const;

private:
	sf::IpAddress ip;
	unsigned short port;
};