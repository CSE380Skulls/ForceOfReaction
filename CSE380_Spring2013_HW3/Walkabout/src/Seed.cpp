/*
	A Seed is thrown by the player to damage bots.
*/

#include "stdafx.h"
#include "src\Seed.h"
#include "src\game\Game.h"
#include "src\gsm\sprite\SpriteManager.h"

void Seed::think(Game *game){
	if(dead)
		return;
	if(this->hitPoints <= 0){
		game->getGSM()->getSpriteManager()->addBotToRemovalList(this, 0);
		dead = true;
	}
}