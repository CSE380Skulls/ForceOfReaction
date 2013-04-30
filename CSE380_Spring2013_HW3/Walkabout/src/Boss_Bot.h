#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "stdafx.h"
#include "src\gsm\ai\Bot.h"
#include "src\game\Game.h"
#include "src\WalkaboutGame.h"
#include "src\gsm\sprite\SpriteDesignations.h"

class Boss_Bot : public Bot
{
private:
	int attack_Speed;
	int attack_Range;
	int attack_Damage;
	int attack_Cool_Down;
	int cd_Counter;
	bool isInBounds(int px);
public:
	Boss_Bot(int att_speed, int att_range, int att_dmg, int cool_down, int designation);
	void	update(Game *game);
	Bot*    clone() { return NULL; }
	void	playSound(Game *game, SpriteDesignations soundType);
	void	stun() {}
};