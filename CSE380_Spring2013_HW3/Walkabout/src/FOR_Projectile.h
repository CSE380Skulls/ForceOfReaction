#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "stdafx.h"
#include "src\gsm\ai\Bot.h"
#include "src\game\Game.h"
#include "src\gsm\state\GameStateManager.h"
#include "src\WalkaboutGame.h"
#include "src\gsm\sprite\Projectile.h"

//ENUM to specifiy the type of
enum ProjectileDesignation {
	SEED_DESG = 0, STATIC_SEED_DESG, VINE_DESG, BUBBLE_DESG, FOUNTAIN_DESG, FIREBALL_DESG, FLAMETHROWER_DESG
};

class FOR_Projectile : public Projectile
{
private:
	ProjectileDesignation projectileDesignation;
public:
	FOR_Projectile(ProjectileDesignation p_designation) { this->projectileDesignation = p_designation;}
	ProjectileDesignation getProjectileDesignation() {return projectileDesignation;}
};
