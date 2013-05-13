/*
	A Seed is thrown by the player to damage bots.
*/

#include "stdafx.h"
#include "src\SwitchWall.h"
#include "src\game\Game.h"
#include "src\gsm\sprite\SpriteManager.h"
#include "src\WalkaboutGame.h"
#include "src\audio\GameAudioManager.h"

void SwitchWall::update(Game *game){
	if(dead)
		return;
	// If hitpoints are 0 remove this wall
	if(hitPoints <= 0){
		game->getGSM()->getSpriteManager()->addBotToRemovalList(this, 0);
		dead = true;
	}
	else 
		hitPoints = MAX_HITPOINTS;
}

void SwitchWall::playSound(Game *game, SpriteDesignations soundType) {
	if(soundType == SPRITE_DEAD && !dead){
		game->getGAM()->playSound(C_EXPLOSION);
	}
}