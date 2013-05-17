/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	WalkaboutKeyEventHandler.cpp

	See WalkaboutKeyEventHandler.h for a class description.
*/

#include "stdafx.h"
#include "src\WalkaboutGame.h"
#include "src\WalkaboutKeyEventHandler.h"
#include "src\Seed.h"
#include "src\Vine.h"
#include "src\FORPlayer.h"
#include "src\game\Game.h"
#include "src\game\WStringTable.h"
#include "src\graphics\GameGraphics.h"
#include "src\gsm\physics\Physics.h"
#include "src\gsm\physics\PhysicalProperties.h"
#include "src\gsm\sprite\AnimatedSprite.h"
#include "src\gsm\state\GameState.h"
#include "src\gsm\state\GameStateManager.h"
#include "src\audio\GameAudioManager.h"
#include "src\gui\Cursor.h"
#include "src\gui\GameGUI.h"
#include "src\input\GameInput.h"
#include "src\timer\GameTimer.h"
#include "src\PlatformPlugins\WindowsPlugin\WindowsTimer.h"

/*
	handleKeyEvent - this method handles all keyboard interactions. Note that every frame this method
	gets called and it can respond to key interactions in any custom way. Ask the GameInput class for
	key states since the last frame, which can allow us to respond to key presses, including when keys
	are held down for multiple frames.
*/
void WalkaboutKeyEventHandler::handleKeyEvents(Game *game)
{
	// WE CAN QUERY INPUT TO SEE WHAT WAS PRESSED
	GameInput *input = game->getInput();

	// LET'S GET THE PLAYER'S PHYSICAL PROPERTIES, IN CASE WE WANT TO CHANGE THEM
	GameStateManager *gsm = game->getGSM();
	FORPlayer *player = (FORPlayer*)gsm->getSpriteManager()->getPlayer();
	PhysicalProperties *pp = player->getPhysicalProperties();

	//((FOR_Player*)player)->update();

	// IF THE GAME IS IN PROGRESS
	if (gsm->isGameInProgress())
	{
		// WASD KEY PRESSES WILL CONTROL THE PLAYER
		// SO WE'LL UPDATE THE PLAYER VELOCITY WHEN THESE KEYS ARE
		// PRESSED, THAT WAY PHYSICS CAN CORRECT AS NEEDED
		if (player->getHitPoints()>0) {
			if (input->isKeyDown(A_KEY) || input->isKeyDown(LEFT_KEY))
			{
				player->setDirection(-1);
				player->run();
			}
			else if (input->isKeyDown(D_KEY) || input->isKeyDown(RIGHT_KEY))
			{
				player->setDirection(1);
				player->run();
			}
			else
			{
				player->hover();
			}
			if (input->isKeyDownForFirstTime(SPACE_KEY))
			{
				player->jump(game);
			}
			if (input->isKeyDownForFirstTime(W_KEY)){
				player->hookToVine(game);
			}
			if (input->isKeyDownForFirstTime(ESCAPE_KEY))
			{
				if (gsm->getPhysics()->isActivated())
					gsm->getPhysics()->togglePhysics();
				gsm->goToPaused();
			}
			if (input->isKeyDownForFirstTime(T_KEY))
			{
				gsm->getPhysics()->activateForSingleUpdate();
			}
			if (input->isKeyDownForFirstTime(MOUSE_LEFT))
			{
				float mX = game->getGUI()->getCursor()->getX() + game->getGUI()->getViewport()->getViewportX();
				float mY = game->getGUI()->getCursor()->getY() + game->getGUI()->getViewport()->getViewportY();
				player->leftAttack(game, mX, mY);
			}
			if(input->isKeyDownForFirstTime(MOUSE_RIGHT))
			{
				float mX = game->getGUI()->getCursor()->getX() + game->getGUI()->getViewport()->getViewportX();
				float mY = game->getGUI()->getCursor()->getY() + game->getGUI()->getViewport()->getViewportY();
				player->rightAttack(game,mX,mY);
			}
		}
		// CTRL + 1 = auto win level
		if(input->isKeyDown(CTRL_KEY) && input->isKeyDownForFirstTime(ONE_KEY)){
			if (gsm->getPhysics()->isActivated())
				gsm->getPhysics()->togglePhysics();
			gsm->goToLevelWon();
		}
		// CTRL + 2 = auto lose level
		if(input->isKeyDown(CTRL_KEY) && input->isKeyDownForFirstTime(TWO_KEY)){
			if (gsm->getPhysics()->isActivated())
				gsm->getPhysics()->togglePhysics();
			gsm->goToGameOver();
		}

		if(input->isKeyDownForFirstTime(Q_KEY)){
			player->nextElement();
		}
		bool viewportMoved = false;
		float viewportVx = 0.0f;
		float viewportVy = 0.0f;
		Viewport *viewport = game->getGUI()->getViewport();

		float x = game->getGSM()->getSpriteManager()->getPlayer()->getCurrentBodyX();
		float y = game->getGSM()->getSpriteManager()->getPlayer()->getCurrentBodyY();
		float pX = x * game->getGSM()->getWorld()->getWorldConvFactor();
		float pY = y * game->getGSM()->getWorld()->getWorldConvFactor();
		pY = game->getGSM()->getWorld()->getWorldHeight() - pY;

		float vpX = viewport->getViewportX();
		float vpY = viewport->getViewportY();
		float vpWidth = viewport->getViewportWidth();
		float vpHeight = viewport->getViewportHeight();
		 
		// If player is on left 3/8 of viewport
		if(pX < ((15.0f / 32.0f * vpWidth) + vpX)) {
			viewportVx = -MAX_VIEWPORT_AXIS_VELOCITY; 
			viewportMoved = true;
		 }

		 // If player is on right 5/8 of viewport
		 if(pX > ((17.0f / 32.0f * vpWidth) + vpX)) {
			viewportVx = MAX_VIEWPORT_AXIS_VELOCITY; 
			viewportMoved = true;
		 }

		 // If player is on top 3/8 of viewport
		 if(pY < ((15.0f / 32.0f * vpHeight) + vpY)) {
			viewportVy = -MAX_VIEWPORT_AXIS_VELOCITY; 
			viewportMoved = true;
		 }

		 // If player is on bottom 5/8 of viewport
		 if(pY > ((17.0f / 32.0f * vpHeight) + vpY)) {
			viewportVy = MAX_VIEWPORT_AXIS_VELOCITY; 
			viewportMoved = true;
		 }
		
		if (viewportMoved)
			viewport->moveViewport((int)viewportVx, (int)viewportVy, game->getGSM()->getWorld()->getWorldWidth(), game->getGSM()->getWorld()->getWorldHeight());
	}

	// LET'S MESS WITH THE TARGET FRAME RATE IF THE USER PRESSES THE HOME OR END KEYS
	WindowsTimer *timer = (WindowsTimer*)game->getTimer();
	int fps = timer->getTargetFPS();

	// THIS SPEEDS UP OUR GAME LOOP AND THUS THE GAME, NOTE THAT WE COULD ALTERNATIVELY SCALE
	// DOWN THE GAME LOGIC (LIKE ALL VELOCITIES) AS WE SPEED UP THE GAME. THAT COULD PROVIDE
	// A BETTER PLAYER EXPERIENCE
	if (input->isKeyDown(VK_HOME) && (fps < MAX_FPS))
		timer->setTargetFPS(fps + FPS_INC);

	// THIS SLOWS DOWN OUR GAME LOOP, BUT WILL NOT GO BELOW 5 FRAMES PER SECOND
	else if (input->isKeyDown(VK_END) && (fps > MIN_FPS))
		timer->setTargetFPS(fps - FPS_INC);
}