#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "stdafx.h"
#include "src\gsm\ai\Bot.h"
#include "src\game\Game.h"
#include "src\WalkaboutGame.h"
#include "src\gsm\sprite\SpriteDesignations.h"

class BossBot : public Bot
{
private:
	int attackSpeed;
	int attackRange;
	int attackDamage;
	int attackCooldown;
	int cooldownCounter;
	bool isInBounds(int px);
public:
	BossBot(int attSpeed, int attRange, int attDmg, int cooldown, int designation);
	void	update(Game *game);
	Bot*    clone() { return NULL; }
	void	playSound(Game *game, SpriteDesignations soundType);
	void	stun(int framesStunned) {}
};