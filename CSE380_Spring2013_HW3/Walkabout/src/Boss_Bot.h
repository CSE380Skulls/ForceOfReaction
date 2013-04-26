#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "stdafx.h"
#include "src\gsm\ai\Bot.h"
#include "src\game\Game.h"
#include "src\WalkaboutGame.h"

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
	Boss_Bot(int att_speed, int att_range, int att_dmg, int cool_down) { attack_Speed = att_speed; attack_Range = att_range; attack_Damage = att_dmg; attack_Cool_Down = cool_down; cd_Counter = 0; }
	void	think(Game *game);
	Bot*    clone() { return NULL; }
};