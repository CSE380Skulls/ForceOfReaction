#include "stdafx.h"
#include "src\WalkaboutGame.h"
#include "src\Switch.h"
#include "src\gsm\sprite\SpriteDesignations.h"

void Switch::update(Game *game) {
	// If this switch is already on, do nothing
	if(on) {
		return;
	}
	else {
		// If this switch just got turned on, destroy the target that it controls
		if(hitPoints <= 0) {
			on = true;
			target->setHitPoints(0);
			target->playSound(game, SPRITE_DEAD);
			// Change direction of switch
			if(getCurrentState().compare(IDLE_LEFT) == 0)
				setCurrentState(IDLE_RIGHT);
			else
				setCurrentState(IDLE_LEFT);
		}
	}
}