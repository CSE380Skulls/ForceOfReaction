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

struct C_Node{
	C_Node *next;
	b2Contact *contact;
};

struct contact_list{
	C_Node *head;
};

class BoxPhysics
{
public:
	BoxPhysics();
	~BoxPhysics();
	void update(Game *game);

	//Accessor methods
	BoxPhysicsFactory*		getPhysicsFactory(){ return box_factory;}
	void addContact(b2Contact *contact);
	void removeContact(b2Contact *contact);

private:
	contact_list contacts;
	b2World* physics_world;
	BoxPhysicsFactory *box_factory;
	float32 b2StepHertz; // should be 60;
	int32 velocityIterations; //recommended value = 6;
	int32 positionIterations; //recommended value = 2;
	void updateContacts(Game *game);
};