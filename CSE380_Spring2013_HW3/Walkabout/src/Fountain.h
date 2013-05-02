#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "stdafx.h"
#include "src\gsm\ai\Bot.h"
#include "src\game\Game.h"
#include "src\WalkaboutGame.h"
#include "src\gsm\sprite\SpriteDesignations.h"

class Fountain : public Bot
{
public:
	Fountain(int designation) { this->designation = designation;  }
	void	update(Game *game);
	Bot*    clone() { return NULL; }
	void	playSound(Game *game, SpriteDesignations soundType) {}
	void    stun(int framesStunned) {}
	void	init(float px, float py, AnimatedSpriteType *sprite);
};