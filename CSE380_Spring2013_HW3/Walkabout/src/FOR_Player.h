#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "stdafx.h"
#include "src\gsm\ai\Bot.h"
#include "src\gsm\sprite\AnimatedSprite.h"
#include "src\game\Game.h"
#include "src\WalkaboutGame.h"

class FOR_Player : public AnimatedSprite
{
private:
	int attack_Cooldown;
	int cd_Counter;
	int death_Cooldown;
public:
	FOR_Player(int att_Cooldown, int d_Cooldown);
	void update(Game *game);
	bool can_Attack();
	bool can_Move() { return !dead; }
};