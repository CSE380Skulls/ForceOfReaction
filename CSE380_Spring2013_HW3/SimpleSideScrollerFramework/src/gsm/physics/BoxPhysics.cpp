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
#include "src\audio\GameAudioManager.h"
#include "..\Walkabout\src\WalkaboutGame.h"

/*
	The constructor initializes the data structures and loads
	the necessary ones with recyclable objects for collision tests.
*/
BoxPhysics::BoxPhysics()
{
	//initialize the game world
	contacts.head = NULL;
	b2Vec2 gravity;
	gravity.Set(0.0f, -9.8f);
	physics_world = new b2World(gravity);
	physics_world->SetAllowSleeping(true);

	b2StepHertz = 60.0f;
	velocityIterations = 6;
	positionIterations = 2;

	BoxContactListener *listener = new BoxContactListener(this);
	physics_world->SetContactListener(listener);

	//Now create a factory for this physics system
	box_factory = new BoxPhysicsFactory(physics_world);
}

/*
	The destructor deletes all the recyclable objects.
*/
BoxPhysics::~BoxPhysics()
{
	while(contacts.head != NULL){
		C_Node* temp = contacts.head;
		contacts.head = temp->next;
		delete temp;
	}
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

	updateContacts(game);
}

void BoxPhysics::updateContacts(Game *game){

	C_Node *n = contacts.head;

	while(n != NULL){
		AnimatedSprite *a = (AnimatedSprite*)n->contact->GetFixtureA()->GetBody()->GetUserData();
		AnimatedSprite *b = (AnimatedSprite*)n->contact->GetFixtureB()->GetBody()->GetUserData();

		if(a == game->getGSM()->getSpriteManager()->getPlayer()){
			a->decrementHitPoints(b->getDamage());
			game->getGAM()->playConditional(C_PLAYERHIT);
			if (a->getHitPoints()==0)
				game->getGAM()->playSound(C_DEATH);
		}
		else if(b == game->getGSM()->getSpriteManager()->getPlayer()){
			b->decrementHitPoints(b->getDamage());
			game->getGAM()->playConditional(C_PLAYERHIT);
			if (b->getHitPoints()==0)
				game->getGAM()->playSound(C_DEATH);
		}
		else {
			int aD = a->getDamage();
			int bD = b->getDamage();
			a->decrementHitPoints(b->getDamage());
			b->decrementHitPoints(a->getDamage());
			game->getGAM()->playConditional(C_HIT);
		}

		// Update these contacts
		n = n->next;
	}
}

/*
	WARNING, CAN END UP WITH MULTIPLE CONTACTS IN THE SAME LIST
*/
void BoxPhysics::addContact(b2Contact *contact){
	C_Node *n = new C_Node();
	n->next = contacts.head;
	n->contact = contact;

	contacts.head = n;
}

void BoxPhysics::removeContact(b2Contact *contact){

	if(contacts.head == NULL)
		return;

	C_Node *previous = contacts.head;

	if(previous->contact == contact){
		C_Node *temp = previous;
		contacts.head = temp->next;
		delete temp;
	}
	else {
		C_Node *next = previous->next;

		while(next != NULL){
			if(next->contact == contact){
				C_Node *temp = next;
				previous->next = next->next;
				delete temp;
				return;
			}
			previous = next;
			next = next->next;
		}
	}
}
