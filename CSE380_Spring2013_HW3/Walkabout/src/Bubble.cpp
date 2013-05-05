/*
	A FireBall is shot by player to damage bots.
*/

#include "stdafx.h"
#include "src\Bubble.h"
#include "src\game\Game.h"
#include "src\gsm\sprite\SpriteManager.h"
#include "src\WalkaboutGame.h"
#include "src\FORPlayer.h"
#include "src\gui\Cursor.h"

void Bubble::update(Game *game){
	if(dead)
		return;

	float mx = game->getGUI()->getCursor()->getX() + game->getGUI()->getViewport()->getViewportX();
	float my = game->getGUI()->getCursor()->getY() + game->getGUI()->getViewport()->getViewportY();

	float bx = game->getGSM()->physicsToScreenX(getCurrentBodyX());
	float by = game->getGSM()->physicsToScreenY(getCurrentBodyY());

	// Get the difference bewteen these loactions
	float difX = mx - bx;
	float difY = my - by;

	// Get the length of the vector from player to mouse
	float length = std::sqrt((difX * difX) + (difY * difY) );


	// Avoid div by 0
	if(length != 0) {
		// Normalize the distances
		difX /= length;
		difY /= length;

		// Scale distances to be x and y velocity
		difX *= BUBBLE_VELOCITY;
		difY *= BUBBLE_VELOCITY;

		if(abs(mx - bx) < abs(difX)) 
			difX = mx - bx;
	
		if(abs(my - by) < abs(difY))
			difY = my - by;
	
		getPhysicsBody()->SetLinearVelocity(b2Vec2(difX, -difY));
	}

	// If hitpoints are 0 or this seed stopped moving, remove it
	if(hitPoints <= 0){
		game->getGSM()->getSpriteManager()->addBotToRemovalList(this, 0);
		((FORPlayer*)game->getGSM()->getSpriteManager()->getPlayer())->destroyProjectile();
		dead = true;
	}
}

void Bubble::init(float px, float py, AnimatedSpriteType *sprite){
	setHitPoints(1);
	setDamage(SEED_DAMAGE);
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