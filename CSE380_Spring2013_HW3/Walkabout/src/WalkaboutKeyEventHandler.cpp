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
#include "src\game\Game.h"
#include "src\game\WStringTable.h"
#include "src\graphics\GameGraphics.h"
#include "src\gsm\physics\Physics.h"
#include "src\gsm\physics\PhysicalProperties.h"
#include "src\gsm\sprite\AnimatedSprite.h"
#include "src\gsm\state\GameState.h"
#include "src\gsm\state\GameStateManager.h"
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
	AnimatedSprite *player = gsm->getSpriteManager()->getPlayer();
	PhysicalProperties *pp = player->getPhysicalProperties();	

	// IF THE GAME IS IN PROGRESS
	if (gsm->isGameInProgress())
	{
		// WASD KEY PRESSES WILL CONTROL THE PLAYER
		// SO WE'LL UPDATE THE PLAYER VELOCITY WHEN THESE KEYS ARE
		// PRESSED, THAT WAY PHYSICS CAN CORRECT AS NEEDED
		float vX = pp->getVelocityX();
		float vY = pp->getVelocityY();

		if (input->isKeyDown(A_KEY))
		{
			vX = -PLAYER_SPEED;
			if (player->wasOnTileLastFrame())
				player->setCurrentState(WALKING_LEFT);
		}
		else if (input->isKeyDown(D_KEY))
		{
			vX = PLAYER_SPEED;
			if (player->wasOnTileLastFrame())
				player->setCurrentState(WALKING_RIGHT);
		}
		else
		{
			vX = 0.0f;
			if(player->getCurrentState() == WALKING_RIGHT)
				player->setCurrentState(IDLE_RIGHT);

			if(player->getCurrentState() == WALKING_LEFT)
				player->setCurrentState(IDLE_LEFT);
			if(player->getCurrentState() == JUMPING_RIGHT && player->getPhysicalProperties()->getVelocityY()==0)
				player->setCurrentState(IDLE_RIGHT);
			if(player->getCurrentState() == JUMPING_LEFT && player->getPhysicalProperties()->getVelocityY()==0)
				player->setCurrentState(IDLE_LEFT);
		}
		if (input->isKeyDownForFirstTime(SPACE_KEY))
		{
			if (player->wasOnTileLastFrame())
			{
				vY = JUMP_SPEED;
				if(player->getCurrentState() == WALKING_RIGHT || player->getCurrentState()==IDLE_RIGHT)
				player->setCurrentState(JUMPING_RIGHT);

				if(player->getCurrentState() == WALKING_LEFT || player->getCurrentState()==IDLE_LEFT)
				player->setCurrentState(JUMPING_LEFT);
			}
			else
			{
				cout << "WHAT HAPPENED?";
			}
		}
		if (input->isKeyDownForFirstTime(ENTER_KEY))
		{
			if (gsm->getPhysics()->isActivated())
				gsm->getPhysics()->togglePhysics();
			gsm->goToPaused();
		}
		if (input->isKeyDownForFirstTime(T_KEY))
		{
			gsm->getPhysics()->activateForSingleUpdate();
		}

		// NOW SET THE ACTUAL PLAYER VELOCITY
 		pp->setVelocity(vX, vY);

		bool viewportMoved = false;
		float viewportVx = 0.0f;
		float viewportVy = 0.0f;
		Viewport *viewport = game->getGUI()->getViewport();
/*
		// Player on right half of viewport
		if((pp->getX() - viewport->getViewportX()) > (viewport->getViewportWidth() / 2.0f)){
			if(pp->getVelocityX() > 0) {
				viewportVx = pp->getVelocityX();
				viewportMoved = true;
			}
		}

		// Player on left half of viewport
		if((pp->getX() - viewport->getViewportX()) < (viewport->getViewportWidth() / 2.0f)){
			if(pp->getVelocityX() < 0) {
				viewportVx = pp->getVelocityX();
				viewportMoved = true;
			}
		}

		// Player on bottom half of viewport
		if((pp->getY() - viewport->getViewportY()) > (viewport->getViewportHeight() / 2.0f)){
			if(pp->getVelocityY() > 0) {
				viewportVy = pp->getVelocityY();
				viewportMoved = true;
			}
		}

		// Player on top half of viewport
		if((pp->getY() - viewport->getViewportY()) < (viewport->getViewportHeight() / 2.0f)){
			if(pp->getVelocityY() < 0) {
				viewportVy = pp->getVelocityY();
				viewportMoved = true;
			}
		}
	*/


		if (input->isKeyDown(UP_KEY))
		{
			viewportVy -= MAX_VIEWPORT_AXIS_VELOCITY;
			viewportMoved = true;
		}
		if (input->isKeyDown(DOWN_KEY))
		{
			viewportVy += MAX_VIEWPORT_AXIS_VELOCITY;
			viewportMoved = true;
		}
		if (input->isKeyDown(LEFT_KEY))
		{
			viewportVx -= MAX_VIEWPORT_AXIS_VELOCITY;
			viewportMoved = true;
		}
		if (input->isKeyDown(RIGHT_KEY))
		{
			viewportVx += MAX_VIEWPORT_AXIS_VELOCITY;
			viewportMoved = true;
		}
		
		if (viewportMoved)
			viewport->moveViewport((int)viewportVx, (int)viewportVy, game->getGSM()->getWorld()->getWorldWidth(), game->getGSM()->getWorld()->getWorldHeight());
		
	}
	/*
	// 0X43 is HEX FOR THE 'C' VIRTUAL KEY
	// THIS CHANGES THE CURSOR IMAGE
	if ((input->isKeyDownForFirstTime(C_KEY))
		&& input->isKeyDown(VK_SHIFT))
	{
		Cursor *cursor = game->getGUI()->getCursor();
		unsigned int id = cursor->getActiveCursorID();
		id++;
		if (id == cursor->getNumCursorIDs())
			id = 0;		
		cursor->setActiveCursorID(id);
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
		timer->setTargetFPS(fps - FPS_INC);*/
}