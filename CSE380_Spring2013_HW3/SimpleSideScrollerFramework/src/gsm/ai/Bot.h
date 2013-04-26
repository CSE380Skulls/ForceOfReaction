#pragma once

#include "stdafx.h"
#include "src\game\Game.h"
#include "src\gsm\sprite\AnimatedSprite.h"

class Bot : public AnimatedSprite
{
public:
	Bot()	{}
	~Bot()	{}

	// Only player uses update method
	void update(Game *game) {};
	// TO BE DEFINED BY BOT AI CLASSES
	virtual void think(Game *game)=0;
	virtual Bot* clone()=0;
};