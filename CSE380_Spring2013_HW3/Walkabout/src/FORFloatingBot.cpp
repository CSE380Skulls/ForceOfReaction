#include "stdafx.h"
#include "src\game\Game.h"
#include "src\FORFloatingBot.h"
#include "src\gsm\state\GameStateManager.h"
#include "src\audio\GameAudioManager.h"
#include "src\gsm\physics\Physics.h"
#include "src\WalkaboutGame.h"
#include "src\audio\GameAudioManager.h"

/*
	This private constructor is only to be used for cloning bots, note
	that this does not setup the velocity for this bot.
*/
FORFloatingBot::FORFloatingBot(	int initMin, 
								int initMax, 
								int initVelocity,
								int spawnX,
								int travelDistance,
								int designation)
{
	// Init player targeting variables
	this->spawnX = spawnX;
	this->travelDistance = travelDistance;
	this->designation = designation;

	// INIT THE BASIC STUFF
	initBot(initMin, initMax, initVelocity);

	// AND START THE BOT OFF WITH RANDOM INTERVAL UNTIL IT THINKS AGAIN
	pickRandomCyclesInRange();
}

/*
	clone - this method makes another RandomFloatingBot object, but does
	not completely initialize it with similar data to this. Most of the 
	object, like velocity and position, are left uninitialized.
*/
/*
Bot* FORFloatingBot::clone()
{
	FORFloatingBot *botClone = new FORFloatingBot(	minCyclesBeforeThinking, 
															maxCyclesBeforeThinking, 
															velocity,
															spawnX,
															travelDistance);
	return botClone;
}
*/

/*
	initBot - this initialization method sets up the basic bot
	properties, but does not setup its velocity.
*/
void FORFloatingBot::initBot(	int initMin,
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
	stunned = false;
}

/*
	pickRandomCyclesInRange - a randomized method for determining when this bot
	will think again. This method sets that value.
*/
void FORFloatingBot::pickRandomCyclesInRange()
{
	cyclesRemainingBeforeThinking = maxCyclesBeforeThinking - minCyclesBeforeThinking + 1;
	cyclesRemainingBeforeThinking = rand() % cyclesRemainingBeforeThinking;
	cyclesRemainingBeforeThinking += minCyclesBeforeThinking;
}

/*
	Calculates a random velocity for this bot, either left, right, nowhere
*/

void FORFloatingBot::pickRandomVelocity(Physics *physics)
{
	// Get a random number
	int r = rand() % 30; 

	if(r >= 20){
		// Don't move
		dir = 0;
	}
	else if(r >= 10){
		// Move Right
		dir = 1;
		setCurrentState(IDLE_RIGHT);
	}
	else {
		// Move left
		dir = 2;
		setCurrentState(IDLE_LEFT);
	}
}

bool FORFloatingBot::isInBounds(int x){
	if(x > (spawnX + travelDistance))
		return false;
	if(x < (spawnX - travelDistance))
		return false;
	return true;
}

/*
	think - called once per frame, this is where the bot performs its
	decision-making. Note that we might not actually do any thinking each
	frame, depending on the value of cyclesRemainingBeforeThinking.
*/
void FORFloatingBot::update(Game *game)
{
	if(dead)
		return;
	if(hitPoints <= 0){
		game->getGSM()->getSpriteManager()->addBotToRemovalList(this, 30);
		setCurrentState(DEAD);
		dead = true;
		return;
	}
	// If player is next to this bot, do something different
	int botX = getCurrentBodyX() * BOX2D_CONVERSION_FACTOR;
	int pX = game->getGSM()->getSpriteManager()->getPlayer()->getCurrentBodyX() * BOX2D_CONVERSION_FACTOR;
	int playerHealth  = game->getGSM()->getSpriteManager()->getPlayer()->getHitPoints();

	// If the player is within the bots targeting area and player isn't dead, go after the player
	if(isInBounds(pX) && playerHealth > 0) {
		int botY = getCurrentBodyY() * BOX2D_CONVERSION_FACTOR;
		int pY = game->getGSM()->getSpriteManager()->getPlayer()->getCurrentBodyY() * BOX2D_CONVERSION_FACTOR;
		// Make sure the player is in the same y area
		if(std::abs(botY - pY) < 200){
			if(pX > botX){
				dir = 1;
				setCurrentState(ATTACKING_RIGHT);
			}
			if(pX < botX){
				dir = 2;
				setCurrentState(ATTACKING_LEFT);
			}
		}
	}

	// If the bot isn't in the bounds, change its direction
	if(!isInBounds(botX)){
		if(botX > (spawnX + travelDistance)) {
			dir = 2;
			setCurrentState(IDLE_LEFT);
		}
		if(botX < (spawnX - travelDistance)) {
			dir = 1;
			setCurrentState(IDLE_RIGHT);
		}
	}

	// Based the the direction the bot is currently going, change its velocity.
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

	// If stunned, don't let move
	if(stunned)
		getPhysicsBody()->SetLinearVelocity(b2Vec2(0, getPhysicsBody()->GetLinearVelocity().y));

	if (cyclesRemainingBeforeThinking <= 0)
	{
		if(stunned)
			stunned = false;
		GameStateManager *gsm = game->getGSM();
		pickRandomVelocity(gsm->getPhysics());
		pickRandomCyclesInRange();
	}
	else
		cyclesRemainingBeforeThinking--;
}

void FORFloatingBot::playSound(Game *game, SpriteDesignations soundType) {
	if(soundType == SPRITE_DEAD && !dead){
		game->getGAM()->playSound(C_EXPLOSION2);
	}

	if(soundType == SPRITE_HIT){
		game->getGAM()->playSound(C_HIT);
	}
}

void FORFloatingBot::stun(int framesStunned) {
	if(!dead && !stunned) {
		cyclesRemainingBeforeThinking = framesStunned;
		//pickRandomCyclesInRange();
		stunned = true;
	}
}