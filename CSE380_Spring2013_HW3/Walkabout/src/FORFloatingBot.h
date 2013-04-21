#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "stdafx.h"
#include "src\gsm\ai\Bot.h"
#include "src\gsm\physics\Physics.h"
#include "src\game\Game.h"

class FORFloatingBot : public Bot
{
private:
	int cyclesRemainingBeforeThinking;
	int minCyclesBeforeThinking;
	int maxCyclesBeforeThinking;
	int velocity;
	int dir;
	bool removed;

	// Note: For right now, bots can only move left and right. Unless adding another type of bot, don't need spawnX.
	int spawnX; // The location this robot is spawned at
	int travelDistance; // The distance this robot is alloud to travel from its spawn Location

	// THIS PRIVATE CONSTRUCTOR IS ONLY USED FOR CLONING
	/*FORFloatingBot(	int initMin, 
						int initMax, 
						int initVelocity);*/

	// Is this bot in bounds (only aloud to move travelDistance away from spawn distance)
	bool	isInBounds(int x);

public:
	/*FORFloatingBot(	Physics *physics,
						int initMin, 
						int initMax, 
						int initVelocity);*/
	FORFloatingBot(	    int initMin, 
						int initMax, 
						int initVelocity,
						int spawnX,
						int travelDistance);
	~FORFloatingBot();
	Bot*	clone();
	void initBot(	int initMin,
					int initMax,
					int initVelocity);
	void	pickRandomCyclesInRange();
	void	pickRandomVelocity(Physics *physics);
	void	think(Game *game);

	// INLINED METHODS
	int getCyclesRemainingBeforeThinking()	{ return cyclesRemainingBeforeThinking; }
	int getMinCyclesBeforeThinking()		{ return minCyclesBeforeThinking;		}
	int getMaxCyclesBeforeThinking()		{ return maxCyclesBeforeThinking;		}
};