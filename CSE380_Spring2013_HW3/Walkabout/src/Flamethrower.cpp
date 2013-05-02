/*
	A FireBall is shot by player to damage bots.
*/

#include "stdafx.h"
#include "src\Flamethrower.h"
#include "src\game\Game.h"
#include "src\gsm\sprite\SpriteManager.h"
#include "src\WalkaboutGame.h"

void Flamethrower::update(Game *game){
}

void Flamethrower::init(float px, float py, AnimatedSpriteType *sprite){
	setHitPoints(1);
	setDamage(FLAMETHROWER_DAMAGE);
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