/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	WalkaboutGame.cpp

	This is a test game application, a game that demonstrates use of the 
	SideScrollerFramework to make a little scrolling, interactive demo. It
	demonstrates the rendering of images and text as well as responding to
	key presses and button cicks. Students should make their own named game
	applictions using a similar pattern, gradually adding other components,
	like additional gui controls, tiles, sprites, ai, and physics.
 */

#include "stdafx.h"

// Walkabout GAME INCLUDES
#include "src\WalkaboutButtonEventHandler.h"
#include "src\WalkaboutCollisionListener.h"
#include "src\WalkaboutDataLoader.h"
#include "src\WalkaboutGame.h"
#include "src\WalkaboutKeyEventHandler.h"
#include "src\WalkaboutTextGenerator.h"

// GAME OBJECT INCLUDES
#include "src\game\Game.h"
#include "src\graphics\GameGraphics.h"
#include "src\gsm\state\GameState.h"
#include "src\gui\Cursor.h"
#include "src\gui\GameGUI.h"
#include "src\gui\ScreenGUI.h"
#include "src\input\GameInput.h"
#include "src\os\GameOS.h"
#include "src\text\GameText.h"

// WINDOWS PLATFORM INCLUDES
#include "src\PlatformPlugins\WindowsPlugin\WindowsOS.h"
#include "src\PlatformPlugins\WindowsPlugin\WindowsInput.h"
#include "src\PlatformPlugins\WindowsPlugin\WindowsTimer.h"

// DIRECTX INCLUDES
#include "src\PlatformPlugins\DirectXPlugin\DirectXGraphics.h"
#include "src\PlatformPlugins\DirectXPlugin\DirectXTextureManager.h"

/*
	WinMain - This is the application's starting point. In this method we will construct a Game object, 
	then initialize all the platform-dependent technologies, then construct all the custom data for our 
	game, and then initialize the Game with	our custom data. We'll then start the game loop.
*/

struct Nums
{
	int num1;
	int num2;
};
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
	// CREATE THE GAME
	Game *walkaboutGame = new Game();
	walkaboutGame->getGSM()->getWorld()->setWorldConvFactor(BOX2D_CONVERSION_FACTOR);

	// FIRST WE'LL SETUP THE DATA LOADER, SINCE IT MAY NEED TO READ
	// IN DATA TO SETUP OTHER STUFF
	WalkaboutDataLoader *walkaboutDataLoader = new WalkaboutDataLoader();
	walkaboutDataLoader->initWinHandle(hInstance, nCmdShow);
	walkaboutGame->setDataLoader(walkaboutDataLoader);
	walkaboutDataLoader->loadGame(walkaboutGame, W_INIT_FILE);
	
	// WHAT WE SHOULD BE DOING HERE IS LOADING THE GAME DATA FROM FILES. THIS
	// MEANS THE GUIS THEMSELVES AS WELL AS THE LEVELS. THAT WILL BE LEFT
	// FOR BECHMARK HWS. FOR NOW WE WILL JUST HARD CODE THE LOADING OF THE GUI

	// LOAD THE GUI STUFF, AGAIN, NOTE THAT THIS SHOULD REALLY
	// BE DONE FROM A FILE, NOT HARD CODED
	walkaboutDataLoader->loadGUI(walkaboutGame, W_GUI_INIT_FILE);

	// SPECIFY WHO WILL HANDLE BUTTON EVENTS
	WalkaboutButtonEventHandler *walkaboutButtonHandler = new WalkaboutButtonEventHandler();
	GameGUI *gui = walkaboutGame->getGUI();
	gui->registerButtonEventHandler((ButtonEventHandler*)walkaboutButtonHandler);

	// SPECIFY WHO WILL HANDLE KEY EVENTS
	WalkaboutKeyEventHandler *walkaboutKeyHandler = new WalkaboutKeyEventHandler();
	walkaboutGame->getInput()->registerKeyHandler((KeyEventHandler*)walkaboutKeyHandler);

	// THIS WILL HANDLE PHYSICS COLLISION EVENTS
	WalkaboutCollisionListener *walkaboutCollisionListener = new WalkaboutCollisionListener();
	walkaboutGame->getGSM()->getPhysics()->setCollisionListener(walkaboutCollisionListener);

	// START THE GAME LOOP
	walkaboutGame->runGameLoop();

	// GAME'S OVER SHUTDOWN ALL THE STUFF WE CONSTRUCTED HERE
	delete (WindowsOS*)walkaboutGame->getOS();
	delete (WindowsInput*)walkaboutGame->getInput();
	delete (WindowsTimer*)walkaboutGame->getTimer();
	delete (DirectXGraphics*)walkaboutGame->getGraphics();
	delete (WalkaboutTextGenerator*)walkaboutGame->getText()->getTextGenerator();
	delete walkaboutButtonHandler;
	delete walkaboutKeyHandler;
	delete walkaboutGame;

	// AND RETURN
	return 0;
}