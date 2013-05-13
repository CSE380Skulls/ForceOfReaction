#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "stdafx.h"
#include "src\gsm\ai\Bot.h"
#include "src\game\Game.h"
#include "src\WalkaboutGame.h"
#include "src\gsm\sprite\SpriteDesignations.h"
#include "FOR_Projectile.h"

class Fountain : public FOR_Projectile
{
public:
	Fountain(int designation) : FOR_Projectile(FOUNTAIN_DESG)
		{ this->designation = designation;  }
	void	update(Game *game);
	Bot*    clone() { return NULL; }
	void	playSound(Game *game, SpriteDesignations soundType) {}
	void    stun(int framesStunned) {}
	void	init(float px, float py, AnimatedSpriteType *sprite, int direction);
};