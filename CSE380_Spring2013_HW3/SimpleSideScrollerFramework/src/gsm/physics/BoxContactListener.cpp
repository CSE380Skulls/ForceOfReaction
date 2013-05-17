#include "stdafx.h"
#include "BoxContactListener.h"
#include "src\gsm\sprite\AnimatedSprite.h"
#include "src\gsm\sprite\Projectile.h"

/*Using a naive implementation to indicate whether or not
		a body is a animated sprite or not. If a body within a condtact
		contains user data then it must be an animated sprite
		static tiles does not create a circular pointer back to
		itself. See example here:
		http://www.iforce2d.net/b2dtut/collision-callbacks */
void BoxContactListener::BeginContact(b2Contact *contact){
	/*Get the sprites that are colliding */
	/* Maybe use something better than the sprite itself such as another
		object that the sprite is, since if we were to have sprite decor
		that is not collidable it would be senseless to give it properties
		like health, etc.
	*/

	// THIS OCCURS WHEN TWO OBJECTS FIRST COLLIDE
 	void * bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
 	void * bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();

	//Simple test to find if a sprite is standing on something
	//by using the sensors under their feet. currently there is only
	//one type of sensor per sprite, and its under them.
	//NOTE: This logic may be expanded or modified to support multiple
	//		sensors in the future
	if(contact->GetFixtureA()->IsSensor()){
		BoxPhysicsObject * object = (BoxPhysicsObject *)(contact->GetFixtureA()->GetUserData());
		object->setIsJumping(false);
	}	

	if(contact->GetFixtureB()->IsSensor()){
		BoxPhysicsObject * object = (BoxPhysicsObject *)(contact->GetFixtureB()->GetUserData());
		object->setIsJumping(false);
	}

	// BOX PHYSICS ONLY USES CONTACTS THAT ARE BETWEEN TWO SPRITES, ADD CONTACT PRUNES OUT NON-SPRITE TO SPRITE CONTACTS
	if(bodyUserDataA || bodyUserDataB){
		box_physics->addContact(contact);
	}
}

void BoxContactListener::EndContact(b2Contact *contact){
	/*Get the sprites that are colliding */
	void * bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
	void * bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();

	// Set the object to jumping. This needs to be done here for accurate determanation
	//of whether or not the player is jumping or not
	if(contact->GetFixtureA()->IsSensor()){
		BoxPhysicsObject * object = (BoxPhysicsObject *)(contact->GetFixtureA()->GetUserData());
		if(object->getPhysicsBody()->GetLinearVelocity().y != 0)
			object->setIsJumping(true);
	}
	if(contact->GetFixtureB()->IsSensor()){
		BoxPhysicsObject * object = (BoxPhysicsObject *)(contact->GetFixtureB()->GetUserData());
		if(object->getPhysicsBody()->GetLinearVelocity().y != 0)
			object->setIsJumping(true);
	}

	if(bodyUserDataA && bodyUserDataB){
		box_physics->removeContact(contact);
	}
}

void BoxContactListener::PreSolve(b2Contact *contact, const b2Manifold *oldManifold){
	// THIS HAPPENS EVERY FRAME AND OCCURS AFTER THE FIRST CALL TO BEGIN CONTACT
	
	void * bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
	void * bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();

	if(bodyUserDataA && bodyUserDataB){
		AnimatedSprite *a = (AnimatedSprite *) bodyUserDataA;
		AnimatedSprite *b = (AnimatedSprite *) bodyUserDataB;
		// If either of the things are dead ignore the contact
		if(b->getHitPoints() <= 0 || a->getHitPoints() <= 0)
			contact->SetEnabled(false);

		if(a->getDesignation() == PROJECTILE_DESIGNATION){
			contact->SetEnabled(((Projectile *)a)->projectilePreCollision(b));
		}

		if(b->getDesignation() == PROJECTILE_DESIGNATION){
			contact->SetEnabled(((Projectile *)b)->projectilePreCollision(a));
		}

	}
}

void BoxContactListener::PostSolve(b2Contact *contact, const b2ContactImpulse *impulse){
}

