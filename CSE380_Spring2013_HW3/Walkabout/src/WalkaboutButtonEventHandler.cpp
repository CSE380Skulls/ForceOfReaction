/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	WalkaboutButtonEventHandler.cpp

	See WalkaboutButtonEventHandler.h for a class description.
*/

#include "stdafx.h"
#include "src\WalkaboutGame.h"
#include "src\WalkaboutButtonEventHandler.h"
#include "src\game\Game.h"
#include "src\gsm\state\GameStateManager.h"
#include "src\audio\GameAudioManager.h"

void WalkaboutButtonEventHandler::handleButtonEvents(	Game *game, 
													wstring command)
{
	// THE USER PRESSED THE Exit BUTTON ON THE MAIN MENU,
	// SO LET'S SHUTDOWN THE ENTIRE APPLICATION
	if (command.compare(W_EXIT_COMMAND) == 0)
	{
		game->shutdown();
	}
	// THE USER PRESSED THE MOUSE BUTTON ON THE SPLASH
	// SCREEN, SO LET'S GO TO THE MAIN MENU
	else if (command.compare(W_GO_TO_MM_COMMAND) == 0)
	{
		GameStateManager *gsm = game->getGSM();
		gsm->goToMainMenu();
	}
	// THE USER PRESSED THE Start BUTTON ON THE MAIN MENU,
	// SO LET'S START THE GAME FROM THE FIRST LEVEL
	else if (command.compare(W_START_COMMAND) == 0)
	{
		game->getGAM()->setMusic(C_LEVEL1);
		game->setCurrentLevelFileName(W_LEVEL_1_NAME);
		game->startGame();
	}
	else if (command.compare(W_RETURN_COMMAND) == 0)
	{
		game->getGSM()->getPhysics()->togglePhysics();
		game->getGSM()->goToGame();
	}
	else if (command.compare(W_HELP_COMMAND) == 0)
	{
		GameStateManager *gsm = game->getGSM();
		gsm->goToHelp();
	}
	else if (command.compare(W_CREDITS_COMMAND) == 0)
	{
		GameStateManager *gsm = game->getGSM();
		gsm->goToCredits();
	}
	// THE USER PRESSED THE Quit BUTTON ON THE IN-GAME MENU,
	// SO LET'S UNLOAD THE LEVEL AND RETURN TO THE MAIN MENU
	else if (command.compare(W_QUIT_COMMAND) == 0)
	{
		game->getGAM()->setMusic(C_INTRO);
		game->quitGame();
	}
}