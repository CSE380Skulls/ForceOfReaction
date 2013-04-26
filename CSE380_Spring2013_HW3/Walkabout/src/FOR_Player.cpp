
#include "stdafx.h"
#include "src\Seed.h"
#include "src\game\Game.h"
#include "src\gsm\sprite\SpriteManager.h"
#include "src\WalkaboutGame.h"
#include "FOR_Player.h"


// Initialize stuff, att_Cooldown -> The number of frames between player attacks
FOR_Player::FOR_Player(int att_Cooldown, int d_Cooldown){
	dead = false;
	cd_Counter = 0;
	death_Cooldown = d_Cooldown;
	attack_Cooldown = att_Cooldown;
}

// Can this player attack?  Assumes that the player is going to attack if they can
bool FOR_Player::can_Attack(){

	if(cd_Counter <= 0){
		cd_Counter = attack_Cooldown;
		return true;
	}

	return false;
}

void FOR_Player::update(Game* game){
	cd_Counter--;
	if(dead) {
		if(cd_Counter <= 0)
			game->getGSM()->goToGameOver();
		return;
	}
	// If hitpoints are 0, remove it
	if(hitPoints <= 0){
		dead = true;
		cd_Counter = death_Cooldown;
		setCurrentState(DEAD);
		return;
	}
	// Quick fix for winning a level
	if(game->getGSM()->getSpriteManager()->areEnemiesDead())
		game->getGSM()->goToLevelWon();
	
}