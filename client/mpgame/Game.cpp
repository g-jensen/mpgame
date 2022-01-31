#include "Game.h"

State Game::state = State::DEFAULT;
Client Game::client = Client();
Player Game::player2 = Player();

void Game::LoadSingleplayer()
{
	Game::state = State::SINGLEPLAYER;
}

void Game::LoadMultiplayer()
{
	Game::state = State::MULTIPLAYER;
}

bool Game::isInGame()
{
	return Game::state == State::SINGLEPLAYER || Game::state == State::MULTIPLAYER;
}
