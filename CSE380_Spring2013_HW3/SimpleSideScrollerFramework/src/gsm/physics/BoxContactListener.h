#pragma once
#include "stdafx.h"
#include <Box2D\Box2D.h>

class BoxContactListener : b2ContactListener
{
public:
	BoxContactListener(){};
	~BoxContactListener(){};
	 //b2ContactListener
    // Called when two fixtures begin to touch
    virtual void BeginContact(b2Contact* contact);
    // Called when two fixtures cease to touch
    virtual void EndContact(b2Contact* contact);
};