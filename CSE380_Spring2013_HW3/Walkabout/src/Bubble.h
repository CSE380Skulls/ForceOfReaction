#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "stdafx.h"
#include "src\gsm\ai\Bot.h"
#include "src\game\Game.h"
#include "src\WalkaboutGame.h"
#include "src\gsm\sprite\SpriteDesignations.h"

class Bubble : public Bot
{
private:
	float vx;
	float vy;
public:
	Bubble(int designation, int vX, int vY) { this->designation = designation; vx = vX; vy= vY; }
	void	update(Game *game);
	Bot*    clone() { return NULL; }
	void	playSound(Game *game, SpriteDesignations soundType) {}
	void	stun(int framesStunned) {}
	void	init(float px, float py, AnimatedSpriteType *sprite);
};