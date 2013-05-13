/*
	A Seed is thrown by the player to damage bots.
*/

#include "stdafx.h"
#include "src\Seed.h"
#include "src\game\Game.h"
#include "src\gsm\sprite\SpriteManager.h"
#include "src\WalkaboutGame.h"
#include "src\FORPlayer.h"

void Seed::update(Game *game){
	if(dead)
		return;

	// If hitpoints are 0 or this seed stopped moving, remove it
	if(hitPoints <= 0){
		game->getGSM()->getSpriteManager()->addBotToRemovalList(this, 0);
		((FORPlayer*)game->getGSM()->getSpriteManager()->getPlayer())->destroyProjectile();
		// If this seed died because it hit a wall, create a static seed, otherwise just kill it
		if(wallCollision && !enemySeed) {
			// CREATE A SEED THAT STAYS AT THE LOCATION THAT THIS SEED DIED AT.
			float width = getSpriteType()->getTextureWidth();
			float height = getSpriteType()->getTextureHeight();
			float x = game->getGSM()->physicsToScreenX(getPhysicsBody()->GetPosition().x);
			float y = game->getGSM()->physicsToScreenY(getPhysicsBody()->GetPosition().y);
			x-= width / 2.0f;
			y-= height / 2.0f;
			((FORPlayer*)game->getGSM()->getSpriteManager()->getPlayer())->createStaticSeed(game, x, y);
		}
		dead = true;
	}
}

void Seed::init(float px, float py, AnimatedSpriteType *sprite){
	setHitPoints(1);
	setDamage(SEED_DAMAGE);
	setSpriteType(sprite);
	setAlpha(255);
	setCurrentState(IDLE_LEFT);
	PhysicalProperties *seedProps = getPhysicalProperties();
	pp.setX(px);
	pp.setY(py);
	pp.setVelocity(0.0f, 0.0f);
	pp.setAccelerationX(0);
	pp.setAccelerationY(0);
	setOnTileThisFrame(false);
	setOnTileLastFrame(false);
	affixTightAABBBoundingVolume();
}