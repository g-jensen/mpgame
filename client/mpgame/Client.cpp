#include "Client.h"

Client::Client()
{
	player = Player();
	moveSpeed = 1000.f;
}

void Client::move(sf::Vector2f delta)
{
	player.position += delta;
}
