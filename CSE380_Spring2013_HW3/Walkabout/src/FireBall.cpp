/*
	A FireBall is shot by player to damage bots.
*/

#include "stdafx.h"
#include "src\FireBall.h"
#include "src\game\Game.h"
#include "src\gsm\sprite\SpriteManager.h"
#include "src\WalkaboutGame.h"
#include "src\FORPlayer.h"

void FireBall::update(Game *game){
	if(dead)
		return;

	// Update velocity
	getPhysicsBody()->SetLinearVelocity(b2Vec2(vx, vy));

	// If hitpoints are 0 or this seed stopped moving, remove it
	if(hitPoints <= 0){
		game->getGSM()->getSpriteManager()->addBotToRemovalList(this, 0);
		((FORPlayer*)game->getGSM()->getSpriteManager()->getPlayer())->destroyProjectile();
		dead = true;
	}
}

void FireBall::init(float px, float py, AnimatedSpriteType *sprite){
	setHitPoints(1);
	setDamage(FIRE_BALL_DAMAGE);
	setSpriteType(sprite);
	setAlpha(255);
	setCurrentState(IDLE_LEFT);
	PhysicalProperties *fireballProps = getPhysicalProperties();
	pp.setX(px);
	pp.setY(py);
	pp.setVelocity(0.0f, 0.0f);
	pp.setAccelerationX(0);
	pp.setAccelerationY(0);
	setOnTileThisFrame(false);
	setOnTileLastFrame(false);
	affixTightAABBBoundingVolume();
}