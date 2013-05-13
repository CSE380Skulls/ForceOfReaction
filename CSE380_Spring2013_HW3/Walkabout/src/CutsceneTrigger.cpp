/*
	A Seed is thrown by the player to damage bots.
*/

#include "stdafx.h"
#include "src\CutsceneTrigger.h"
#include "src\game\Game.h"
#include "src\gsm\sprite\SpriteManager.h"
#include "src\WalkaboutGame.h"
#include "src\audio\GameAudioManager.h"

void CutsceneTrigger::update(Game *game){
	if(dead)
		return;
	// If hitpoints are 0 remove this cutscene
	if(hitPoints <= 0){
		game->getGSM()->getSpriteManager()->addBotToRemovalList(this, 0);
		dead = true;
	}
}

void CutsceneTrigger::playSound(Game *game, SpriteDesignations soundType) {
	if(soundType == SPRITE_DEAD && !dead){
		game->getGAM()->playSound(C_SWOOSHTALK);
	}
}