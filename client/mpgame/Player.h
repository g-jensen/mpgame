#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <string>

typedef struct Trimmed_Player {
	char name[32];
	sf::Vector2f position;
	int id;
} Trimmed_Player;

class Player
{
public:
	char name[32];
	int id;
	sf::Vector2f position;
	Player();
	void drawPlayer(sf::RenderWindow& window);
	sf::Text label;
private:
	sf::RectangleShape rect;
};

