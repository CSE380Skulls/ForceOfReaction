#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "stdafx.h"
#include "src\gsm\ai\Bot.h"
#include "src\game\Game.h"
#include "src\WalkaboutGame.h"

class Vine : public Bot
{
public:
	void	think(Game *game) {};
	Bot*    clone() { return NULL; }
};