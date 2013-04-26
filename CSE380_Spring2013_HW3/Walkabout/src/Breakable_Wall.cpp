/*
	A Seed is thrown by the player to damage bots.
*/

#include "stdafx.h"
#include "src\Breakable_Wall.h"
#include "src\game\Game.h"
#include "src\gsm\sprite\SpriteManager.h"
#include "src\WalkaboutGame.h"

void Breakable_Wall::think(Game *game){
	if(dead)
		return;

	if(hitPoints < (4.0f * WALL_HITPOINTS / 5.0f ))
		setCurrentState(FOUR);
	if(hitPoints < (3.0f * WALL_HITPOINTS / 5.0f ))
		setCurrentState(THREE);
	if(hitPoints < (2.0f * WALL_HITPOINTS / 5.0f) )
		setCurrentState(TWO);
	if(hitPoints < (WALL_HITPOINTS / 5.0f) )
		setCurrentState(ONE);
	// If hitpoints are 0 remove this wall
	if(hitPoints <= 0){
		game->getGSM()->getSpriteManager()->addBotToRemovalList(this, 0);
		dead = true;
	}
}