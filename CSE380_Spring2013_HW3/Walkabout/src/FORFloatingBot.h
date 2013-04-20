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

	// THIS PRIVATE CONSTRUCTOR IS ONLY USED FOR CLONING
	FORFloatingBot(	int initMin, 
						int initMax, 
						int initVelocity);

public:
	FORFloatingBot(	Physics *physics,
						int initMin, 
						int initMax, 
						int initVelocity);
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