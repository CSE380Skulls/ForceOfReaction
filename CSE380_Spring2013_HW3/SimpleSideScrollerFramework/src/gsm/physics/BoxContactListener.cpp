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

	if(bodyUserDataA && bodyUserDataB){
		AnimatedSprite *spriteA = (AnimatedSprite *) bodyUserDataA;
		AnimatedSprite *spriteB = (AnimatedSprite *) bodyUserDataB;
		// Just testing that this works, will need to differentiate between projectiles, bots, and player
		spriteA->decrementHitPoints(10);
		spriteB->decrementHitPoints(10);



		/*
			If player is colliding with something, hurt the player
		*/

		/*
			If something is colliding with bot, hurt the bot and add whatever hit it to the 
			bot removal list
		*/
	}
	else{
		// colliding with a tile? Right now tiles never have userData.

				/*
			If something that isn't an enemy or player hits a tile, add it to bot removal list
		*/
	}

}

void BoxContactListener::EndContact(b2Contact *contact){
	/*Get the sprites that are colliding */
	void * bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
	void * bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();

	if(bodyUserDataA && bodyUserDataB){
		AnimatedSprite *spriteA = (AnimatedSprite *) bodyUserDataA;
		AnimatedSprite *spriteB = (AnimatedSprite *) bodyUserDataB;

		//Now change the state of the sprites

	}
	else{
		// colliding with a tile? Right now tiles never have userData.
	}
}

void BoxContactListener::PreSolve(b2Contact *contact, const b2Manifold *oldManifold){
	/*If you want to ignore the collision before the defuat impluses
		are applied use this: contact->SetEnabled(false);
		This needs to be set every step for continued effect.
		Also this could be useful for ONE WAY PLATFORMS, so we could
		check the players y value against the platform accordingly.
	*/
}

void BoxContactListener::PostSolve(b2Contact *contact, const b2ContactImpulse *impulse){
	
}

