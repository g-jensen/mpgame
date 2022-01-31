#include "Player.h"

Player::Player()
{
	id = 0;
	memset(name, '\0', 32);
	rect = sf::RectangleShape();
	rect.setPosition(0, 0);
	rect.setSize({20,20});

	label = sf::Text();
}

void Player::drawPlayer(sf::RenderWindow& window)
{
	rect.setPosition(position);
	label.setString(name);
	label.setPosition(position + sf::Vector2f(-20, -35));
	window.draw(rect);
	window.draw(label);
}