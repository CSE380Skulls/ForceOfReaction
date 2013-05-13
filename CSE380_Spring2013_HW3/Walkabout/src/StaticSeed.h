#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "stdafx.h"
#include "src\gsm\ai\Bot.h"
#include "src\game\Game.h"
#include "src\WalkaboutGame.h"
#include "src\gsm\sprite\SpriteDesignations.h"
#include "FOR_Projectile.h"
#include "Vine.h"
class Vine;
class StaticSeed : public FOR_Projectile
{
private:
	Vine *attachedVine;
public:
	StaticSeed(int designation) : FOR_Projectile(STATIC_SEED_DESG) 
		{ this->designation = designation; attachedVine = NULL;}

	~StaticSeed(){}
	void	update(Game *game) {this->hitPoints = MAX_HITPOINTS;}
	Bot*    clone() { return NULL; } 
	void	playSound(Game *game, SpriteDesignations soundType) {}
	void	stun(int framesStunned) {}
	void	setAttachedVine(Vine *vine) {attachedVine = vine;}
	Vine *	getAttachedVine() {return attachedVine;}
};