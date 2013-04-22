#pragma once
#include "stdafx.h"
#include <Box2D\Box2D.h>
#include "src\gsm\physics\BoxPhysics.h"

class BoxContactListener : public b2ContactListener
{
public:
	BoxContactListener(BoxPhysics *physics){ box_physics = physics; }
	~BoxContactListener(){};
	 //b2ContactListener
    // Called when two fixtures begin to touch
    void BeginContact(b2Contact* contact);
    // Called when two fixtures cease to touch
    void EndContact(b2Contact* contact);

	/* The presolve and postsolve functions will be called every time
	  step while two fixtures are overlapping*/

	//This is called after collision detection, but before collision resolution.
	void PreSolve(b2Contact *contact, const b2Manifold *oldManifold);
	//The post solve event is where you can gather collision impulse results.
	void PostSolve(b2Contact *contact, const b2ContactImpulse *impulse);

private:
	BoxPhysics * box_physics;

};