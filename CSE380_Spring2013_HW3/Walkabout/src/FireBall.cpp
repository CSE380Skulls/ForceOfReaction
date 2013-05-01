/*
	A FireBall is shot by player to damage bots.
*/

#include "stdafx.h"
#include "src\FireBall.h"
#include "src\game\Game.h"
#include "src\gsm\sprite\SpriteManager.h"
#include "src\WalkaboutGame.h"

void FireBall::update(Game *game){
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