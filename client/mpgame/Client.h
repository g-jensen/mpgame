#pragma once

#include "Player.h"

class Client
{
public:
	Player player;
	float moveSpeed;
	Client();
	void move(sf::Vector2f delta);
};

