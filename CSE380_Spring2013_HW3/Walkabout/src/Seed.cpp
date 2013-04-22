/*
	A Seed is thrown by the player to damage bots.
*/

#include "stdafx.h"
#include "src\Seed.h"
#include "src\game\Game.h"
#include "src\gsm\sprite\SpriteManager.h"
#include "src\WalkaboutGame.h"

void Seed::think(Game *game){
	// If this seed is no longer moving, remove it from the game
	float vX = getPhysicsBody()->GetLinearVelocity().x * BOX2D_CONVERSION_FACTOR;
	float vY = getPhysicsBody()->GetLinearVelocity().y * BOX2D_CONVERSION_FACTOR;
	if(dead)
		return;
	if( (hitPoints <= 0) || (vX == 0 && vY == 0) ){
		game->getGSM()->getSpriteManager()->addBotToRemovalList(this, 0);
		dead = true;
	}
}