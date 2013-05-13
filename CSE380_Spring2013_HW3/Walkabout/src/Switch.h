#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "stdafx.h"
#include "src\gsm\ai\Bot.h"
#include "src\game\Game.h"
#include "src\WalkaboutGame.h"
#include "src\gsm\sprite\SpriteDesignations.h"

class Switch : public Bot
{
private:
	// Is this switch on?
	bool on;
	// Kill this target when switch is turned off
	AnimatedSprite *target;
public:
	Switch(int designation, AnimatedSprite *targ) { this->designation = designation; on = false; target = targ; }
	void	update(Game *game);
	Bot*    clone() { return NULL; } 
	void	playSound(Game *game, SpriteDesignations soundType) {}
	void	stun(int framesStunned) {}
};