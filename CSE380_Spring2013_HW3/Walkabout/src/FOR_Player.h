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
	int selected_element;
	int num_elements;

	static const int MAX_HP = 30;

	static const int EARTH   =	0;
	static const int WATER   =	1;
	static const int FIRE	 =	2;
	static const int NOTHING =  3;

public:
	FOR_Player(int att_Cooldown, int d_Cooldown);
	void update(Game *game);
	bool can_Attack();
	void updateStatusGUI(Game* game);

	bool can_Move() { return !dead; }
	void setSelectedElement(int i) { selected_element = i; }
	int getSelectedElement() {return selected_element; }
};