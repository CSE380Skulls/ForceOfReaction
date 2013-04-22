#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "stdafx.h"
#include "src\gsm\ai\Bot.h"
#include "src\game\Game.h"

class Seed : public Bot
{
public:
	void	think(Game *game);
	Bot*    clone() { return NULL; }
};