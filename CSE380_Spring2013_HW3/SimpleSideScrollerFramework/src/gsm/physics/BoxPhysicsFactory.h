/*	
	Author: Richard McKenna

	Physics.h

	This class will be used to manage all game collisions
	and physics. This includes collision detection, and resolution.
	Each frame we will update the collided objects velocities and
	positions accordingly.
*/

#pragma once
#include "stdafx.h"
#include "src\game\Game.h"
#include "src\gsm\sprite\AnimatedSprite.h"
#include "BoxPhysicsObject.h"
#include <Box2D\Box2D.h>

/* For temporary testing */
static const int FRIENDLY_OBJECT_INDEX = -1;
static const int ENEMY_OBJECT_INDEX = -2;
static const int OTHER = -3;

class BoxPhysicsFactory
{
public:
	BoxPhysicsFactory(b2World * worldRef){ physicsWorldRef = worldRef;}
	~BoxPhysicsFactory(){}
	void createPlayerObject(Game *game, AnimatedSprite *sprite);
	void createEnemyObject(Game *game, AnimatedSprite *sprite);
	void createTileObject(Game *game, BoxPhysicsObject *phyobj, 
		float screen_center_x, float screen_center_y, float extent_x, float extent_y);
	void createStaticPlayerObject(Game *game, AnimatedSprite *sprite);

private:
	b2World *physicsWorldRef;

	// Used for tiles
	void createStaticBox(Game *game, BoxPhysicsObject *phyobj, 
		float screen_center_x, float screen_center_y, float extent_x, float extent_y);
	// Used for animated sprites that can move
	void createDynamicBox(Game *game, BoxPhysicsObject *phyobj, AnimatedSprite *obj, int groupIndex,
		float screen_center_x, float screen_center_y, float extent_x, float extent_y);
	// Used for animated sprites that cannot move
	void createStaticBox(Game *game, BoxPhysicsObject *phyobj, AnimatedSprite *obj, int groupIndex, 
		float screen_center_x, float screen_center_y, float extent_x, float extent_y);
	
};