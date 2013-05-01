#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "stdafx.h"
#include "src\gsm\ai\Bot.h"
#include "src\gsm\sprite\AnimatedSprite.h"
#include "src\game\Game.h"
#include "src\WalkaboutGame.h"
#include "src\gsm\sprite\SpriteDesignations.h"

class FOR_Player : public AnimatedSprite
{
private:
	int attack_Cooldown;
	int cd_Counter;
	int death_Cooldown;
	int selected_element;
	int num_elements;
	int direction;
	bool available_Elements[3];
	void earth_Attack_L(Game *game, float mx, float my);
	void earth_Attack_R(Game *game, float mx, float my);
	void water_Attack_L() {}
	void water_Attack_R() {}
	void fire_Attack_L() {}
	void fire_Attack_R(Game *game, float mx, float my);

	static const int MAX_HP = 30;

	static const int EARTH   =	0;
	static const int WATER   =	1;
	static const int FIRE	 =	2;
	static const int NOTHING =  3;

public:
	FOR_Player(int att_Cooldown, int d_Cooldown, int designation);

	void update(Game *game);
	void updateStatusGUI(Game* game);

	bool canAttack();
	bool canMove() { return !dead && !stunned; }

	void stun(int framesStunned);
	void run();
	void hover();
	void leftAttack(Game* game, float mX, float mY);
	void rightAttack(Game* game, float mX, float mY);
	void jump(Game* game);
	void nextElement();

	int getSelectedElement() { return selected_element; }
	int getDirection() { return direction; }
	bool isMovingV() {return getPhysicsBody()->GetLinearVelocity().y != 0.0f; }
	bool isFloating() { return (getCurrentState() == JUMPING_RIGHT || getCurrentState() == FALLING_RIGHT ||
								getCurrentState() == JUMPING_LEFT || getCurrentState() == FALLING_LEFT); } 
	bool isAttacking() { return (getCurrentState() == ATTACKING_RIGHT || getCurrentState() == ATTACKING_LEFT); }

	void setDirection(int i) { direction = i; }
	void setSelectedElement(int i) { selected_element = i; }
	
	
	void playSound(Game* game, SpriteDesignations soundType);
};