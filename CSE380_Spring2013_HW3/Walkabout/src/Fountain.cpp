/*
	A FireBall is shot by player to damage bots.
*/

#include "stdafx.h"
#include "src\Fountain.h"
#include "src\game\Game.h"
#include "src\gsm\sprite\SpriteManager.h"
#include "src\WalkaboutGame.h"
#include "src\FORPlayer.h"

void Fountain::update(Game *game){
	if(dead)
		return;

	// If hitpoints are 0 or this seed stopped moving, remove it
	if(hitPoints <= 0){
		game->getGSM()->getSpriteManager()->addBotToRemovalList(this, 0);
		((FORPlayer*)game->getGSM()->getSpriteManager()->getPlayer())->destroyProjectile();
		dead = true;
	}
}

void Fountain::init(float px, float py, AnimatedSpriteType *sprite, int direction){
	setHitPoints(1);
	setDamage(SEED_DAMAGE);
	setSpriteType(sprite);
	setAlpha(255);
	setCurrentState(direction==1?IDLE_RIGHT:IDLE_LEFT);
	pp.setX(px);
	pp.setY(py);
	pp.setVelocity(0.0f, 0.0f);
	pp.setAccelerationX(0);
	pp.setAccelerationY(0);
	setOnTileThisFrame(false);
	setOnTileLastFrame(false);
	affixTightAABBBoundingVolume();
}