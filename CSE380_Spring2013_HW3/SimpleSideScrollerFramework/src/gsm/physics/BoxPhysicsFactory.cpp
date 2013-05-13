/*
	BoxPhysics.cpp

	See BoxPhysics.h for a class description.
*/

#include "stdafx.h"
//need box2d references here
#include "src\game\Game.h"
#include "src\gsm\physics\BoxPhysicsFactory.h"
#include "BoxContactListener.h"
#include "src\gsm\state\GameStateManager.h"

/*Create an friendly physics object*/
void BoxPhysicsFactory::createPlayerObject(Game *game, AnimatedSprite *sprite, bool rotate){
	float extent_x = sprite->getSpriteType()->getTextureWidth()/2.0f;
	float extent_y = sprite->getSpriteType()->getTextureHeight()/2.0f;
	float sprite_x = sprite->getPhysicalProperties()->getX();
	float sprite_y = sprite->getPhysicalProperties()->getY();
	createDynamicBox(game,sprite,sprite,FRIENDLY_OBJECT_INDEX,(sprite_x + extent_x),
	//(sprite_y + extent_y),extent_x,extent_y,!rotate);
	(sprite_y + extent_y),16,extent_y,!rotate);
}

void BoxPhysicsFactory::createStaticPlayerObject(Game *game, AnimatedSprite *sprite){
	float extent_x = sprite->getSpriteType()->getTextureWidth()/2.0f;
	float extent_y = sprite->getSpriteType()->getTextureHeight()/2.0f;
	float sprite_x = sprite->getPhysicalProperties()->getX();
	float sprite_y = sprite->getPhysicalProperties()->getY();
	createStaticBox(game,sprite,sprite,FRIENDLY_OBJECT_INDEX,(sprite_x + extent_x),
		(sprite_y + extent_y),16,extent_y);
}

void BoxPhysicsFactory::createStaticWorldObject(Game *game, AnimatedSprite *sprite){
	float extent_x = sprite->getSpriteType()->getTextureWidth()/2.0f;
	float extent_y = sprite->getSpriteType()->getTextureHeight()/2.0f;
	float sprite_x = sprite->getPhysicalProperties()->getX();
	float sprite_y = sprite->getPhysicalProperties()->getY();
	createStaticBox(game,sprite,sprite,OTHER,(sprite_x + extent_x),
		(sprite_y + extent_y),extent_x,extent_y);
}

void BoxPhysicsFactory::createStaticWorldTrigger(Game *game, AnimatedSprite *sprite, int width, int height){
	float extent_x = width*1.0;
	float extent_y = height*1.0;
	float sprite_x = sprite->getPhysicalProperties()->getX();
	float sprite_y = sprite->getPhysicalProperties()->getY();
	createStaticBox(game,sprite,sprite,OTHER,(sprite_x + extent_x),
		(sprite_y + extent_y),extent_x,extent_y);
}

/* Create an enemy physics object*/
void BoxPhysicsFactory::createEnemyObject(Game *game, AnimatedSprite *sprite, bool rotate){
	float extent_x = sprite->getSpriteType()->getTextureWidth()/2.0f;
	float extent_y = sprite->getSpriteType()->getTextureHeight()/2.0f;
	float sprite_x = sprite->getPhysicalProperties()->getX();
	float sprite_y = sprite->getPhysicalProperties()->getY();
	if (sprite->getHitPoints()==10)
		createDynamicBox(game,sprite,sprite,ENEMY_OBJECT_INDEX,(sprite_x + extent_x),
		(sprite_y + extent_y+10),26,extent_y,!rotate);
	else
	createDynamicBox(game,sprite,sprite,ENEMY_OBJECT_INDEX,(sprite_x + extent_x),
		(sprite_y + extent_y),extent_x,extent_y,!rotate);
}

void BoxPhysicsFactory::createFriendlyProjectile(Game *game, AnimatedSprite *sprite, bool rotate){
	createPlayerObject(game,sprite,rotate);
	sprite->getPhysicsBody()->SetBullet(true);
}

/* This function will eventually be removed or replaced when we start using chain
	edges instead of static box objects for the tiles.*/

void BoxPhysicsFactory::createTileObject(Game *game, BoxPhysicsObject *phyobj, 
	float screen_center_x, float screen_center_y, float screen_extent_x, float screen_extent_y)
{
	createStaticBox(game,phyobj,screen_center_x,screen_center_y,screen_extent_x,screen_extent_y);
}


//this will eventually return something to the user, a reference to a box2d body
void BoxPhysicsFactory::createStaticBox(Game *game, BoxPhysicsObject *phyobj, 
	float screen_center_x, float screen_center_y, float screen_extent_x, float screen_extent_y)
{
	
	//perhaps I need to send the centered x and y coordinates of the body as an argument
	
	//WARNING: I NEED FLIP THE COORDINATE FOR THE Y UPSIDE DOWN SINCE THE SCREEN SPACE
		//COORDINATES USE +Y AS THE DOWN POSITION. (the start of the screen coordinate
		//space is the top left corner
		
		/** This must be tested further to confirm that this actually true about
			the coordinate spaces**/
	

	//This will create a body and associate a fixture ( I think since its needed ), and add this to the world

	b2BodyDef staticBodyDef;
	b2PolygonShape staticBox;
	b2FixtureDef fixtureDef;

	//convert to physics coordinates using the conversion factor
	//position in meters

	float32 WORLD_CONV_FACTOR = game->getGSM()->getWorld()->getWorldConvFactor();

	float center_x = screen_center_x/WORLD_CONV_FACTOR;
	float center_y = (game->getGSM()->getWorld()->getWorldHeight() - screen_center_y) / WORLD_CONV_FACTOR;
	staticBodyDef.position.Set(center_x, center_y);
	//extents in meters
	float extent_x = screen_extent_x/WORLD_CONV_FACTOR;
	float extent_y = screen_extent_y/WORLD_CONV_FACTOR;

	staticBox.SetAsBox(extent_x, extent_y);
	fixtureDef.shape = &staticBox;

	b2Body* body = physicsWorldRef->CreateBody(&staticBodyDef);
	body->CreateFixture(&fixtureDef);

	phyobj->initPhysicsBody(body);
}

//this will eventually return something to the user, a reference to a box2d body
void BoxPhysicsFactory::createDynamicBox(Game *game, BoxPhysicsObject *phyobj, AnimatedSprite * obj, int groupIndex,
	float screen_center_x, float screen_center_y, float screen_extent_x, float screen_extent_y, bool rotate)
{
	b2BodyDef dynamicBodyDef;
	b2PolygonShape dynamicBox;
	b2FixtureDef fixtureDef;

	dynamicBodyDef.type = b2_dynamicBody;
	dynamicBodyDef.fixedRotation = rotate;
	dynamicBodyDef.userData = obj;

	//convert to physics coordinates using the conversion factor
	//position in meters

	float32 WORLD_CONV_FACTOR = game->getGSM()->getWorld()->getWorldConvFactor();

	float center_x = screen_center_x/WORLD_CONV_FACTOR;
	float center_y = (game->getGSM()->getWorld()->getWorldHeight() - screen_center_y) / WORLD_CONV_FACTOR;
	dynamicBodyDef.position.Set(center_x, center_y);
	//extents in meters
	float extent_x = screen_extent_x/WORLD_CONV_FACTOR;
	float extent_y = screen_extent_y/WORLD_CONV_FACTOR;

	dynamicBox.SetAsBox(extent_x, extent_y);
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 10.0f;

	//This allows no collision to occur between two objects of the same
	//group (if the inde is negative). Each group will have a different index.
	//Bots and player will have different gorup Indices
	fixtureDef.filter.groupIndex = groupIndex;

	b2Body* body = physicsWorldRef->CreateBody(&dynamicBodyDef);
	body->CreateFixture(&fixtureDef);

	//I believe that this scale number is arbitrary for now
	//it shouldn't have any baring on actual gravity
	//need to investigate why the object is floaty without
	//this value
	body->SetGravityScale(9.0f);

	phyobj->initPhysicsBody(body);
}

void BoxPhysicsFactory::createStaticBox(Game *game, BoxPhysicsObject *phyobj, AnimatedSprite *obj, int groupIndex, 
	float screen_center_x, float screen_center_y, float screen_extent_x, float screen_extent_y) {

			//perhaps I need to send the centered x and y coordinates of the body as an argument
	
	//WARNING: I NEED FLIP THE COORDINATE FOR THE Y UPSIDE DOWN SINCE THE SCREEN SPACE
		//COORDINATES USE +Y AS THE DOWN POSITION. (the start of the screen coordinate
		//space is the top left corner
		
		/** This must be tested further to confirm that this actually true about
			the coordinate spaces**/
	

	//This will create a body and associate a fixture ( I think since its needed ), and add this to the world

	b2BodyDef staticBodyDef;
	b2PolygonShape staticBox;
	b2FixtureDef fixtureDef;

	// Give userData
	staticBodyDef.userData = obj;

	//convert to physics coordinates using the conversion factor
	//position in meters

	float32 WORLD_CONV_FACTOR = game->getGSM()->getWorld()->getWorldConvFactor();

	float center_x = screen_center_x/WORLD_CONV_FACTOR;
	float center_y = (game->getGSM()->getWorld()->getWorldHeight() - screen_center_y) / WORLD_CONV_FACTOR;
	staticBodyDef.position.Set(center_x, center_y);
	//extents in meters
	float extent_x = screen_extent_x/WORLD_CONV_FACTOR;
	float extent_y = screen_extent_y/WORLD_CONV_FACTOR;

	staticBox.SetAsBox(extent_x, extent_y);
	fixtureDef.shape = &staticBox;

	fixtureDef.filter.groupIndex = groupIndex;

	b2Body* body = physicsWorldRef->CreateBody(&staticBodyDef);
	body->CreateFixture(&fixtureDef);

	phyobj->initPhysicsBody(body);
}

void BoxPhysicsFactory::createStaticRope(Game *game, vector<AnimatedSprite *>spritesArray, float x, float y){
	//At this point the sprites are in the manager, now we attach the physics object
	//to each rectangle joint
	BoxPhysicsObject *tempObj = new BoxPhysicsObject();
	//create an invisible box for testing
	createStaticBox(game,tempObj, NULL, FRIENDLY_OBJECT_INDEX, x , y , 1, 1);

	float physics_width = game->getGSM()->screenToPhysicsX(((float)spritesArray.at(0)->getSpriteType()->getTextureWidth())/2.0f);
	float physics_height = game->getGSM()->screenToPhysicsX(((float)spritesArray.at(0)->getSpriteType()->getTextureHeight())/2.0f);

	b2PolygonShape shape;
	shape.SetAsBox(physics_width, physics_height);

	b2FixtureDef fd;
	fd.shape = &shape;
	fd.density = 10.0f;
	b2Body* prevBody = tempObj->getPhysicsBody();

	float phyX = game->getGSM()->screenToPhysicsX(x);
	float phyY = game->getGSM()->screenToPhysicsY(y);

	for(int i = 0; i < spritesArray.size(); i++){
		b2BodyDef bdef;
		bdef.type = b2_dynamicBody;
		bdef.position.Set(phyX - 5.0f, phyY - 5.0f);
		shape.SetAsBox(physics_width, physics_height);
		fd.shape = &shape;
		b2Body* body = physicsWorldRef->CreateBody(&bdef);
		body->CreateFixture(&fd);

		//b2Vec2 anchor(prevBody->GetPosition().x, prevBody->GetPosition().y - physics_height/2);
		//jd.Initialize(prevBody, body, anchor);
		//physicsWorldRef->CreateJoint(&jd);

		b2RevoluteJointDef revoluteJointDef;
		revoluteJointDef.collideConnected = false;
		revoluteJointDef.bodyA = prevBody;
		revoluteJointDef.bodyB = body;
		//these are static values now for testing, scaling the width and height
		//of the sprite's width and height
		revoluteJointDef.localAnchorA.Set(0,-0.32);
		revoluteJointDef.localAnchorB.Set(0,0.32);
		revoluteJointDef.referenceAngle = body->GetAngle() - prevBody->GetAngle();
		physicsWorldRef->CreateJoint(&revoluteJointDef);
		
		spritesArray[i]->initPhysicsBody(body);

		prevBody = body;
	}
}


void BoxPhysicsFactory::createAttackRope(Game * game, vector<AnimatedSprite *>spritesArray, float px, float py, float angle){
	//At this point the sprites are in the manager, now we attach the physics object
	//to each rectangle joint
	BoxPhysicsObject *tempObj = new BoxPhysicsObject();

	float physics_width = game->getGSM()->screenToPhysicsX(((float)spritesArray.at(0)->getSpriteType()->getTextureWidth())/2.0f);
	float physics_height = game->getGSM()->screenToPhysicsX(((float)spritesArray.at(0)->getSpriteType()->getTextureHeight())/2.0f);

	float joint_buffer = .05; // in units 
	float joint_position = physics_height - joint_buffer;

	b2BodyDef bdef;
	b2FixtureDef fd;
	b2PolygonShape shape;
	shape.SetAsBox(physics_width, physics_height);
	bdef.type = b2_dynamicBody;
	bdef.position.Set(px,py);
	bdef.angle = -angle; // negative for now, find out why, probably rotation is in a different coordinate system
	fd.shape = &shape;
	fd.density = 10.0f;

	b2RevoluteJointDef revoluteJointDef;
	revoluteJointDef.collideConnected = false;

	//create the first body in the list and assign a body to it
	b2Body *currentBody, *prevBody;
	currentBody = physicsWorldRef->CreateBody(&bdef);
	currentBody->CreateFixture(&fd);
	spritesArray[0]->initPhysicsBody(currentBody);

	prevBody = currentBody;

	for(int i = 1; i < spritesArray.size(); i++){
		currentBody = physicsWorldRef->CreateBody(&bdef);
		currentBody->CreateFixture(&fd);

		revoluteJointDef.bodyA = prevBody;
		revoluteJointDef.bodyB = currentBody;
		//these are static values now for testing, scaling the width and height
		//of the sprite's width and height
		revoluteJointDef.localAnchorA.Set(0,-joint_position);
		revoluteJointDef.localAnchorB.Set(0,joint_position);
		revoluteJointDef.referenceAngle = currentBody->GetAngle() - prevBody->GetAngle();
		physicsWorldRef->CreateJoint(&revoluteJointDef);
		
		spritesArray[i]->initPhysicsBody(currentBody);

		prevBody = currentBody;
	}
}