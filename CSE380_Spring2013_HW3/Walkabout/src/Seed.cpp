/*
	A Seed is thrown by the player to damage bots.
*/

#include "stdafx.h"
#include "src\Seed.h"
#include "src\game\Game.h"
#include "src\gsm\sprite\SpriteManager.h"
#include "src\WalkaboutGame.h"

void Seed::update(Game *game){
	if(dead)
		return;
	// If this seed is no longer moving, remove it from the game
	float vX = getPhysicsBody()->GetLinearVelocity().x * BOX2D_CONVERSION_FACTOR;
	float vY = getPhysicsBody()->GetLinearVelocity().y * BOX2D_CONVERSION_FACTOR;

	//int x = getCurrentBodyX() * BOX2D_CONVERSION_FACTOR;
	//int y = getCurrentBodyY() * BOX2D_CONVERSION_FACTOR;
	// If hitpoints are 0 or this seed stopped moving, remove it
	if( (hitPoints <= 0) || (vY == 0) ){
		game->getGSM()->getSpriteManager()->addBotToRemovalList(this, 0);
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