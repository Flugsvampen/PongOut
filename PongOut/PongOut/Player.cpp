#include "Player.h"
#include <thread>


Player::Player()
{
}


Player::~Player()
{
}


void Player::Initialize()
{
	std::thread send(&Send);
	std::thread receive(&Receive);

	send.join();
	receive.join();
}


void Player::Send()
{
	while (true)
	{

	}
}


void Player::Receive()
{
	while (true)
	{

	}
}


void Player::Draw()
{
}
