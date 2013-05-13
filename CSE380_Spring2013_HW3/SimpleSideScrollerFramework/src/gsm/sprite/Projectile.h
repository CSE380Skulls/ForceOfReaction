#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "stdafx.h"
#include "src\gsm\ai\Bot.h"
#include "src\game\Game.h"
#include "src\gsm\state\GameStateManager.h"

class Projectile : public Bot
{
public:
	Projectile() {}
	~Projectile() {}
	virtual void projectileCollisionCallback(Game *game, AnimatedSprite *collidedObject){
		//Destory the projectile by default
		if(collidedObject == game->getGSM()->getSpriteManager()->getPlayer()){
			this->setHitPoints(0);
		}
	}
};
