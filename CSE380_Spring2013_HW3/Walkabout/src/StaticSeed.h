#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "stdafx.h"
#include "src\gsm\ai\Bot.h"
#include "src\game\Game.h"
#include "src\WalkaboutGame.h"
#include "src\gsm\sprite\SpriteDesignations.h"

class StaticSeed : public Bot
{
public:
	StaticSeed(int designation) { this->designation = designation; }
	void	update(Game *game) {this->hitPoints = MAX_HITPOINTS;}
	Bot*    clone() { return NULL; } 
	void	playSound(Game *game, SpriteDesignations soundType) {}
	void	stun(int framesStunned) {}
};