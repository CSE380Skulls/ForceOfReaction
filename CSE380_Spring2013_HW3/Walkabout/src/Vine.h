#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "stdafx.h"
#include "src\gsm\ai\Bot.h"
#include "src\game\Game.h"
#include "src\WalkaboutGame.h"
#include "src\gsm\sprite\SpriteDesignations.h"
class Vine : public Bot
{
private:
	int vineDestroyed;
public:
	Vine(int designation) { this->designation = designation;  vineDestroyed = false;}
	void	setVineDestroyed(bool destroyed) { vineDestroyed = destroyed; }
	void	update(Game *game);
	Bot*    clone() { return NULL; }
	void	playSound(Game *game, SpriteDesignations soundType) {}
	void    stun(int framesStunned) {}
	void	init(float px, float py, AnimatedSpriteType *sprite);
};

struct completeVineRef{
	std::vector<AnimatedSprite *> vineParts;
};