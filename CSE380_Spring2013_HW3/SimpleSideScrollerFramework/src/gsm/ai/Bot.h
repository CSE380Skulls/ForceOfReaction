#pragma once

#include "stdafx.h"
#include "src\game\Game.h"
#include "src\gsm\sprite\AnimatedSprite.h"

class Bot : public AnimatedSprite
{
public:

	// Only player uses update method
	virtual void update(Game *game)=0;
	// Clone used for bot recycler
	virtual Bot* clone()=0;
};