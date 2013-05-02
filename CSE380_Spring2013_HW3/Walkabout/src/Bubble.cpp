/*
	A FireBall is shot by player to damage bots.
*/

#include "stdafx.h"
#include "src\Bubble.h"
#include "src\game\Game.h"
#include "src\gsm\sprite\SpriteManager.h"
#include "src\WalkaboutGame.h"

void Bubble::update(Game *game){
	//this->setCurrentBodyAngleVelocity(0);
	if(dead)
		return;

	// If this fireball is no longer moving, remove it from the game
	float vX = getPhysicsBody()->GetLinearVelocity().x * BOX2D_CONVERSION_FACTOR;
	float vY = getPhysicsBody()->GetLinearVelocity().y * BOX2D_CONVERSION_FACTOR;

	// If hitpoints are 0 or this seed stopped moving, remove it
	if( (hitPoints <= 0) || (vY == 0) || (vX == 0) ){
		game->getGSM()->getSpriteManager()->addBotToRemovalList(this, 0);
		dead = true;
	}

	// This vy and vx is a normalized vector towards the goal location
	getPhysicsBody()->SetLinearVelocity(b2Vec2(vx, vy));
}

void Bubble::init(float px, float py, AnimatedSpriteType *sprite){
	setHitPoints(1);
	setDamage(0);
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