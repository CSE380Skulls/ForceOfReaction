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
#include "src\gsm\sprite\SpriteDesignations.h"

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

		if(!n->contact->IsEnabled()) {
			C_Node *temp = n;
			n = n->next;
			removeContact(temp->contact);
			continue;	
		}

		if(a->getHitPoints() <=0 || b->getHitPoints() <= 0){
			n->contact->SetEnabled(false);
		}
		

		// Player is a
		if(a == game->getGSM()->getSpriteManager()->getPlayer()){
			// Not hitting into a wall
			if(!(b->getDesignation() == WALL_DESIGNATION)) {
				// What its hitting isn't dead
				if(b->getHitPoints() > 0) {
					// Hurt me
					a->decrementHitPoints(b->getDamage());
					// If I died, play that I'm dead
					if (a->getHitPoints() <= 0)
						a->playSound(game, SPRITE_DEAD);
					else if(n->firstContact) {
						// apply impulse
							float px = a->getCurrentBodyX();
							float bx = b->getCurrentBodyX();
							// Apply an impulse
							a->stun();
							b->stun();
							if(px > bx){
								//n->contact->SetEnabled(false);
								a->getPhysicsBody()->ApplyLinearImpulse(b2Vec2(60.0f, 15.0f), a->getPhysicsBody()->GetPosition());
								//b->getPhysicsBody()->ApplyLinearImpulse(b2Vec2(-120.0f, 120.0f), b->getPhysicsBody()->GetPosition());
							}
							else {
								//n->contact->SetEnabled(false);
								a->getPhysicsBody()->ApplyLinearImpulse(b2Vec2(-60.0f, 15.0f), a->getPhysicsBody()->GetPosition());
								//b->getPhysicsBody()->ApplyLinearImpulse(b2Vec2(120.0f, 120.0f), b->getPhysicsBody()->GetPosition());
							
							}
							a->playSound(game, SPRITE_HIT);
					}
				}
			}
		}
		else if(b == game->getGSM()->getSpriteManager()->getPlayer()){
			// If not wall
			if(!(a->getDesignation() == WALL_DESIGNATION)) {
				// Other isn't dead
				if(a->getHitPoints() > 0) {
					// Hurt me
					b->decrementHitPoints(a->getDamage());
					// If I died, play sound
					if (b->getHitPoints() <= 0)
						b->playSound(game, SPRITE_DEAD);
					else if(n->firstContact) {
						// apply impulse
							float px = b->getCurrentBodyX();
							float bx = a->getCurrentBodyX();
							a->stun();
							b->stun();
							// Apply an impulse
							if(px > bx){
								//n->contact->SetEnabled(false);
								//a->getPhysicsBody()->ApplyLinearImpulse(b2Vec2(-60.0f, 15.0f), a->getPhysicsBody()->GetPosition());
								b->getPhysicsBody()->ApplyLinearImpulse(b2Vec2(60.0f, 15.0f), b->getPhysicsBody()->GetPosition());
							}
							else {
								//n->contact->SetEnabled(false);
								//a->getPhysicsBody()->ApplyLinearImpulse(b2Vec2(60.0f, 15.0f), a->getPhysicsBody()->GetPosition());
								b->getPhysicsBody()->ApplyLinearImpulse(b2Vec2(-60.0f, 15.0f), b->getPhysicsBody()->GetPosition());
							}


							b->playSound(game, SPRITE_HIT);
					}
				}
			}
		}
		else {
			if(a->getDesignation() == PROJECTILE_DESIGNATION || b->getDesignation() == PROJECTILE_DESIGNATION){
				// Small problem where when adding an object with 0 frames until removal, ends up being 1 frame b/c of ordering.
				if(b->getHitPoints() > 0 && a->getHitPoints() > 0){
					a->decrementHitPoints(b->getDamage());
					b->decrementHitPoints(a->getDamage());
					a->playSound(game, SPRITE_HIT);
					b->playSound(game, SPRITE_HIT);
					if(b->getHitPoints() <= 0 || a->getHitPoints() <= 0){
						if(a->getHitPoints() <= 0)
							a->playSound(game, SPRITE_DEAD);
						if(b->getHitPoints() <= 0)
							b->playSound(game, SPRITE_DEAD);
					}
				}

			}
		}

		n->firstContact = false;

		n = n->next;
	}
}

/*
	Add a contact to contact list.
*/
void BoxPhysics::addContact(b2Contact *contact){
	C_Node *n = new C_Node();
	n->next = contacts.head;
	n->contact = contact;
	n->firstContact = true;

	contacts.head = n;
}

// Remove a contact from contact list
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

// Free all of the memory assoiated with this contact list.
void BoxPhysics::clearContacts(){
	C_Node *n = contacts.head;
	contacts.head = NULL;

	while(n!= NULL){
		C_Node *temp = n;
		n = n->next;
		delete temp;
	}
}