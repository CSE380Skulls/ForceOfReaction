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
#include "src\gsm\sprite\Projectile.h"

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
	positionIterations = 10;

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

	deleteAdjacencyList();

	//destruct the b2world object
	
	//perhaps tell all classes that hold references to box2d objects to remove
	//the references
}

void BoxPhysics::unloadPhysics(){
	deleteAdjacencyList();
	deleteChainList();
	clearContacts();
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
	// I GUARENTEE THAT THE ONLY CONTACTS THAT ARE CONTAINED HERE ARE CONTACTS BETWEEN PLAYERS AND BOTS
	C_Node *n = contacts.head;

	// GO THROUGH ALL OF THE CONTACTS
	while(n != NULL){
		AnimatedSprite *a = (AnimatedSprite*)n->contact->GetFixtureA()->GetBody()->GetUserData();
		AnimatedSprite *b = (AnimatedSprite*)n->contact->GetFixtureB()->GetBody()->GetUserData();

		// I ALWAYS WANT SPRITE A TO BE THE PLAYER
		if(b == game->getGSM()->getSpriteManager()->getPlayer()) {
			AnimatedSprite *temp = a;
			a=  b;
			b = temp;
		}
		
		// I ALWAYS WANT SPRITE B TO BE A PROJECTILE
		if(a->getDesignation() == PROJECTILE_DESIGNATION) {
			AnimatedSprite *temp = a;
			a = b;
			b = temp;
		}

		// NEED TO CHECK IF PLAYER IS BEING HIT BY A PROJECTILE
		if(a == game->getGSM()->getSpriteManager()->getPlayer()) {
			if (b->getDesignation()==CUTSCENE_DESIGNATION) {
				b->setHitPoints(0);
				n->contact->SetEnabled(false);
				game->getGSM()->goToCutscene();
			}
			else if((b->getHitPoints() > 0) 
				|| ((b->getDesignation() == PROJECTILE_DESIGNATION)) 
				|| (b->getDesignation() == SPIKES_DESIGNATION)) 
			{
				if(b->getDesignation() == PROJECTILE_DESIGNATION){
					int groupIndex = b->getPhysicsBody()->GetFixtureList()->GetFilterData().groupIndex;
					((Projectile *)b)->projectileCollisionCallback(game,a);
					if(groupIndex != FRIENDLY_PROJECTILE_INDEX){
						handlePlayerCollision(game, a, b);
					}
				}
				else{
					handlePlayerCollision(game, a, b);
				}
				if(a->getHitPoints() <= 0) {
					n->contact->SetEnabled(false);
				}
			}
			else {
				n->contact->SetEnabled(false);
			}
		}
		else if((a->getDesignation() == PROJECTILE_DESIGNATION) && 
			(b->getDesignation() == PROJECTILE_DESIGNATION))
		{
			// PROJECTILE TO PROJECTILE
			((Projectile *)a)->projectileCollisionCallback(game,b);
			((Projectile *)b)->projectileCollisionCallback(game,a);
			if((a->getHitPoints() <= 0) || (b->getHitPoints() <= 0)) {
				a->playSound(game, SPRITE_DEAD);
			}
			else {
				a->playSound(game, SPRITE_HIT);
			}
		}
		else {
			//maybe use the projectileCollisionCallback here? (If needed) #CrunchTime
			//PROJECTILE TO BOT, PROJECTILE TO WALL
			b->setHitPoints(0);
			a->decrementHitPoints(b->getDamage());
			if(a->getHitPoints() <= 0) {
				a->playSound(game, SPRITE_DEAD);
			}
			else {
				a->playSound(game, SPRITE_HIT);
			}
		}

		// I'M ONLY DEALING WITH THESE CONTACTS ONCE, AFTER DEALING WITH THEM, REMOVE THEM (THIS DOESN'T DISABLE PHYSICS)
		C_Node *temp = n;
		n = n->next;
		removeContact(temp->contact);
	}
}

/*
	Add a contact to contact list.
*/
void BoxPhysics::addContact(b2Contact *contact){
	// I GUARENTEE THAT AT LEAST ONE OF THE BODY USER DATA AREN'T NULL COMMING INTO THIS METHOD
	void * bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
 	void * bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();
	
	// This ensures that if there is a collision with a wall, the non wall will always be bodyUserDataA
	if(!bodyUserDataA) {
		bodyUserDataA = bodyUserDataB;
		bodyUserDataB = NULL;
	}

	// This deals with projectiles hitting walls, I want them to be removed
	if(bodyUserDataA && !bodyUserDataB){
		AnimatedSprite *a = (AnimatedSprite*)bodyUserDataA;
		if(a->getDesignation() == PROJECTILE_DESIGNATION) {
			// Tell this projectile it just hit a wall.
			a->setWallCollision();
			a->setHitPoints(0);
		}
		return;
	}
	/// END PROJECTILE HITTING WALL CHECKS

	// AT THIS POINT, ONLY SPRITE TO SPRITE COLLISIONS REMAIN
	AnimatedSprite *a = (AnimatedSprite*)bodyUserDataA;
	AnimatedSprite *b = (AnimatedSprite*)bodyUserDataB;

	// IF EITHER SPRITE IS DEAD, IGNORE THIS COLLISION
	if(a->getHitPoints() <= 0 || b->getHitPoints() <= 0) {
		contact->SetEnabled(false);
		return;
	}

	// IF EITHER SPRITE IS A PROJECTILE, MAKE SURE THE PROJECTILE DIES, BUT AFTER IT DOES ITS DAMAGE
	if(a->getDesignation() == PROJECTILE_DESIGNATION || b->getDesignation() == PROJECTILE_DESIGNATION) {
		createContact(contact);
		return;
	}
	// END PROJECTILE CHECKS

	// IF ONE OF THE THINGS IN THIS COLLISION IS A WALL, DON'T DO ANYTHING BESIDES APPLY THE PHYSICS
	// NOTE: THE PROJECTILES HAVE BEEN DEALT WITH, ONLY PROJECTILES CAN BREAK WALLS
	if(a->getDesignation() == WALL_DESIGNATION || b->getDesignation() == WALL_DESIGNATION){
		return;
	}

	// AT THIS POINT, ALL THAT REMAINS IS PLAYER TO BOT COLLISIONS
	// I WANT TO FULLY GET RID OF THIS CONTACT LIST, BUT NO WAY OF TELLING WHO PLAYER IS BESIDES UPDATE METHOD
	if(bodyUserDataA && bodyUserDataB) {
		// At this point, both things in collision are animated sprites
		createContact(contact);
	}
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

void BoxPhysics::addEdgeToAdjacency(int id1, int id2, float x1, float y1, float x2, float y2){
	//TODO: I feel 
	BoxVertexStart * start1 =  boxMapAdjacencyList[id1];
	BoxVertexStart * start2 =  boxMapAdjacencyList[id2];
	if(start1 == NULL){
		start1 = new BoxVertexStart();
		start1->x = x1;
		start1->y = y1;
		start1->id = id1;
		start1->head = NULL;
		start1->waschecked = false;
		boxMapAdjacencyList[id1] = start1;
	}
	if(start2 == NULL){
		start2 = new BoxVertexStart();
		start2->x = x2;
		start2->y = y2;
		start2->id = id2;
		start2->head = NULL;
		start2->waschecked = false;
		boxMapAdjacencyList[id2] = start2;
	}
	//add the edge Node to the first indexed node
	if(start1->head == NULL){
		start1->head = new BoxEdgeNode();
		start1->head->id = id2;
		start1->head->nextNode = NULL;
	}
	else{
		BoxEdgeNode * newNode = new BoxEdgeNode();
		newNode->id = id2;
		BoxEdgeNode * formerHead = start1->head;
		start1->head = newNode;
		newNode->nextNode = formerHead;
	}

	//add the edgeNode to the second indexed Node
	if(start2->head == NULL){
		start2->head = new BoxEdgeNode();
		start2->head->id = id1;
		start2->head->nextNode = NULL;
	}
	else{
		BoxEdgeNode * newNode = new BoxEdgeNode();
		newNode->id = id1;
		BoxEdgeNode * formerHead = start2->head;
		start2->head = newNode;
		newNode->nextNode = formerHead;
	}

}

void BoxPhysics::deleteAdjacencyList(){
	map<int,BoxVertexStart*>::iterator aListIt = boxMapAdjacencyList.begin();
	BoxVertexStart* vertex;
	BoxEdgeNode *currentEdgeNode, *prevEdgeNode;
	while(aListIt != boxMapAdjacencyList.end()){
		vertex = aListIt->second;
		currentEdgeNode = vertex->head;
		while(currentEdgeNode != NULL){
			prevEdgeNode = currentEdgeNode;
			currentEdgeNode = currentEdgeNode->nextNode;
			delete prevEdgeNode;
		}
		delete vertex;
		aListIt++;
	}
	boxMapAdjacencyList.clear();
}

void BoxPhysics::createWorldChains(){
	map<int,BoxVertexStart*>::iterator aListIt = boxMapAdjacencyList.begin();
	int count = 0; 
	int currentSize = boxMapAdjacencyList.size();
	while(aListIt != boxMapAdjacencyList.end()){
		BoxVertexStart* vertex = aListIt->second;
		if(vertex->waschecked == false){
			b2ChainShape chain;
			b2Vec2 * vertexList = new b2Vec2[currentSize];
			//Here's the trick here: since this is a graph whose vertices are
			//interconnected in a line fashion we could simply follow the trail
			//throughout the map to connect all of the components. We don't have
			//to worry about graph search recursion, since we are garunteed this
			//type of graph each time.
			bool finished = false;
			int lastCheckedId = vertex->id;
			BoxVertexStart* currentVertex = vertex;
			BoxEdgeNode * edgeNode;
			while(!finished && !currentVertex->waschecked){
				vertexList[count] = b2Vec2(currentVertex->x,currentVertex->y);
				currentVertex->waschecked = true;
				count++;
				edgeNode = currentVertex->head;
				while((edgeNode !=NULL) && (edgeNode->id == lastCheckedId)){
					edgeNode = edgeNode->nextNode;
				}
				if(edgeNode){
					lastCheckedId = currentVertex->id;
					currentVertex = boxMapAdjacencyList[edgeNode->id];
				}
				else
					finished = true;
			}
			if(finished){
				chain.CreateChain(vertexList,count);
			}
			else if(vertex == currentVertex){
				chain.CreateLoop(vertexList,count);
			}
			b2BodyDef bd;
			b2Body* body = physics_world->CreateBody(&bd);
			body->CreateFixture(&chain,1.0f);
			chainList.push_back(body);
			currentSize = boxMapAdjacencyList.size() - count;
			count = 0;
			delete vertexList;
		}
		aListIt++;
	}
}

void BoxPhysics::handlePlayerCollision(Game *game, AnimatedSprite *player, AnimatedSprite *other){

		// DAMAGE THE PLAYER
		player->decrementHitPoints(other->getDamage());

		// If the player died play the player dead sound
		if(player->getHitPoints() <= 0) {
			player->playSound(game, SPRITE_DEAD);
			return;
		}

		// Player isn't dead if this spot it reached, play player hit sound.
		player->playSound(game, SPRITE_HIT);

		// Make sure the player doesn't go flying from hitting a projectile
		if(other->getDesignation() == PROJECTILE_DESIGNATION)
			return;

		float px = game->getGSM()->physicsToScreenX(player->getCurrentBodyX());
		float bx = game->getGSM()->physicsToScreenX(other->getCurrentBodyX());
		float py = game->getGSM()->physicsToScreenY(player->getCurrentBodyY());
		float by = game->getGSM()->physicsToScreenY(other->getCurrentBodyY());
	
		// If the player isn't on top of the bot, don't stun and push the player away
		if(py > by) {
			player->stun(8);
			other->stun(30);
			if(px > bx){
				player->getPhysicsBody()->ApplyLinearImpulse(b2Vec2(50.0f, 10.0f), player->getPhysicsBody()->GetPosition());
				other->getPhysicsBody()->ApplyLinearImpulse(b2Vec2(-50.0f, 10.0f), other->getPhysicsBody()->GetPosition());
			}
			else {
				player->getPhysicsBody()->ApplyLinearImpulse(b2Vec2(-50.0f, 10.0f), player->getPhysicsBody()->GetPosition());
				other->getPhysicsBody()->ApplyLinearImpulse(b2Vec2(50.0f, 10.0f), other->getPhysicsBody()->GetPosition());			
			}
		}
		else {
			// Make the player bounce up
			other->stun(60);
			player->getPhysicsBody()->ApplyLinearImpulse(b2Vec2(player->getPhysicsBody()->GetLinearVelocity().x, 60.0f), player->getPhysicsBody()->GetPosition());
		}
}

void BoxPhysics::iterativeDFS(BoxVertexStart * startNode){

}

/*
	Return the box2d object to box2d and clear the list.
*/
void BoxPhysics::deleteChainList(){
	for(int x = 0; x < chainList.size(); x++){
		chainList.at(x)->GetWorld()->DestroyBody(chainList.at(x));
	}
	chainList.clear();
}

void BoxPhysics::createContact(b2Contact *contact) {
	C_Node *n = new C_Node();
	n->next = contacts.head;
	n->contact = contact;
	contacts.head = n;
}

b2RevoluteJoint * BoxPhysics::createWorldJoint(b2Body * bodyA, b2Body * bodyB){
	b2RevoluteJointDef vineJointDef;
	vineJointDef.collideConnected = true;
	vineJointDef.bodyA = bodyA;
	vineJointDef.bodyB = bodyB;
	//these are static values now for testing, scaling the width and height
	//of the sprite's width and height
	vineJointDef.localAnchorA.Set(0,0);
	vineJointDef.localAnchorB.Set(0,0);
	//revoluteJointDef.referenceAngle = currentBody->GetAngle() - prevBody->GetAngle();
	return (b2RevoluteJoint *)(physics_world->CreateJoint(&vineJointDef));
}

void BoxPhysics::deleteWorldJoint(b2RevoluteJoint *joint){
	physics_world->DestroyJoint(joint);
}