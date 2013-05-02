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
	positionIterations = 4;

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
		// Currently only adding contacts between animated sprites
		AnimatedSprite *a = (AnimatedSprite*)n->contact->GetFixtureA()->GetBody()->GetUserData();
		AnimatedSprite *b = (AnimatedSprite*)n->contact->GetFixtureB()->GetBody()->GetUserData();

		// If this contact isn't enabled, remove it
		if(!n->contact->IsEnabled()) {
			C_Node *temp = n;
			n = n->next;
			removeContact(temp->contact);
			continue;	
		}

		// If one of these two are dead, disable the contact
		if(a->getHitPoints() <=0 || b->getHitPoints() <= 0){
			n->contact->SetEnabled(false);
		}

		// I want a to always be the player in a collision, this cuts out a lot of redundant code
		if(b == game->getGSM()->getSpriteManager()->getPlayer()) {
			AnimatedSprite *temp = a;
			a=  b;
			b = temp;
		}
		
		// Player is sprite A always
		if(a == game->getGSM()->getSpriteManager()->getPlayer()){
			// Not hitting into a wall (this means the breakable wall, not any wall)
			if(!(b->getDesignation() == WALL_DESIGNATION)) {
				// What its hitting isn't dead
				if(b->getHitPoints() > 0) {
					// Hurt me
					a->decrementHitPoints(b->getDamage());
					// If I died, play dead sound
					if (a->getHitPoints() <= 0)
						a->playSound(game, SPRITE_DEAD);
					else if(n->firstContact) {
						// handle the contact
						handlePlayerCollision(game, a, b);
					}
				}
			}
		}
		else {
			if(a->getDesignation() == PROJECTILE_DESIGNATION || b->getDesignation() == PROJECTILE_DESIGNATION){
				// One of the two things involved in this contact is a projectile
				if(b->getHitPoints() > 0 && a->getHitPoints() > 0){
					// Deal damage to the projectile to tell it that it was involved in a collision, also do damage to what it hits
					a->decrementHitPoints(b->getDamage());
					b->decrementHitPoints(a->getDamage());
					// Play sound that two things were hit
					a->playSound(game, SPRITE_HIT);
					b->playSound(game, SPRITE_HIT);
					// If either or both are dead, play dead sound
					if(b->getHitPoints() <= 0 || a->getHitPoints() <= 0){
						if(a->getHitPoints() <= 0)
							a->playSound(game, SPRITE_DEAD);
						if(b->getHitPoints() <= 0)
							b->playSound(game, SPRITE_DEAD);
					}
				}

			}
		}
		// This is no longer the first contact between the two things
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
		}
		aListIt++;
	}
}

void BoxPhysics::handlePlayerCollision(Game *game, AnimatedSprite *player, AnimatedSprite *other){

		float px = game->getGSM()->physicsToScreenX(player->getCurrentBodyX());
		float bx = game->getGSM()->physicsToScreenX(other->getCurrentBodyX());
		float py = game->getGSM()->physicsToScreenY(player->getCurrentBodyY());
		float by = game->getGSM()->physicsToScreenY(other->getCurrentBodyY());

		// Find the bottom of player and top of bot, with a little buffer inbetween
		//float playerBottom = py + (player->getSpriteType()->getTextureHeight() / 1.9f);
		//float botTop = by - (other->getSpriteType()->getTextureHeight() / 1.9f);
	
		// If the player isn't on top of the bot, don't stun and push the player away
		if(py > by) {
			player->stun(8);
			player->setInvincible();
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
			player->setInvincible();
			other->stun(60);
			player->getPhysicsBody()->ApplyLinearImpulse(b2Vec2(player->getPhysicsBody()->GetLinearVelocity().x, 60.0f), player->getPhysicsBody()->GetPosition());
		}
		player->playSound(game, SPRITE_HIT);
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