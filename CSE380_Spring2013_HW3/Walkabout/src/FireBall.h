#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "stdafx.h"
#include "src\gsm\ai\Bot.h"
#include "src\game\Game.h"
#include "src\WalkaboutGame.h"
#include "src\gsm\sprite\SpriteDesignations.h"
#include "FOR_Projectile.h"

class FireBall : public FOR_Projectile
{
private:
	float vx;
	float vy;
public:
	FireBall(int designation, int vX, int vY): FOR_Projectile(FIREBALL_DESG)
		{ this->designation = designation; vx = vX; vy= vY; }
	void	update(Game *game);
	Bot*    clone() { return NULL; }
	void	init(float px, float py, AnimatedSpriteType *sprite);
	void	playSound(Game *game, SpriteDesignations soundType) {}
	void	stun(int framesStunned) {}
};