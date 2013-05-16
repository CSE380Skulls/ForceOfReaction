/*
	A FireBall is shot by player to damage bots.
*/

#include "stdafx.h"
#include "src\Vine.h"
#include "src\game\Game.h"
#include "src\gsm\sprite\SpriteManager.h"
#include "src\WalkaboutGame.h"
#include "src\FORPlayer.h"

void Vine::update(Game *game){
	if(dead)
		return;

	// If hitpoints are 0 or this seed stopped moving, remove it
	if(hitPoints <= 0 && vineDestroyed){
		game->getGSM()->getSpriteManager()->addBotToRemovalList(this, 0);
		((FORPlayer*)game->getGSM()->getSpriteManager()->getPlayer())->destroyProjectile();
		dead = true;
	}
}

void Vine::init(float px, float py, AnimatedSpriteType *sprite){
	setHitPoints(1);
	setDamage(VINE_DAMAGE);
	setSpriteType(sprite);
	setAlpha(255);
	setCurrentState(IDLE_LEFT);
	pp.setX(px);
	pp.setY(py);
	pp.setVelocity(0.0f, 0.0f);
	pp.setAccelerationX(0);
	pp.setAccelerationY(0);
	setOnTileThisFrame(false);
	setOnTileLastFrame(false);
	affixTightAABBBoundingVolume();
}

bool Vine::projectileCollisionCallback(Game *game, AnimatedSprite *collidedObject){
	//part of this vine collided with the player, notify the player
	//casting galore -_- #CrunchTime
	if(collidedObject == game->getGSM()->getSpriteManager()->getPlayer()){
		((FORPlayer *)collidedObject)->setLastCollidedVine(this);
		return false;
	}

	if(collidedObject->getDesignation() == PROJECTILE_DESIGNATION){
		//vine collided with another projectile
		FOR_Projectile * projectile = (FOR_Projectile *)collidedObject;
		if(projectile->getProjectileDesignation() == STATIC_SEED_DESG){
			StaticSeed * seed = (StaticSeed *)projectile;
			if(seed->getAttachedVine() == NULL){
				seed->setAttachedVine(this);
				seedAttached = seed;
				game->getGSM()->getBoxPhysics()->createWorldJoint(this,seed,0.0f,
						this->getSpriteType()->getTextureHeight()/2);
				//create JOINT!! :)
			}
		}
	}

	return true;
}

void Vine::projectileWallCollisionCallback(){
	this->setWallCollision();
	//do nothing here, we will destroy vine parts on our own terms
}