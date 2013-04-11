#pragma once

#include "src\game\Game.h"
#include "src\gsm\physics\CollisionListener.h"
#include "src\gsm\physics\Collision.h"

class WalkaboutCollisionListener : public CollisionListener
{
public:
	void respondToCollision(Game *game, Collision *collision);
};