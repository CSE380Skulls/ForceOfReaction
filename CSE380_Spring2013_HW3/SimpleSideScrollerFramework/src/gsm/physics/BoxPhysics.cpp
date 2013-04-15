/*
	BoxPhysics.cpp

	See BoxPhysics.h for a class description.
*/

#include "stdafx.h"
//need box2d references here
#include "src\game\Game.h"
#include "src\gsm\physics\BoxPhysics.h"
#include "src\gsm\state\GameStateManager.h"

/*
	The constructor initializes the data structures and loads
	the necessary ones with recyclable objects for collision tests.
*/
BoxPhysics::BoxPhysics()
{
	//initialize the game world
	b2Vec2 gravity;
	gravity.Set(0.0f, -9.8f);
	physics_world = new b2World(gravity);
	physics_world->SetAllowSleeping(true);

	b2StepHertz = 60.0f;
	velocityIterations = 6;
	positionIterations = 2;
}

/*
	The destructor deletes all the recyclable objects.
*/
BoxPhysics::~BoxPhysics()
{
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
	//call the box2d step
	physics_world->Step( 1.0f / b2StepHertz , velocityIterations, positionIterations);
	
	//update the screen coordinate locations of the dynamic objects based on the
	//box2d world coordinates, probably need to tell the sprite manager this information
	//since it carries all the references to the sprites in the game (should this update
	//be in this function? Something to think about perhaps)

	//for now we are just going to directly update the one test sprite we have right now:
	//This is not right to do here, but this will be moved once confirmed to work, I say this
	//because the physics engine should have no idea what an animated sprite is

	//AnimatedSprite * test_sprite = game->getGSM()->getSpriteManager()->getTestSprite();
	//int worldHeightMeters = game->getGSM()->getWorld()->getWorldHeight()/WORLD_CONV_FACTOR;
	//PhysicalProperties * test_pp = test_sprite->getPhysicalProperties();


	//test_pp->setX( (test_sprite->getCurrentBodyX() * WORLD_CONV_FACTOR) - 
	//	test_sprite->getSpriteType()->getTextureWidth());
	//test_pp->setY( ( (-test_sprite->getCurrentBodyY() + worldHeightMeters) * 
	//	WORLD_CONV_FACTOR) - test_sprite->getSpriteType()->getTextureHeight());

	//test_pp->setX(game->getGSM()->physicsToScreenX(test_sprite->getCurrentBodyX()) - test_sprite->getSpriteType()->getTextureWidth());
	//test_pp->setY(game->getGSM()->physicsToScreenY(test_sprite->getCurrentBodyY()) - test_sprite->getSpriteType()->getTextureHeight());

	
}

//this will eventually return something to the user, a reference to a box2d body
void BoxPhysics::createStaticBox(Game *game, BoxPhysicsObject *phyobj, 
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

	float center_x = screen_center_x/WORLD_CONV_FACTOR;
	float center_y = (game->getGSM()->getWorld()->getWorldHeight() - screen_center_y) / WORLD_CONV_FACTOR;
	staticBodyDef.position.Set(center_x, center_y);
	//extents in meters
	float extent_x = screen_extent_x/WORLD_CONV_FACTOR;
	float extent_y = screen_extent_y/WORLD_CONV_FACTOR;

	staticBox.SetAsBox(extent_x, extent_y);
	fixtureDef.shape = &staticBox;

	b2Body* body = physics_world->CreateBody(&staticBodyDef);
	body->CreateFixture(&fixtureDef);

	phyobj->initPhysicsBody(body);
}

//this will eventually return something to the user, a reference to a box2d body
void BoxPhysics::createDynamicBox(Game *game, BoxPhysicsObject *phyobj, CollidableObject * obj, 
	float screen_center_x, float screen_center_y, float screen_extent_x, float screen_extent_y)
{
	b2BodyDef dynamicBodyDef;
	b2PolygonShape dynamicBox;
	b2FixtureDef fixtureDef;

	dynamicBodyDef.type = b2_dynamicBody;
	dynamicBodyDef.fixedRotation = true;

	//convert to physics coordinates using the conversion factor
	//position in meters

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
	fixtureDef.userData = obj;

	b2Body* body = physics_world->CreateBody(&dynamicBodyDef);
	body->CreateFixture(&fixtureDef);

	//I believe that this scale number is arbitrary for now
	//it shouldn't have any baring on actual gravity
	//need to investigate why the object is floaty without
	//this value
	body->SetGravityScale(9.0f);

	phyobj->initPhysicsBody(body);
}
