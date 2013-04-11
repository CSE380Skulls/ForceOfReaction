#pragma once

#include "src\game\Game.h"
#include "src\gsm\physics\Collision.h"

class CollisionListener
{
public:
	virtual void respondToCollision(Game *game, Collision *collision)=0;
};