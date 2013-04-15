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
#include "src\gsm\physics\CollidableObject.h"
#include "BoxPhysicsObject.h"
#include <Box2D\Box2D.h>

class BoxPhysics
{
public:
	
	BoxPhysics();
	~BoxPhysics();
	void update(Game *game);
	void createStaticBox(Game *game, BoxPhysicsObject *phyobj, 
		float screen_center_x, float screen_center_y, float extent_x, float extent_y);
	void createDynamicBox(Game *game, BoxPhysicsObject *phyobj, CollidableObject *obj, 
		float screen_center_x, float screen_center_y, float extent_x, float extent_y);

private:
	b2World* physics_world;
	float32 b2StepHertz; // should be 60;
	int32 velocityIterations; //recommended value = 6;
	int32 positionIterations; //recommended value = 2;
	
};