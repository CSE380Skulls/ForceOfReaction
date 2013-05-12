#include "stdafx.h"
#include "BoxContactListener.h"
#include "src\gsm\sprite\AnimatedSprite.h"

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

	if(bodyUserDataA || bodyUserDataB){
		if(contact->IsEnabled())
			box_physics->addContact(contact);
	}
}

void BoxContactListener::EndContact(b2Contact *contact){
	/*Get the sprites that are colliding */
	void * bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
	void * bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();

	//See comments in addContact(...) for more info on this logic. Set
	//the object to jumping. This needs to be done here for accurate determanation
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
	/*If you want to ignore the collision before the defuat impluses
		are applied use this: contact->SetEnabled(false);
		This needs to be set every step for continued effect.
		Also this could be useful for ONE WAY PLATFORMS, so we could
		check the players y value against the platform accordingly.
	*/
	
	void * bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
	void * bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();

	if(bodyUserDataA && bodyUserDataB){
		AnimatedSprite *a = (AnimatedSprite *) bodyUserDataA;
		AnimatedSprite *b = (AnimatedSprite *) bodyUserDataB;

		if(b->getHitPoints() <= 0 || a->getHitPoints() <= 0 || a->isSpriteInvincible() || b->isSpriteInvincible())
			contact->SetEnabled(false);
	}
}

void BoxContactListener::PostSolve(b2Contact *contact, const b2ContactImpulse *impulse){
	
}

