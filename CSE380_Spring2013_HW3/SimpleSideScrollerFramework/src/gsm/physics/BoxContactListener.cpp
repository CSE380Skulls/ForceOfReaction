#include "stdafx.h"
#include "BoxContactListener.h"

/*Using a naive implementation to indicate whether or not
		a body is a animated sprite or not. If a body within a condtact
		contains user data then it must be an animated sprite
		static tiles does not create a circular pointer back to
		itself. See example here:
		http://www.iforce2d.net/b2dtut/collision-callbacks */
void BoxContactListener::BeginContact(b2Contact *contact){
	void * bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
	if (bodyUserData){
		//then this is a sprite

	}

	bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
	if (bodyUserData){
		//then this is the sprite

	}

}

void BoxContactListener::EndContact(b2Contact *contact){
	void * bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
	if (bodyUserData){
		//then this is a sprite

	}

	bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
	if (bodyUserData){
		//then this is the sprite

	}

}