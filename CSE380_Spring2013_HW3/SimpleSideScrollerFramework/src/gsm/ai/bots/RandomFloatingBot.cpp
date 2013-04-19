#include "stdafx.h"
#include "src\game\Game.h"
#include "src\gsm\ai\bots\RandomFloatingBot.h"
#include "src\gsm\state\GameStateManager.h"
#include "src\gsm\physics\Physics.h"

/*
	This private constructor is only to be used for cloning bots, note
	that this does not setup the velocity for this bot.
*/
RandomFloatingBot::RandomFloatingBot(	int initMin, 
										int initMax, 
										int initVelocity)
{
	// INIT THE BASIC STUFF
	initBot(initMin, initMax, initVelocity);
}

/*
	This is the public constructor used by other classes for 
	creating these types of bots.
*/
RandomFloatingBot::RandomFloatingBot(	Physics *physics,
										int initMin, 
										int initMax, 
										int initVelocity)
{
	// INIT THE BASIC STUFF
	initBot(initMin, initMax, initVelocity);

	// AND START THE BOT OFF IN A RANDOM DIRECTION AND VELOCITY
	// AND WITH RANDOM INTERVAL UNTIL IT THINKS AGAIN
	//pickRandomVelocity(physics);
	pickRandomCyclesInRange();
}

/*
	clone - this method makes another RandomFloatingBot object, but does
	not completely initialize it with similar data to this. Most of the 
	object, like velocity and position, are left uninitialized.
*/
Bot* RandomFloatingBot::clone()
{
	RandomFloatingBot *botClone = new RandomFloatingBot(	minCyclesBeforeThinking, 
															maxCyclesBeforeThinking, 
															velocity);
	return botClone;
}

/*
	initBot - this initialization method sets up the basic bot
	properties, but does not setup its velocity.
*/
void RandomFloatingBot::initBot(	int initMin,
									int initMax,
									int initVelocity)
{
	// IF THE MAX IS SMALLER THAN THE MIN, SWITCH THEM
	if (initMax < initMin)
	{
		int temp = initMax;
		initMax = initMin;
		initMin = temp;
	}
	// IF THEY ARE THE SAME, ADD 100 CYCLES TO THE MAX
	else if (initMax == initMin)
		initMax += 100;

	// INIT OUR RANGE VARIABLES
	minCyclesBeforeThinking = initMin;
	maxCyclesBeforeThinking = initMax;

	// AND OUR VELOCITY CAPPER
	velocity = initVelocity;
	dir = -1;
}

/*
	pickRandomCyclesInRange - a randomized method for determining when this bot
	will think again. This method sets that value.
*/
void RandomFloatingBot::pickRandomCyclesInRange()
{
	
	cyclesRemainingBeforeThinking = maxCyclesBeforeThinking - minCyclesBeforeThinking + 1;
	cyclesRemainingBeforeThinking = rand() % cyclesRemainingBeforeThinking;
	cyclesRemainingBeforeThinking += minCyclesBeforeThinking;
	
	//cyclesRemainingBeforeThinking = 120;
}

/*
	Calculates a random velocity for this bot, either left, right, nowhere
*/

void RandomFloatingBot::pickRandomVelocity(Physics *physics)
{
	// Get a random number
	int r = rand() % 30; 

	if(r >= 20){
		// Don't move
		dir = 0;
		//getPhysicsBody()->SetLinearVelocity(b2Vec2(0, getPhysicsBody()->GetLinearVelocity().y));
	}
	else if(r >= 10){
		// Move Right
		dir = 1;
		setCurrentState(L"IDLE_RIGHT");
		//getPhysicsBody()->SetLinearVelocity(b2Vec2(velocity, getPhysicsBody()->GetLinearVelocity().y));
	}
	else {
		// Move left
		//getPhysicsBody()->SetLinearVelocity(b2Vec2(-velocity, getPhysicsBody()->GetLinearVelocity().y));
		dir = 2;
		this->setCurrentState(L"IDLE_LEFT");
	}
}

/*
	think - called once per frame, this is where the bot performs its
	decision-making. Note that we might not actually do any thinking each
	frame, depending on the value of cyclesRemainingBeforeThinking.
*/
void RandomFloatingBot::think(Game *game)
{
	// EACH FRAME WE'LL TEST THIS BOT TO SEE IF WE NEED
	// TO PICK A DIFFERENT DIRECTION TO FLOAT IN

	// If player is next to this bot, do something different
	if(dir == 0) {
		// Idle
		getPhysicsBody()->SetLinearVelocity(b2Vec2(0, getPhysicsBody()->GetLinearVelocity().y));
	}
	else if(dir == 1){
		// Move Right
		getPhysicsBody()->SetLinearVelocity(b2Vec2(velocity, getPhysicsBody()->GetLinearVelocity().y));
	}
	else if(dir == 2) {
		// Move Left
		getPhysicsBody()->SetLinearVelocity(b2Vec2(-velocity, getPhysicsBody()->GetLinearVelocity().y));
	}


	if (cyclesRemainingBeforeThinking == 0)
	{
		GameStateManager *gsm = game->getGSM();
		pickRandomVelocity(gsm->getPhysics());
		pickRandomCyclesInRange();
	}
	else
		cyclesRemainingBeforeThinking--;
}