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

class BoxPhysicsFactory
{
public:
	BoxPhysicsFactory(b2World * worldRef){ physicsWorldRef = worldRef;}
	~BoxPhysicsFactory(){}
	void createPlayerObject(Game *game, AnimatedSprite *sprite, bool rotate);
	void createEnemyObject(Game *game, AnimatedSprite *sprite, bool rotate);
	void createTileObject(Game *game, BoxPhysicsObject *phyobj, 
		float screen_center_x, float screen_center_y, float extent_x, float extent_y);
	void createStaticPlayerObject(Game *game, AnimatedSprite *sprite);
	void createFriendlyProjectile(Game *game, AnimatedSprite *sprite, bool rotate);
	void createMainPlayer(Game *game, AnimatedSprite *sprite);
	void createStaticWorldObject(Game *game, AnimatedSprite *sprite);
	void createStaticWorldTrigger(Game *game, AnimatedSprite *sprite, int width, int height);
	// Used for rope objects
	void createStaticRope(Game *game, vector<AnimatedSprite *>spritesArray, float x, float y);
	void createAttackRope(Game * game, vector<AnimatedSprite *>spritesArray, float px, float py, float angle);

private:
	b2World *physicsWorldRef;

	// Used for tiles
	void createStaticBox(Game *game, BoxPhysicsObject *phyobj, 
		float screen_center_x, float screen_center_y, float extent_x, float extent_y);
	// Used for animated sprites that can move
	void createDynamicBox(Game *game, BoxPhysicsObject *phyobj, AnimatedSprite *obj, int groupIndex,
		float screen_center_x, float screen_center_y, float extent_x, float extent_y, bool rotate);
	// Used for animated sprites that cannot move
	void createStaticBox(Game *game, BoxPhysicsObject *phyobj, AnimatedSprite *obj, int groupIndex, 
		float screen_center_x, float screen_center_y, float extent_x, float extent_y);
	
};