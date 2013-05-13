#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "stdafx.h"
#include "src\gsm\ai\Bot.h"
#include "src\game\Game.h"
#include "src\WalkaboutGame.h"
#include "src\gsm\sprite\SpriteDesignations.h"
#include "FOR_Projectile.h"

class Seed : public FOR_Projectile
{
private:
	bool enemySeed;
public:
	Seed(int designation, bool enemy) : FOR_Projectile(SEED_DESG) 
		{ this->designation = designation;  enemySeed = enemy;}
	void	update(Game *game);
	Bot*    clone() { return NULL; }
	void	playSound(Game *game, SpriteDesignations soundType) {}
	void	stun(int framesStunned) {}
	void	init(float px, float py, AnimatedSpriteType *sprite);
};