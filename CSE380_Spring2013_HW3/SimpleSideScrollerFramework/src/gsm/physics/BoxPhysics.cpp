/*
	BoxPhysics.cpp

	See BoxPhysics.h for a class description.
*/

#include "stdafx.h"
//need box2d references here
#include "src\game\Game.h"
#include "src\gsm\physics\BoxPhysics.h"
#include "BoxContactListener.h"
#include "src\gsm\state\GameStateManager.h"

/*
	The constructor initializes the data structures and loads
	the necessary ones with recyclable objects for collision tests.
*/
BoxPhysics::BoxPhysics()
{
	//initialize the game world
	b2Vec2 gravity;
	gravity.Set(0.0f, -9.8f);
	physics_world = new b2World(gravity);
	physics_world->SetAllowSleeping(true);

	b2StepHertz = 60.0f;
	velocityIterations = 6;
	positionIterations = 2;

	BoxContactListener *listener = new BoxContactListener();
	physics_world->SetContactListener(listener);

	//Now create a factory for this physics system
	box_factory = new BoxPhysicsFactory(physics_world);
}

/*
	The destructor deletes all the recyclable objects.
*/
BoxPhysics::~BoxPhysics()
{
	//destruct the b2world object
	
	//perhaps tell all classes that hold references to box2d objects to remove
	//the references
}


/*
	This is where all game physics starts each frame. It is called each frame 
	by the game statem manager after player input and AI have been processed. It
	updates the physical state of all dynamic objects in the game and
	moves all objects to their end of frame positions, updates all necessary
	object velocities, and calls all collision event handlers.
*/
void BoxPhysics::update(Game *game)
{
	//Only call the box2d step for now
	physics_world->Step( 1.0f / b2StepHertz , velocityIterations, positionIterations);
}
