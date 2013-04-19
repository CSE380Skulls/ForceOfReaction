#include "stdafx.h"

// Walkabout GAME INCLUDES
#include "src\WalkaboutButtonEventHandler.h"
#include "src\WalkaboutDataLoader.h"
#include "src\WalkaboutGame.h"
#include "src\WalkaboutKeyEventHandler.h"
#include "src\WalkaboutTextGenerator.h"

// GAME OBJECT INCLUDES
#include "src\game\Game.h"
#include "src\graphics\GameGraphics.h"
#include "src\gsm\ai\bots\RandomJumpingBot.h"
#include "src\gsm\state\GameState.h"
#include "src\gsm\world\TiledLayer.h"
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

// TMX MAP LOADING
#include "src\TMXMapImporter.h"

// ANIMATED SPRITE TYPE LOADING
#include "src\PoseurSpriteTypesImporter.h"

// LUA
#include "LuaPlus.h"
using namespace LuaPlus;

/*
	loadGame - This method loads the setup game data into the game and
	constructs all the needed objects for the game to work.
*/
void WalkaboutDataLoader::loadGame(Game *game, wstring gameInitFile)
{
	// Init Lua
	LuaState* luaPState = LuaState::Create();

	// Open the Lua Script File
	int result = luaPState->DoFile(GAME_INIT_FILE);
	
	// name
	LuaObject obj = luaPState->GetGlobal("title");
	string title = obj.GetString();

	// screen width
	obj = luaPState->GetGlobal("screen_width");
	int screen_width = obj.GetInteger();

	// screen height
	obj = luaPState->GetGlobal("screen_height");
	int screen_height = obj.GetInteger();

	//full screen 
	obj = luaPState->GetGlobal("fullscreen_mode");
	bool fullscreen = obj.GetBoolean();

	// Font size
	obj = luaPState->GetGlobal("font_size");
	int font_size = obj.GetInteger();

	// x offset
	obj = luaPState->GetGlobal("viewport_offset_x");
	int viewport_offset_x = obj.GetInteger();

	// y offset
	obj = luaPState->GetGlobal("viewport_offset_y");
	int viewport_offset_y = obj.GetInteger();

	// font red
	obj = luaPState->GetGlobal("font_color_red");
	int font_color_red = obj.GetInteger();

	// font green
	obj = luaPState->GetGlobal("font_color_green");
	int font_color_green = obj.GetInteger();

	// font blue
	obj = luaPState->GetGlobal("font_color_blue");
	int font_color_blue = obj.GetInteger();
	
	// key red
	obj = luaPState->GetGlobal("color_key_red");
	int color_key_red = obj.GetInteger();

	// key green
	obj = luaPState->GetGlobal("color_key_green");
	int color_key_green = obj.GetInteger();

	// key blue
	obj = luaPState->GetGlobal("color_key_blue");
	int color_key_blue = obj.GetInteger();


	// AND LET'S READ IN THE GAME SETUP INFO
	// FIRST LOAD ALL THE PROPERTIES
	//map<wstring,wstring> *properties = new map<wstring,wstring>();
	//loadGameProperties(game, properties, gameInitFile);

	// WE NEED THE TITLE AND USE_FULLSCREEN_MODE TO INITIALIZE OUR WINDOW
	//wstring titleProp = (*properties)[W_TITLE];
	//wstring useFullscreenProp = (*properties)[W_USE_FULLSCREEN_MODE];
	//bool useFullscreen = false;
	//if (useFullscreenProp.compare(L"true") == 0)
	//	useFullscreen = true;

	// GET THE SCREEN WIDTH AND HEIGHT
	//int screenWidth, screenHeight;
	//wstring screenWidthProp = (*properties)[W_SCREEN_WIDTH];
	//wstring screenHeightProp = (*properties)[W_SCREEN_HEIGHT];
	//wstringstream(screenWidthProp) >> screenWidth;
	//wstringstream(screenHeightProp) >> screenHeight;

	// MAKE A CUSTOM GameOS OBJECT (WindowsOS) FOR SOME WINDOWS
	// PLATFORM STUFF, INCLUDING A Window OF COURSE
	/*WindowsOS *walkaboutOS = new WindowsOS(	hInstance, 
										nCmdShow,
										useFullscreen,
										titleProp,
										screenWidth, screenHeight,
										game);
	*/
	
	std::wstring wsTmp(title.begin(), title.end());

	WindowsOS *walkaboutOS = new WindowsOS(	hInstance, 
										nCmdShow,
										fullscreen,
										wsTmp,
										screen_width, screen_height,
										game);

	//int textFontSize;
	//wstring textFontSizeProp = (*properties)[W_TEXT_FONT_SIZE];
	//wstringstream(textFontSizeProp) >> textFontSize;

	// RENDERING WILL BE DONE USING DirectX
	DirectXGraphics *walkaboutGraphics = new DirectXGraphics(game);
	/*walkaboutGraphics->init(screenWidth, screenHeight);
	walkaboutGraphics->initGraphics(walkaboutOS, useFullscreen);
	walkaboutGraphics->initTextFont(textFontSize);
	*/
	walkaboutGraphics->init(screen_width, screen_height);
	walkaboutGraphics->initGraphics(walkaboutOS, fullscreen);
	walkaboutGraphics->initTextFont(font_size);

	// AND NOW LOAD THE COLORS THE GRAPHICS WILL USE
	// AS A COLOR KEY AND FOR RENDERING TEXT
	//initColors(walkaboutGraphics, properties);

	// COLOR USED FOR RENDERING TEXT
	walkaboutGraphics->setFontColor(font_color_red, font_color_green, font_color_blue);
	// COLOR KEY - COLOR TO BE IGNORED WHEN LOADING AN IMAGE
	// NOTE, IF YOU WISH TO USE PNG IMAGES THAT CONTAIN ALPHA
	// CHANNEL DATA, YOU DON'T NEED A COLOR KEY
	walkaboutGraphics->setColorKey(color_key_red, color_key_green, color_key_blue);

	// WE'LL USE WINDOWS PLATFORM METHODS FOR GETTING INPUT
	WindowsInput *walkaboutInput = new WindowsInput();

	// AND WINDOWS FOR THE TIMER AS WELL
	WindowsTimer *walkaboutTimer = new WindowsTimer();

	// NOW INITIALIZE THE Game WITH ALL THE
	// PLATFORM AND GAME SPECIFIC DATA WE JUST CREATED
	game->initPlatformPlugins(	(GameGraphics*)walkaboutGraphics,
								(GameInput*)walkaboutInput,
								(GameOS*)walkaboutOS,
								(GameTimer*)walkaboutTimer);

	// LOAD OUR CUSTOM TEXT GENERATOR, WHICH DRAWS
	// TEXT ON THE SCREEN EACH FRAME
	WalkaboutTextGenerator *walkaboutTextGenerator = new WalkaboutTextGenerator();
	walkaboutTextGenerator->initText(game);
	GameText *text = game->getText();
	text->initDebugFile(W_DEBUG_FILE);
	text->setTextGenerator((TextGenerator*)walkaboutTextGenerator);

	// INIT THE VIEWPORT TOO
	//initViewport(game->getGUI(), properties);
	int viewportWidth = screen_width - viewport_offset_x;
	int viewportHeight = screen_height - viewport_offset_y;

	Viewport *viewport = game->getGUI()->getViewport();
	viewport->setViewportWidth(viewportWidth);
	viewport->setViewportHeight(viewportHeight);
	viewport->setViewportOffsetX(viewport_offset_x);
	viewport->setViewportOffsetY(viewport_offset_y);

	// WE DON'T NEED THE PROPERTIES MAP ANYMORE, THE GAME IS ALL LOADED
	//delete properties;
}

/*
	initColors - this helper method loads the color key, used for loading
	images, and the font color, used for rendering text.
*/
void WalkaboutDataLoader::initColors(	GameGraphics *graphics,
									map<wstring,wstring> *properties)
{
	int fontRed, fontGreen, fontBlue;
	wstring fontRedProp = (*properties)[W_FONT_COLOR_RED];
	wstring fontGreenProp = (*properties)[W_FONT_COLOR_GREEN];
	wstring fontBlueProp = (*properties)[W_FONT_COLOR_BLUE];
	wstringstream(fontRedProp) >> fontRed;
	wstringstream(fontGreenProp) >> fontGreen;
	wstringstream(fontBlueProp) >> fontBlue;

	// COLOR USED FOR RENDERING TEXT
	graphics->setFontColor(fontRed, fontGreen, fontBlue);

	int keyRed, keyGreen, keyBlue;
	wstring keyRedProp = (*properties)[W_COLOR_KEY_RED];
	wstring keyGreenProp = (*properties)[W_COLOR_KEY_GREEN];
	wstring keyBlueProp = (*properties)[W_COLOR_KEY_BLUE];
	wstringstream(keyRedProp) >> keyRed;
	wstringstream(keyGreenProp) >> keyGreen;
	wstringstream(keyBlueProp) >> keyBlue;

	// COLOR KEY - COLOR TO BE IGNORED WHEN LOADING AN IMAGE
	// NOTE, IF YOU WISH TO USE PNG IMAGES THAT CONTAIN ALPHA
	// CHANNEL DATA, YOU DON'T NEED A COLOR KEY
	graphics->setColorKey(keyRed, keyGreen, keyBlue);
}

/*
	loadGUI - This method loads all the GUI assets described in the guiInitFile
	argument. Note that we are loading all GUI art for all GUIs when the application
	first starts. We'll learn a better technique later in the semester.
*/
void WalkaboutDataLoader::loadGUI(Game *game, wstring guiInitFile)
{
	// WE'RE JUST GOING TO IGNORE THE GUI FILE FOR NOW.
	// FOR THE MOMENT WE ARE CALLING THIS HARD-CODED GUI LOADER
	hardCodedLoadGUIExample(game);
}

/*
	loadLevel - This method should load the data the level described by the
	levelInitFile argument in to the Game's game state manager.
*/
void WalkaboutDataLoader::loadWorld(Game *game, wstring levelInitFile)	
{
	// LOAD THE LEVEL'S BACKGROUND TILES
	TMXMapImporter tmxMapImporter;
	tmxMapImporter.loadWorld(game, W_LEVEL_1_DIR, W_LEVEL_1_NAME);

	//add all of the items to the physics system
	game->getGSM()->getWorld()->initWorldPhysicsSystem(game);

	// LOAD THE LEVEL'S SPRITE IMAGES
	PoseurSpriteTypesImporter psti;
	psti.loadSpriteTypes(game, SPRITE_TYPES_LIST);

	// LET'S MAKE A PLAYER SPRITE
	// @TODO - LATER WE'LL LOAD ALL LEVEL DATA FROM A FILE
	GameStateManager *gsm = game->getGSM();
	Physics *physics = gsm->getPhysics();
	physics->setGravity(W_GRAVITY);
	SpriteManager *spriteManager = gsm->getSpriteManager();
	AnimatedSprite *player = spriteManager->getPlayer();
	physics->addCollidableObject(player);

	// NOTE THAT RED BOX MAN IS SPRITE ID 2
	AnimatedSpriteType *playerSpriteType = spriteManager->getSpriteType(0);
	player->setSpriteType(playerSpriteType);
	player->setAlpha(255);
	//player->setCurrentState(IDLE);
	player->setCurrentState(IDLE_RIGHT);
	PhysicalProperties *playerProps = player->getPhysicalProperties();
	playerProps->setX(PLAYER_INIT_X);
	playerProps->setY(PLAYER_INIT_Y);
	playerProps->setVelocity(0.0f, 0.0f);
	playerProps->setAccelerationX(0);
	playerProps->setAccelerationY(0);
	player->setOnTileThisFrame(false);
	player->setOnTileLastFrame(false);
	player->affixTightAABBBoundingVolume();

	float extent_x = player->getSpriteType()->getTextureWidth()/2.0f;
	float extent_y = player->getSpriteType()->getTextureHeight()/2.0f;
	game->getGSM()->getBoxPhysics()->createDynamicBox(game,player,player,
					PLAYER_INIT_X + extent_x,PLAYER_INIT_Y + extent_y, extent_x, extent_y);

	//This is the test box2d Sprite
	//AnimatedSprite *test_sprite = spriteManager->getTestSprite();
	//AnimatedSpriteType *test_spriteType= spriteManager->getSpriteType(4);
	//test_sprite->setSpriteType(test_spriteType);
	//test_sprite->setAlpha(255);
	////player->setCurrentState(IDLE);
	//test_sprite->setCurrentState(IDLE_RIGHT);
	//PhysicalProperties *testProps = test_sprite->getPhysicalProperties();
	//testProps->setX(PLAYER_INIT_X);
	//testProps->setY(PLAYER_INIT_Y - 300);
	//test_sprite->setOnTileThisFrame(false);
	//test_sprite->setOnTileLastFrame(false);
	//test_sprite->affixTightAABBBoundingVolume();

	/*	set the box2dBody; note this is seriously a test, make this entire sprite
		creation business into a factory class or function to do this.
	*/
	/*float extent_x = test_sprite->getSpriteType()->getTextureWidth()/2;
	float extent_y = test_sprite->getSpriteType()->getTextureHeight()/2;*/

	/*game->getGSM()->getBoxPhysics()->createDynamicBox(game,test_sprite,test_sprite,
					PLAYER_INIT_X + extent_x,(PLAYER_INIT_Y - 300) + extent_y, extent_x/2, extent_y);*/

	AnimatedSpriteType *botSpriteType = spriteManager->getSpriteType(1);
	//AnimatedSpriteType *botSpriteType = spriteManager->getSpriteType(1);
	// AND LET'S ADD A BUNCH OF RANDOM JUMPING BOTS, FIRST ALONG
	// A LINE NEAR THE TOP
	RandomJumpingBot *bot = new RandomJumpingBot(physics, 30, 120, 40);
	bot->setSpriteType(botSpriteType);
	bot->setAlpha(255);
	//bot->setCurrentState(IDLE);
	bot->setCurrentState(IDLE_RIGHT);
	PhysicalProperties *botProps = bot->getPhysicalProperties();
	botProps->setX(PLAYER_INIT_X+100);
	botProps->setY(PLAYER_INIT_Y);
	botProps->setVelocity(0.0f, 0.0f);
	botProps->setAccelerationX(0);
	botProps->setAccelerationY(0);
	bot->setOnTileThisFrame(false);
	bot->setOnTileLastFrame(false);
	bot->affixTightAABBBoundingVolume();

	extent_x = bot->getSpriteType()->getTextureWidth()/2.0f;
	extent_y = bot->getSpriteType()->getTextureHeight()/2.0f; 
	game->getGSM()->getBoxPhysics()->createDynamicBox(game,bot,bot,
					PLAYER_INIT_X + 100 + extent_x,PLAYER_INIT_Y + extent_y, extent_x, extent_y);
	physics->addCollidableObject(bot);
	spriteManager->addBot(bot);
// UNCOMMENT THE FOLLOWING CODE BLOCK WHEN YOU ARE READY TO ADD SOME BOTS
/*	for (int i = 2; i <= 26; i++)
	{
		float botX = 400.0f + (i * 100.0f);
		float botY = 100.0f;
		makeRandomJumpingBot(game, botSpriteType, botX, botY);
	}

	// AND THEN STRATEGICALLY PLACED AROUND THE LEVEL
	makeRandomJumpingBot(game, botSpriteType, 400, 100);
	makeRandomJumpingBot(game, botSpriteType, 200, 400);
	makeRandomJumpingBot(game, botSpriteType, 400, 400);
	makeRandomJumpingBot(game, botSpriteType, 800, 700);
	makeRandomJumpingBot(game, botSpriteType, 900, 700);
	makeRandomJumpingBot(game, botSpriteType, 1000, 700);
	makeRandomJumpingBot(game, botSpriteType, 100, 1000);
	makeRandomJumpingBot(game, botSpriteType, 300, 1000);	
	makeRandomJumpingBot(game, botSpriteType, 500, 1000);
	makeRandomJumpingBot(game, botSpriteType, 100, 1400);
	makeRandomJumpingBot(game, botSpriteType, 400, 1400);	
	makeRandomJumpingBot(game, botSpriteType, 700, 1400);

	// AND THEN A BUNCH LINED UP NEAR THE LEVEL EXIT
	for (int i = 0; i < 14; i++)
		makeRandomJumpingBot(game, botSpriteType, 1700.0f + (i*100.0f), 1300.0f);
*/		
	game->getGSM()->goToGame();
}

void WalkaboutDataLoader::makeRandomJumpingBot(Game *game, AnimatedSpriteType *randomJumpingBotType, float initX, float initY)
{
	/*
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	Physics *physics = game->getGSM()->getPhysics();
	RandomJumpingBot *bot = new RandomJumpingBot(physics, 30, 120, 40);
	physics->addCollidableObject(bot);
	PhysicalProperties *pp = bot->getPhysicalProperties();
	pp->setPosition(initX, initY);
	bot->setSpriteType(randomJumpingBotType);
	bot->setCurrentState(JUMPING);
	bot->setAlpha(255);
	spriteManager->addBot(bot);
	bot->affixTightAABBBoundingVolume();
	*/
}

/*
	initWalkaboutGUI - This method builds a GUI for the Walkabout Game application.
	Note that we load all the GUI components from this method, including
	the ScreenGUI with Buttons and Overlays and the Cursor.
*/
void WalkaboutDataLoader::hardCodedLoadGUIExample(Game *game)
{
	GameGUI *gui = game->getGUI();
	GameGraphics *graphics = game->getGraphics();
	DirectXTextureManager *guiTextureManager = (DirectXTextureManager*)graphics->getGUITextureManager();

	// SETUP THE CURSOR VIA OUR HELPER METHOD
	initCursor(gui, guiTextureManager);
	initSplashScreen(game, gui, guiTextureManager);
	initMainMenu(gui, guiTextureManager);
	initOptions(gui, guiTextureManager);
	initCredits(gui, guiTextureManager);
	initInGameGUI(gui, guiTextureManager);
	initLoading(gui, guiTextureManager);
	initPaused(gui, guiTextureManager);
}

/*
	initCursor - initializes a simple little cursor for the gui.
*/
void WalkaboutDataLoader::initCursor(GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// SETUP THE CURSOR
	vector<unsigned int> *imageIDs = new vector<unsigned int>();
	int imageID;

	// - FIRST LOAD THE GREEN CURSOR IMAGE
	//imageID = guiTextureManager->loadTexture(W_GREEN_CURSOR_PATH);
	//imageIDs->push_back(imageID);

	// - AND NOW THE RED ONE
	//imageID = guiTextureManager->loadTexture(W_RED_CURSOR_PATH);
	//imageIDs->push_back(imageID);

	imageID = guiTextureManager->loadTexture(W_CURSOR_PATH);
	imageIDs->push_back(imageID);

	// - NOW BUILD AND LOAD THE CURSOR
	Cursor *cursor = new Cursor();
	cursor->initCursor(	imageIDs,
						*(imageIDs->begin()),
						0,
						0,
						0,
						255,
						32,
						32);
	gui->setCursor(cursor);
}

/*
	initSplashScreen - initializes the game's splash screen gui.
*/
void WalkaboutDataLoader::initSplashScreen(Game *game, GameGUI *gui,	DirectXTextureManager *guiTextureManager)
{
	// NOW, FIRST LET'S ADD A SPLASH SCREEN GUI
	ScreenGUI *splashScreenGUI = new ScreenGUI();

	// WE'LL ONLY HAVE ONE IMAGE FOR OUR GIANT BUTTON
	unsigned int normalTextureID = guiTextureManager->loadTexture(W_SPLASH_SCREEN_PATH);
	unsigned int mouseOverTextureID = normalTextureID;

	// INIT THE QUIT BUTTON
	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							0,
							0,
							0,
							255,
							game->getGraphics()->getScreenWidth(),
							game->getGraphics()->getScreenHeight(),
							false,
							W_GO_TO_MM_COMMAND);
	splashScreenGUI->addButton(buttonToAdd);

	// AND REGISTER IT WITH THE GUI
	gui->addScreenGUI(GS_SPLASH_SCREEN, splashScreenGUI);
}

void WalkaboutDataLoader::initPaused(GameGUI *gui,	DirectXTextureManager *guiTextureManager)
{
	// NOW, FIRST LET'S ADD A SPLASH SCREEN GUI
	ScreenGUI *pausedGUI = new ScreenGUI();

	unsigned int imageID = guiTextureManager->loadTexture(W_PAUSED_MENU_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	pausedGUI->addOverlayImage(imageToAdd);

	// WE'LL ONLY HAVE ONE IMAGE FOR OUR GIANT BUTTON
	unsigned int normalTextureID = guiTextureManager->loadTexture(W_EXIT_IMAGE_PATH);
	unsigned int mouseOverTextureID = guiTextureManager->loadTexture(W_EXIT_IMAGE_MO_PATH);

	// INIT THE QUIT BUTTON
	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							310,
							500,
							0,
							255,
							378,
							80,
							false,
							W_QUIT_COMMAND);
	pausedGUI->addButton(buttonToAdd);

	normalTextureID = guiTextureManager->loadTexture(W_RETURN_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_RETURN_IMAGE_MO_PATH);

	// INIT THE QUIT BUTTON
	buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							310,
							350,
							0,
							255,
							378,
							80,
							false,
							W_RETURN_COMMAND);
	pausedGUI->addButton(buttonToAdd);

	// AND REGISTER IT WITH THE GUI
	gui->addScreenGUI(GS_PAUSED, pausedGUI);
}

/*
	initMainMenu - initializes the game's main menu gui.
*/
void WalkaboutDataLoader::initMainMenu(GameGUI *gui,	DirectXTextureManager *guiTextureManager)
{
	// NOW LET'S LOAD A MAIN MENU GUI SCREEN
	ScreenGUI *mainMenuGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture(W_MAIN_MENU_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	mainMenuGUI->addOverlayImage(imageToAdd);

	// AND LET'S ADD AN EXIT BUTTON
	Button *buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_PATH);
	int mouseOverTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_MO_PATH);

	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							325,
							570,
							0,
							255,
							378,
							80,
							false,
							W_EXIT_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd);

	// AND LET'S ADD A START BUTTON
	buttonToAdd = new Button();
	normalTextureID = guiTextureManager->loadTexture(W_OPTIONS_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_OPTIONS_IMAGE_MO_PATH);

	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							325,
							390,
							0,
							255,
							378,
							80,
							false,
							W_HELP_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd);

	// AND LET'S ADD A START BUTTON
	buttonToAdd = new Button();
	normalTextureID = guiTextureManager->loadTexture(W_CREDITS_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_CREDITS_IMAGE_MO_PATH);

	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							325,
							480,
							0,
							255,
							378,
							80,
							false,
							W_CREDITS_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd);

	// AND LET'S ADD A START BUTTON
	buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs

	normalTextureID = guiTextureManager->loadTexture(W_START_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_START_IMAGE_MO_PATH);

	// - INIT THE START BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							325,
							300,
							0,
							255,
							378,
							80,
							false,
							W_START_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_MAIN_MENU,		mainMenuGUI);
}
void WalkaboutDataLoader::initLoading(GameGUI *gui,	DirectXTextureManager *guiTextureManager)
{
	// NOW, FIRST LET'S ADD A SPLASH SCREEN GUI
	ScreenGUI *loadingGUI = new ScreenGUI();

	// WE'LL ONLY HAVE ONE IMAGE FOR OUR GIANT BUTTON
	unsigned int imageID = guiTextureManager->loadTexture(W_LOADING_SCREEN_PATH);

	// INIT THE QUIT BUTTON
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	loadingGUI->addOverlayImage(imageToAdd);

	// AND REGISTER IT WITH THE GUI
	gui->addScreenGUI(GS_STARTING_LEVEL, loadingGUI);
}
void WalkaboutDataLoader::initOptions(GameGUI *gui,	DirectXTextureManager *guiTextureManager)
{
	// NOW, FIRST LET'S ADD A SPLASH SCREEN GUI
	ScreenGUI *optionsGUI = new ScreenGUI();

	// WE'LL ONLY HAVE ONE IMAGE FOR OUR GIANT BUTTON
	unsigned int normalTextureID = guiTextureManager->loadTexture(W_OPTION_SCREEN_PATH);
	unsigned int mouseOverTextureID = normalTextureID;

	// INIT THE QUIT BUTTON
	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							0,
							0,
							0,
							255,
							1024,
							768,
							false,
							W_GO_TO_MM_COMMAND);
	optionsGUI->addButton(buttonToAdd);

	// AND REGISTER IT WITH THE GUI
	gui->addScreenGUI(GS_MENU_HELP_MENU, optionsGUI);
}
void WalkaboutDataLoader::initCredits(GameGUI *gui,	DirectXTextureManager *guiTextureManager)
{
	// NOW, FIRST LET'S ADD A SPLASH SCREEN GUI
	ScreenGUI *creditsGUI = new ScreenGUI();

	// WE'LL ONLY HAVE ONE IMAGE FOR OUR GIANT BUTTON
	unsigned int normalTextureID = guiTextureManager->loadTexture(W_CREDITS_SCREEN_PATH);
	unsigned int mouseOverTextureID = normalTextureID;

	// INIT THE QUIT BUTTON
	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							0,
							0,
							0,
							255,
							1024,
							768,
							false,
							W_GO_TO_MM_COMMAND);
	creditsGUI->addButton(buttonToAdd);

	// AND REGISTER IT WITH THE GUI
	gui->addScreenGUI(GS_MENU_ABOUT_MENU, creditsGUI);
}
/*
	initInGameGUI - initializes the game's in-game gui.
*/
void WalkaboutDataLoader::initInGameGUI(GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// NOW ADD THE IN-GAME GUI
	ScreenGUI *inGameGUI = new ScreenGUI();

	unsigned int imageID = guiTextureManager->loadTexture(W_STATUS_HEAD_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 20;
	imageToAdd->y = 20;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 64;
	imageToAdd->height = 64;
	imageToAdd->imageID = imageID;
	inGameGUI->addOverlayImage(imageToAdd);

	imageID = guiTextureManager->loadTexture(W_STATUS_FIRE_PATH);
	imageToAdd = new OverlayImage();
	imageToAdd->x = 84;
	imageToAdd->y = 15;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 340;
	imageToAdd->height = 22;
	imageToAdd->imageID = imageID;
	inGameGUI->addOverlayImage(imageToAdd);

	imageID = guiTextureManager->loadTexture(W_STATUS_WATER_PATH);
	imageToAdd = new OverlayImage();
	imageToAdd->x = 84;
	imageToAdd->y = 40;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 340;
	imageToAdd->height = 22;
	imageToAdd->imageID = imageID;
	inGameGUI->addOverlayImage(imageToAdd);

	imageID = guiTextureManager->loadTexture(W_STATUS_EARTH_PATH);
	imageToAdd = new OverlayImage();
	imageToAdd->x = 84;
	imageToAdd->y = 67;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 340;
	imageToAdd->height = 22;
	imageToAdd->imageID = imageID;
	inGameGUI->addOverlayImage(imageToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_GAME_IN_PROGRESS,	inGameGUI);
}

/*
	initViewport - initializes the game's viewport.
*/
void WalkaboutDataLoader::initViewport(GameGUI *gui, map<wstring,wstring> *properties)
{
	// AND NOW SPECIFY THE VIEWPORT SIZE AND LOCATION. NOTE THAT IN THIS EXAMPLE,
	// WE ARE PUTTING A TOOLBAR WITH A BUTTON ACCROSS THE NORTH OF THE APPLICATION.
	// THAT TOOLBAR HAS A HEIGHT OF 64 PIXELS, SO WE'LL MAKE THAT THE OFFSET FOR
	// THE VIEWPORT IN THE Y AXIS
	Viewport *viewport = gui->getViewport();

	int viewportOffsetX, viewportOffsetY, screenWidth, screenHeight;
	wstring viewportOffsetXProp = (*properties)[W_VIEWPORT_OFFSET_X];
	wstring viewportOffsetYProp = (*properties)[W_VIEWPORT_OFFSET_Y];
	wstring screenWidthProp = (*properties)[W_SCREEN_WIDTH];
	wstring screenHeightProp = (*properties)[W_SCREEN_HEIGHT];
	wstringstream(viewportOffsetXProp) >> viewportOffsetX;
	wstringstream(viewportOffsetYProp) >> viewportOffsetY;
	wstringstream(screenWidthProp) >> screenWidth;
	wstringstream(screenHeightProp) >> screenHeight;
	int viewportWidth = screenWidth - viewportOffsetX;
	int viewportHeight = screenHeight - viewportOffsetY;
	viewport->setViewportWidth(viewportWidth);
	viewport->setViewportHeight(viewportHeight);
	viewport->setViewportOffsetX(viewportOffsetX);
	viewport->setViewportOffsetY(viewportOffsetY);
}