#pragma once

#include "Client.h"

enum class State {
	DEFAULT,
	MAINMENU,
	SINGLEPLAYER,
	MULTIPLAYER
};

class Game
{
public:
	static State state;
	static Client client;
	static Player player2;
	static Player playerlist[10];
	static void LoadSingleplayer();
	static void LoadMultiplayer();
	static bool isInGame();
};

