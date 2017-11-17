#pragma once
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Network/Packet.hpp>

sf::Packet& operator <<(sf::Packet& packet, sf::Vector2f& vec)
{
	return packet << vec.x << vec.y;
}


sf::Packet& operator >>(sf::Packet& packet, sf::Vector2f& vec)
{
	return packet >> vec.x >> vec.y;
}


sf::Packet& operator <<(sf::Packet& packet, sf::Color& color)
{
	return packet << color.r << color.g << color.b << color.a;
}


sf::Packet& operator >>(sf::Packet& packet, sf::Color& color)
{
	return packet >> color.r >> color.g >> color.b >> color.a;
}