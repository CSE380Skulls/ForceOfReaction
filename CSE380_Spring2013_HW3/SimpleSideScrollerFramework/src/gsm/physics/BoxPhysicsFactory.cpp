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
