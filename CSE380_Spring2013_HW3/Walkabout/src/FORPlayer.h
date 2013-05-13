#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "stdafx.h"
#include "src\gsm\ai\Bot.h"
#include "src\gsm\sprite\AnimatedSprite.h"
#include "src\game\Game.h"
#include "src\WalkaboutGame.h"
#include "src\gsm\sprite\SpriteDesignations.h"
#include "src\StaticSeed.h"
#include "src\Vine.h"

class FORPlayer : public AnimatedSprite
{
private:
	int attackCooldown;
	int cooldownCounter;
	int deathCooldown;
	int selectedElement;
	int numElements;
	int direction;
	int availableElements;
	void earthAttackL(Game *game, float mx, float my);
	void earthAttackR(Game *game, float mx, float my);
	void waterAttackL(Game *game, float mx, float my);
	void waterAttackR(Game *game, float mx, float my);
	void fireAttackL(Game *game, float mx, float my);
	void fireAttackR(Game *game, float mx, float my);
	// Current Projectile
	AnimatedSprite* projectile;
	// Player  can have both a static seed and projectile out at once, but not two static seeds.
	StaticSeed* staticSeed;
	//list of all vines that are currently within the world
	std::list<completeVineRef> vineList;
	//last vine part collided with
	Vine * lastCollidedVine;
	bool isHookedToVine;
	b2RevoluteJoint * vineJoint;

	static const int EARTH   =	0;
	static const int WATER   =	1;
	static const int FIRE	 =	2;
	static const int NOTHING =  -1;

public:
	FORPlayer(int attCooldown, int dCooldown, int designation);

	void update(Game *game);
	void updateStatusGUI(Game* game);

	bool canAttack();
	bool canMove() { return (hitPoints > 0) && !stunned; }

	void stun(int framesStunned);
	void run();
	void hover();
	void leftAttack(Game* game, float mX, float mY);
	void rightAttack(Game* game, float mX, float mY);
	void jump(Game* game);
	void hookToVine(Game* game);
	void deleteRopeContainingVinePart(Vine *vine);
	void deleteCompleteVine(completeVineRef *vineref);
	void nextElement();

	int getSelectedElement() { return selectedElement; }
	int getDirection() { return direction; }
	bool isMovingV() {return objectJumping; }
	bool isFloating() { return (getCurrentState() == JUMPING_RIGHT || getCurrentState() == FALLING_RIGHT ||
								getCurrentState() == JUMPING_LEFT || getCurrentState() == FALLING_LEFT); } 
	bool isAttacking() { return (getCurrentState() == ATTACKING_RIGHT || getCurrentState() == ATTACKING_LEFT); }

	void setDirection(int i) { direction = i; }
	void setSelectedElement(int i) { selectedElement = i; }
	void setLastCollidedVine(Vine *vine) { lastCollidedVine = vine; }
	
	void playSound(Game* game, SpriteDesignations soundType);
	void destroyProjectile() { projectile = NULL; }
	void createStaticSeed(Game* game, int x, int y);
	void destorySeed() { staticSeed = NULL; }
	void reset();
};