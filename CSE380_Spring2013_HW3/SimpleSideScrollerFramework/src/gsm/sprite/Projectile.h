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

	/* Callback function for specific needs of each projectile. Returns false
	if the collision should be further ignored for the remainder of the contact*/
	virtual void projectileCollisionCallback(Game *game, AnimatedSprite *collidedObject){
		//Destory the projectile by default
		if(collidedObject == game->getGSM()->getSpriteManager()->getPlayer()){
			this->setHitPoints(0);
		}
	}

	virtual void projectileWallCollisionCallback(){
		this->setWallCollision();
		this->setHitPoints(0);
	}

	virtual bool projectilePreCollision(AnimatedSprite *collidedObject){
		//by default return true;
		return true;
	}
};
