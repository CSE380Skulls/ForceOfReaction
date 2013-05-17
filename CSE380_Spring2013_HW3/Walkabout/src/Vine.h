#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "stdafx.h"
#include "src\gsm\ai\Bot.h"
#include "src\game\Game.h"
#include "src\WalkaboutGame.h"
#include "src\gsm\sprite\SpriteDesignations.h"
#include "FOR_Projectile.h"
#include "Seed.h"
#include "StaticSeed.h"

class StaticSeed;
class Seed;
class Vine : public FOR_Projectile
{
private:
	int vineDestroyed;
	StaticSeed *seedAttached;
	//b2RevoluteJoint * jointToSeed;
public:
	Vine(int designation) : FOR_Projectile(VINE_DESG) 
		{ this->designation = designation;  vineDestroyed = false; seedAttached = NULL;}
	~Vine(){}
	void	setVineDestroyed(bool destroyed) { vineDestroyed = destroyed; }
	void	update(Game *game);
	Bot*    clone() { return NULL; }
	void	playSound(Game *game, SpriteDesignations soundType) {}
	void    stun(int framesStunned) {}
	void	init(float px, float py, AnimatedSpriteType *sprite);
	void	projectileCollisionCallback(Game *game, AnimatedSprite *collidedObject);
	void	projectileWallCollisionCallback();
	bool	projectilePreCollision(AnimatedSprite *collidedObject);
	void	setAttachedSeed(StaticSeed *seed) {seedAttached = seed;}
	StaticSeed * getStaticSeed() {return seedAttached;}
};

struct completeVineRef{
	std::vector<AnimatedSprite *> vineParts;
};