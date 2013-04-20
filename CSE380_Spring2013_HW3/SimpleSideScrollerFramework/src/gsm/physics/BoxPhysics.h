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
#include "BoxPhysicsObject.h"
#include "BoxPhysicsFactory.h"
#include <Box2D\Box2D.h>

class BoxPhysics
{
public:
	
	BoxPhysics();
	~BoxPhysics();
	void update(Game *game);

	//Accessor methods
	BoxPhysicsFactory*		getPhysicsFactory(){ return box_factory;}

private:
	b2World* physics_world;
	BoxPhysicsFactory *box_factory;
	float32 b2StepHertz; // should be 60;
	int32 velocityIterations; //recommended value = 6;
	int32 positionIterations; //recommended value = 2;
	
};