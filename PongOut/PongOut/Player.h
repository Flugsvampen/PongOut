#pragma once
class Player
{
public:
	Player();
	~Player();

	void Initialize();

private:
	void Send();
	void Receive();
	void Draw();
};

