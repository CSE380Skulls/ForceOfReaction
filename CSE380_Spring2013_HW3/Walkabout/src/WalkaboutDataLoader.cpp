#include "stdafx.h"

// Walkabout GAME INCLUDES
#include "src\WalkaboutButtonEventHandler.h"
#include "src\WalkaboutDataLoader.h"
#include "src\WalkaboutGame.h"
#include "src\WalkaboutKeyEventHandler.h"
#include "src\WalkaboutTextGenerator.h"
#include "src\FORFloatingBot.h"
#include "src\MonkeyBot.h"
#include "src\BreakableWall.h"
#include "src\CutsceneTrigger.h"
#include "src\BossBot.h"
#include "src\FORPlayer.h"
#include "src\Vine.h"
#include "src\Spikes.h"
#include "src\Switch.h"

// GAME OBJECT INCLUDES
#include "src\game\Game.h"
#include "src\graphics\GameGraphics.h"
#include "src\audio\GameAudioManager.h"
#include "src\gsm\sprite\AnimatedSprite.h"
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
	string title = luaPState->GetGlobal(LUA_TITLE).GetString();
	// screen width
	int screen_width = luaPState->GetGlobal(LUA_SCREEN_WIDTH).GetInteger();
	// screen height
	int screen_height = luaPState->GetGlobal(LUA_SCREEN_HEIGHT).GetInteger();
	//full screen
	bool fullscreen = luaPState->GetGlobal(LUA_FULLSCREEN_MODE).GetBoolean();
	// Font size
	int font_size = luaPState->GetGlobal(LUA_FONT_SIZE).GetInteger();
	// x offset
	int viewport_offset_x = luaPState->GetGlobal(LUA_VIEWPORT_OFFSET_X).GetInteger();
	// y offset
	int viewport_offset_y = luaPState->GetGlobal(LUA_VIEWPORT_OFFSET_Y).GetInteger();
	// font red
	int font_color_red =luaPState->GetGlobal(LUA_FONT_COLOR_RED).GetInteger();
	// font green
	int font_color_green = luaPState->GetGlobal(LUA_FONT_COLOR_GREEN).GetInteger();
	// font blue
	int font_color_blue = luaPState->GetGlobal(LUA_FONT_COLOR_BLUE).GetInteger();
	// key red
	int color_key_red = luaPState->GetGlobal(LUA_COLOR_KEY_RED).GetInteger();
	// key green
	int color_key_green = luaPState->GetGlobal(LUA_COLOR_KEY_GREEN).GetInteger();
	// key blue
	int color_key_blue = luaPState->GetGlobal(LUA_COLOR_KEY_BLUE).GetInteger();
	
	// Convert title to wstring
	std::wstring wsTmp(title.begin(), title.end());

	// Initialize windowsOS
	WindowsOS *walkaboutOS = new WindowsOS(	hInstance, 
										nCmdShow,
										fullscreen,
										wsTmp,
										screen_width, screen_height,
										game);

	// RENDERING WILL BE DONE USING DirectX
	DirectXGraphics *walkaboutGraphics = new DirectXGraphics(game);
	walkaboutGraphics->init(screen_width, screen_height);
	walkaboutGraphics->initGraphics(walkaboutOS, fullscreen);
	walkaboutGraphics->initTextFont(font_size);

	// AND NOW LOAD THE COLORS THE GRAPHICS WILL USE
	// AS A COLOR KEY AND FOR RENDERING TEXT

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

	// Initialize the player
	GameStateManager *gsm = game->getGSM();
	SpriteManager *spriteManager = gsm->getSpriteManager();
	FORPlayer *player = new FORPlayer(PLAYER_ATTACK_COOLDOWN, PLAYER_DEATH_COOLDOWN, PLAYER_DESIGNATION);
	spriteManager->setPlayer(player);
	player->setHitPoints(PLAYER_HITPOINTS);
	player->setDamage(0);
	player->setAlpha(255);
}
/*
	loadSounds - This method loads all the GUI assets described in the guiInitFile
	argument. Note that we are loading all GUI art for all GUIs when the application
	first starts. We'll learn a better technique later in the semester.
*/
void WalkaboutDataLoader::loadAudio(Game *game, wstring audioInitFile)
{
	// FOR THE MOMENT WE ARE CALLING THIS HARD-CODED AUDIO LOADER
	hardCodedLoadAudioExample(game);
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
	//@TODO: ADD THIRD LEVEL
	if(levelInitFile.compare(W_LEVEL_1_NAME) == 0 ) {
		loadLevel1(game);
	}
	else if(levelInitFile.compare(W_LEVEL_2_NAME) == 0 ) {
		loadLevel2(game);
	}
	else if(levelInitFile.compare(W_LEVEL_3_NAME) == 0 ) {
		loadLevel3(game);
	}
	else {
		game->getGSM()->goToMainMenu();
	}
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
	initLevelWon(gui, guiTextureManager);
	initGameOver(gui, guiTextureManager);
	initCutscene(game, gui, guiTextureManager);
}
/*
	loadSounds - This method loads all the GUI assets described in the guiInitFile
	argument. Note that we are loading all GUI art for all GUIs when the application
	first starts. We'll learn a better technique later in the semester.
*/
void WalkaboutDataLoader::hardCodedLoadAudioExample(Game* game)
{
	GameAudioManager *gam = game->getGAM();
	gam->addSound(C_INTRO);
	gam->addSound(C_LEVEL1);
	gam->addSound(C_LEVEL2);
	gam->addSound(C_LEVEL3);
	gam->addSound(C_EXPLOSION);
	gam->addSound(C_EXPLOSION2);
	gam->addSound(C_DEATH);
	gam->addSound(C_HIT);
	gam->addSound(C_JUMP);
	gam->addSound(C_MENUBUTTON);
	gam->addSound(C_PLAYERHIT);
	gam->addSound(C_SWING);
	gam->addSound(C_SWOOSHTALK);

	gam->initMusic(C_INTRO);
}
/*
	initCursor - initializes a simple little cursor for the gui.
*/
void WalkaboutDataLoader::initCursor(GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// SETUP THE CURSOR
	vector<unsigned int> *imageIDs = new vector<unsigned int>();
	int imageID;

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


void WalkaboutDataLoader::initLevelWon(GameGUI *gui,	DirectXTextureManager *guiTextureManager)
{
	// NOW, FIRST LET'S ADD A SPLASH SCREEN GUI
	ScreenGUI *levelWonGUI = new ScreenGUI();

	unsigned int imageID = guiTextureManager->loadTexture(W_LEVEL_WON_SCREEN_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	levelWonGUI->addOverlayImage(imageToAdd);
	/*
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
	levelWonGUI->addButton(buttonToAdd);
	*/
	unsigned int normalTextureID = guiTextureManager->loadTexture(W_NEXT_LEVEL_IMAGE_PATH);
	unsigned int mouseOverTextureID = guiTextureManager->loadTexture(W_NEXT_LEVEL_IMAGE_MO_PATH);

	// INIT THE QUIT BUTTON
	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							310,
							350,
							0,
							255,
							378,
							80,
							false,
							W_NEXT_LEVEL_COMMAND);
	levelWonGUI->addButton(buttonToAdd);

	// AND REGISTER IT WITH THE GUI
	gui->addScreenGUI(GS_LEVEL_WON, levelWonGUI);
	//gui->addScreenGUI(GS_PAUSED, levelWonGUI);
}

void WalkaboutDataLoader::initGameOver(GameGUI *gui,	DirectXTextureManager *guiTextureManager)
{
	// NOW, FIRST LET'S ADD A SPLASH SCREEN GUI
	ScreenGUI *gameOverGUI = new ScreenGUI();

	unsigned int imageID = guiTextureManager->loadTexture(W_GAME_OVER_SCREEN_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	gameOverGUI->addOverlayImage(imageToAdd);

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
	gameOverGUI->addButton(buttonToAdd);

	// RESTART BUTTON
	normalTextureID = guiTextureManager->loadTexture(W_RESTART_LEVEL_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_RESTART_LEVEL_MO_PATH);

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
							W_RESTART_LEVEL_COMMAND);
	gameOverGUI->addButton(buttonToAdd);

	// AND REGISTER IT WITH THE GUI
	gui->addScreenGUI(GS_GAME_OVER, gameOverGUI);
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

	// - INIT THE CREDITS BUTTON
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

	guiTextureManager->loadTexture(W_STATUS_HP_9_PATH);
	guiTextureManager->loadTexture(W_STATUS_HP_8_PATH);
	guiTextureManager->loadTexture(W_STATUS_HP_7_PATH);
	guiTextureManager->loadTexture(W_STATUS_HP_6_PATH);
	guiTextureManager->loadTexture(W_STATUS_HP_5_PATH);
	guiTextureManager->loadTexture(W_STATUS_HP_4_PATH);
	guiTextureManager->loadTexture(W_STATUS_HP_3_PATH);
	guiTextureManager->loadTexture(W_STATUS_HP_2_PATH);
	guiTextureManager->loadTexture(W_STATUS_HP_1_PATH);
	guiTextureManager->loadTexture(W_STATUS_HP_0_PATH);
	guiTextureManager->loadTexture(W_STATUS_EARTH_SELECTED_PATH);
	guiTextureManager->loadTexture(W_STATUS_WATER_SELECTED_PATH);
	guiTextureManager->loadTexture(W_STATUS_FIRE_SELECTED_PATH);

	unsigned int imageID = guiTextureManager->loadTexture(W_STATUS_HEAD_PATH);
	OverlayImage* imageToAdd = new OverlayImage();
	imageToAdd->x = 20;
	imageToAdd->y = 20;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 64;
	imageToAdd->height = 64;
	imageToAdd->imageID = imageID;
	inGameGUI->addOverlayImage(imageToAdd);
	
	imageID = guiTextureManager->loadTexture(W_STATUS_HP_10_PATH);
	imageToAdd = new OverlayImage();
	imageToAdd->x = 84;
	imageToAdd->y = 15;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 320;
	imageToAdd->height = 22;
	imageToAdd->imageID = imageID;
	inGameGUI->addOverlayImage(imageToAdd);

	imageID = guiTextureManager->loadTexture(W_STATUS_EARTH_UNSELECTED_PATH);
	imageToAdd = new OverlayImage();
	imageToAdd->x = 110;
	imageToAdd->y = 50;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 48;
	imageToAdd->height = 48;
	imageToAdd->imageID = imageID;
	inGameGUI->addOverlayImage(imageToAdd);

	imageID = guiTextureManager->loadTexture(W_STATUS_WATER_UNSELECTED_PATH);
	imageToAdd = new OverlayImage();
	imageToAdd->x = 200;
	imageToAdd->y = 50;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 48;
	imageToAdd->height = 48;
	imageToAdd->imageID = imageID;
	inGameGUI->addOverlayImage(imageToAdd);

	imageID = guiTextureManager->loadTexture(W_STATUS_FIRE_UNSELECTED_PATH);
	imageToAdd = new OverlayImage();
	imageToAdd->x = 290;
	imageToAdd->y = 50;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 48;
	imageToAdd->height = 48;
	imageToAdd->imageID = imageID;
	inGameGUI->addOverlayImage(imageToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_GAME_IN_PROGRESS,	inGameGUI);
}
void WalkaboutDataLoader::initCutscene(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// NOW, FIRST LET'S ADD A CUTSCENE GUI
	ScreenGUI *cutsceneGUI = new ScreenGUI();

	unsigned int textureID = guiTextureManager->loadTexture(W_INVISIBILE_PIXEL);
	// INIT THE CUTSCENE BUTTON
	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(textureID, 
							textureID,
							0,
							0,
							0,
							255,
							game->getGraphics()->getScreenWidth(),
							game->getGraphics()->getScreenHeight(),
							false,
							W_NEXT_SCENE_COMMAND);
	cutsceneGUI->addButton(buttonToAdd);
	// AND REGISTER IT WITH THE GUI
	gui->addScreenGUI(GS_CUTSCENE, cutsceneGUI);
}


void WalkaboutDataLoader::loadLevel1(Game *game) {

	// LOAD THE LEVEL'S BACKGROUND TILES
	TMXMapImporter tmxMapImporter;
	tmxMapImporter.loadWorld(game, W_LEVEL_1_DIR, W_LEVEL_1_NAME);

	//add all of the items to the physics system
	game->getGSM()->getWorld()->initWorldPhysicsSystem(game);

	// LOAD THE SPRITE IMAGES
	PoseurSpriteTypesImporter psti;
	psti.loadSpriteTypes(game, SPRITE_TYPES_LIST);

	// LET'S MAKE A PLAYER SPRITE
	GameStateManager *gsm = game->getGSM();
	Physics *physics = gsm->getPhysics();
	physics->setGravity(W_GRAVITY);
	SpriteManager *spriteManager = gsm->getSpriteManager();
	FORPlayer *player = (FORPlayer*)spriteManager->getPlayer();
	physics->addCollidableObject(player);

	// INITIALIZE THE PLAYER
	AnimatedSpriteType *playerSpriteType = spriteManager->getSpriteType(0);
	player->setSpriteType(playerSpriteType);
	player->setCurrentState(FALLING_RIGHT);
	player->setDirection(1);
	player->reset();

	PhysicalProperties *playerProps = player->getPhysicalProperties();
	playerProps->setX(256);
	playerProps->setY(256);
	playerProps->setVelocity(0.0f, 0.0f);
	playerProps->setAccelerationX(0);
	playerProps->setAccelerationY(0);
	player->setOnTileThisFrame(false);
	player->setOnTileLastFrame(false);
	player->affixTightAABBBoundingVolume();

	AnimatedSpriteType *cutsceneTriggerSprite = spriteManager->getSpriteType(16);
	createCutscene(game,cutsceneTriggerSprite,256,350,1,200);
	//create the player object in the physics world
	game->getGSM()->getBoxPhysics()->getPhysicsFactory()->createPlayerObject(game,player,false);


	// LEVEL SET UP GOES AFTER THIS POINT, ALL LEVELS MUST LOAD THESE THINGS UP TO THIS POINT BEFORE ADDING ANYTHING ELSE
	
	// Add cut scenes
	game->getGSM()->clearScenes();
	DirectXTextureManager* guiTextureManager = (DirectXTextureManager*)game->getGraphics()->getGUITextureManager();
	unsigned int imageIDs [6];
	OverlayImage* images [6];
	imageIDs[0] = guiTextureManager->loadTexture(W_CUTSCENE_1_1);
	imageIDs[1] = guiTextureManager->loadTexture(W_CUTSCENE_1_2);
	imageIDs[2] = guiTextureManager->loadTexture(W_CUTSCENE_1_3);
	imageIDs[3] = guiTextureManager->loadTexture(W_CUTSCENE_2_1);
	imageIDs[4] = guiTextureManager->loadTexture(W_CUTSCENE_2_2);
	imageIDs[5] = guiTextureManager->loadTexture(W_CUTSCENE_2_3);
	for (int i=0;i<6;i++){
		OverlayImage* imageToAdd = new OverlayImage();
		imageToAdd->x = 112;
		imageToAdd->y = 608;
		imageToAdd->z = 0;
		imageToAdd->alpha = 255;
		imageToAdd->width = 800;
		imageToAdd->height = 150;
		imageToAdd->imageID = imageIDs[i];
		images[i]=imageToAdd;
	}
	Cutscene* cuts = new Cutscene();
	cuts->addImage(images[0]);
	cuts->addImage(images[1]);
	game->getGSM()->addCutscene(cuts);
	cuts = new Cutscene();
	cuts->addImage(images[2]);
	cuts->addImage(images[3]);
	cuts->addImage(images[4]);
	cuts->addImage(images[5]);
	game->getGSM()->addCutscene(cuts);

	ScreenGUI* gui = game->getGUI()->getScreen(GS_CUTSCENE);
	gui->popOverlayImage(1);
	gui->addOverlayImage(game->getGSM()->initCutscene());

	// CREATE VINES
	createVine(game, 832, 192);
	createVine(game, 2368, 768);

	// CREATE BREAKABLE WALLS
	AnimatedSpriteType *breakable_wall = spriteManager->getSpriteType(2);
	createBreakableWall(game, breakable_wall, 2368, 1536);
	createBreakableWall(game, breakable_wall, 2752, 1536);
	createBreakableWall(game, breakable_wall, 3136, 1536);

	//CREATE SPIKES
	AnimatedSpriteType *spikes = spriteManager->getSpriteType(11);
	// First set
	createSpikes(game, spikes, 704, 576);
	createSpikes(game, spikes, 768, 576);
	createSpikes(game, spikes, 832, 576);
	createSpikes(game, spikes, 896, 576);
	// Second set
	createSpikes(game, spikes, 2496, 576);
	createSpikes(game, spikes, 2624, 576);
	createSpikes(game, spikes, 2752, 576);
	createSpikes(game, spikes, 2880, 576);
	createSpikes(game, spikes, 3008, 576);
	// Third set
	createSpikes(game, spikes, 2240, 1152);
	createSpikes(game, spikes, 2304, 1152);
	createSpikes(game, spikes, 2368, 1152);
	createSpikes(game, spikes, 2432, 1152);
	// Fourth set
	createSpikes(game, spikes, 832, 1152);
	createSpikes(game, spikes, 896, 1152);

	createSpikes(game, spikes, 1024, 1152);
	createSpikes(game, spikes, 1088, 1152);
	
	createSpikes(game, spikes, 1216, 1152);
	createSpikes(game, spikes, 1280, 1152);
	
	createSpikes(game, spikes, 1408, 1152);
	createSpikes(game, spikes, 1472, 1152);
	
	createSpikes(game, spikes, 1600, 1152);
	createSpikes(game, spikes, 1664, 1152);
	// Fifth set
	createSpikes(game, spikes, 576, 1728);
	createSpikes(game, spikes, 640, 1728);
	createSpikes(game, spikes, 704, 1728);

	createSpikes(game, spikes, 832, 1728);
	createSpikes(game, spikes, 896, 1728);
	createSpikes(game, spikes, 960, 1728);
	
	createSpikes(game, spikes, 1088, 1728);
	createSpikes(game, spikes, 1152, 1728);
	createSpikes(game, spikes, 1216, 1728);
	
	createSpikes(game, spikes, 1344, 1728);
	createSpikes(game, spikes, 1408, 1728);
	createSpikes(game, spikes, 1472, 1728);
	
	createSpikes(game, spikes, 1600, 1728);
	createSpikes(game, spikes, 1664, 1728);
	createSpikes(game, spikes, 1728, 1728);

	// CREATE BOTS
	AnimatedSpriteType *bot = spriteManager->getSpriteType(1);
	// FIRST LEVEL
	//createFORFloatingBot(game, bot, 1344, 256, 256);
	createFORFloatingBot(game, bot, 2048, 256, 256);
	// SECOND LEVEL
	createFORFloatingBot(game, bot, 2880, 896, 256);
	createFORFloatingBot(game, bot, 1984, 896, 128);
	// THIRD LEVEL
	createFORFloatingBot(game, bot, 2048, 1472, 192);
	createFORFloatingBot(game, bot, 2496, 1472, 128);
	createFORFloatingBot(game, bot, 2944, 1472, 128);
	// FOURTH LEVEL
	createFORFloatingBot(game, bot, 1152, 2048, 384);
	createFORFloatingBot(game, bot, 1920, 2048, 384);
	createFORFloatingBot(game, bot, 2688, 2048, 384);

	// CREATE THE BOSS
	AnimatedSpriteType *bossSpriteType = spriteManager->getSpriteType(6);
	BossBot *boss = new BossBot(BULLET_SPEED, BOT_ATTACK_RANGE, BULLET_SPEED, BOT_ATTACK_COOLDOWN, BOT_DESIGNATION);
	boss->setHitPoints(BOSS_HITPOINTS);
	boss->setDamage(1);
	boss->setSpriteType(bossSpriteType);
	boss->setAlpha(255);
	boss->setCurrentState(IDLE_RIGHT);
	PhysicalProperties *bossProps = boss->getPhysicalProperties();
	bossProps->setX(512);
	bossProps->setY(2048);
	bossProps->setVelocity(0.0f, 0.0f);
	bossProps->setAccelerationX(0);
	bossProps->setAccelerationY(0);
	boss->setOnTileThisFrame(false);
	boss->setOnTileLastFrame(false);
	boss->affixTightAABBBoundingVolume();

	//create a physics object for the boss
	game->getGSM()->getBoxPhysics()->getPhysicsFactory()->createEnemyObject(game,boss,false);
	physics->addCollidableObject(boss);
	spriteManager->addBot(boss);
	
	// Reset viewport
	game->getGUI()->getViewport()->setViewportX(0);
	game->getGUI()->getViewport()->setViewportY(0);
	game->getGSM()->goToCutscene();
}

// @TODO: CREATE SETUP FOR LEVEL 2
void WalkaboutDataLoader::loadLevel2(Game *game) {
	// LOAD THE LEVEL'S BACKGROUND TILES
	TMXMapImporter tmxMapImporter;
	tmxMapImporter.loadWorld(game, W_LEVEL_2_DIR, W_LEVEL_2_NAME);

	//add all of the items to the physics system
	game->getGSM()->getWorld()->initWorldPhysicsSystem(game);

	// LOAD THE SPRITE IMAGES
	PoseurSpriteTypesImporter psti;
	psti.loadSpriteTypes(game, SPRITE_TYPES_LIST);

	// LET'S MAKE A PLAYER SPRITE
	GameStateManager *gsm = game->getGSM();
	Physics *physics = gsm->getPhysics();
	physics->setGravity(W_GRAVITY);
	SpriteManager *spriteManager = gsm->getSpriteManager();
	FORPlayer *player = (FORPlayer*)spriteManager->getPlayer();
	spriteManager->setPlayer(player);
	physics->addCollidableObject(player);

	// INITIALIZE THE PLAYER
	AnimatedSpriteType *playerSpriteType = spriteManager->getSpriteType(0);
	player->setSpriteType(playerSpriteType);
	player->setCurrentState(FALLING_RIGHT);
	player->setDirection(1);
	player->reset();

	PhysicalProperties *playerProps = player->getPhysicalProperties();
	playerProps->setX(256);
	playerProps->setY(256);
	playerProps->setVelocity(0.0f, 0.0f);
	playerProps->setAccelerationX(0);
	playerProps->setAccelerationY(0);
	player->setOnTileThisFrame(false);
	player->setOnTileLastFrame(false);
	player->affixTightAABBBoundingVolume();

	//create the player object in the physics world
	game->getGSM()->getBoxPhysics()->getPhysicsFactory()->createPlayerObject(game,player,false);

	/*game->getGSM()->clearScenes();
	DirectXTextureManager* guiTextureManager = (DirectXTextureManager*)game->getGraphics()->getGUITextureManager();
	unsigned int imageIDs [6];
	OverlayImage* images [6];
	imageIDs[0] = guiTextureManager->loadTexture(W_CUTSCENE_1_1);
	imageIDs[1] = guiTextureManager->loadTexture(W_CUTSCENE_1_2);
	imageIDs[2] = guiTextureManager->loadTexture(W_CUTSCENE_1_3);
	imageIDs[3] = guiTextureManager->loadTexture(W_CUTSCENE_2_1);
	imageIDs[4] = guiTextureManager->loadTexture(W_CUTSCENE_2_2);
	imageIDs[5] = guiTextureManager->loadTexture(W_CUTSCENE_2_3);
	for (int i=0;i<6;i++){
		OverlayImage* imageToAdd = new OverlayImage();
		imageToAdd->x = 112;
		imageToAdd->y = 608;
		imageToAdd->z = 0;
		imageToAdd->alpha = 255;
		imageToAdd->width = 800;
		imageToAdd->height = 150;
		imageToAdd->imageID = imageIDs[i];
		images[i]=imageToAdd;
	}
	Cutscene* cuts = new Cutscene();
	cuts->addImage(images[0]);
	cuts->addImage(images[1]);
	game->getGSM()->addCutscene(cuts);
	cuts = new Cutscene();
	cuts->addImage(images[2]);
	cuts->addImage(images[3]);
	cuts->addImage(images[4]);
	cuts->addImage(images[5]);
	game->getGSM()->addCutscene(cuts);

	ScreenGUI* gui = game->getGUI()->getScreen(GS_CUTSCENE);
	gui->popOverlayImage(1);
	gui->addOverlayImage(game->getGSM()->initCutscene());
	*/

	// Create wall switches
	AnimatedSpriteType *wall_2 = spriteManager->getSpriteType(12);
	AnimatedSpriteType *wall_3 = spriteManager->getSpriteType(13);
	AnimatedSpriteType *_switch = spriteManager->getSpriteType(14);
	// First Pair
	SwitchWall* wall = createSwitchWall(game, wall_2, 2880, 1152);	
	createSwitch(game, _switch, wall, 3456, 1280, true);
	// Second Pair
	wall = createSwitchWall(game, wall_3, 448, 1024);
	createSwitch(game, _switch, wall, 192, 960, false);

	// Create Water
	AnimatedSpriteType *water = spriteManager->getSpriteType(17);
	createSpikes(game, water, 1216, 1856);
	createSpikes(game, water, 1280, 1856);
	createSpikes(game, water, 1344, 1856);
	createSpikes(game, water, 1408, 1856);

	createSpikes(game, water, 1664, 1856);
	createSpikes(game, water, 1728, 1856);
	createSpikes(game, water, 1792, 1856);
	createSpikes(game, water, 1856, 1856);

	// Create Vines
	createVine(game, 1344, 1472);
	createVine(game, 1792, 1472);

	// Create Monkies
	AnimatedSpriteType *monkey = spriteManager->getSpriteType(15);
	createMonkeyBot(game, monkey, 3216, 1024, 256, -1);
	createMonkeyBot(game, monkey, 400, 704, 256, 1);

	// Create Bots
	AnimatedSpriteType *bot = spriteManager->getSpriteType(1);
	// First level
	createFORFloatingBot(game, bot, 1024, 256, 256);
	createFORFloatingBot(game, bot, 1536, 256, 256);
	createFORFloatingBot(game, bot, 2048, 256, 256);
	createFORFloatingBot(game, bot, 2560, 256, 256);
	// Second level
	createFORFloatingBot(game, bot, 1536, 1216, 256);
	createFORFloatingBot(game, bot, 2048, 1216, 256);
	createFORFloatingBot(game, bot, 2560, 1216, 256);
	// Third level
	createFORFloatingBot(game, bot, 768, 832, 128);
	// Fourth level
	createFORFloatingBot(game, bot, 2816, 1600, 512);
	// Fifth level
	createFORFloatingBot(game, bot, 2944, 2176, 256);

	// Create Boss
	AnimatedSpriteType *bossSpriteType = spriteManager->getSpriteType(6);
	BossBot *boss = new BossBot(BULLET_SPEED, BOT_ATTACK_RANGE, BULLET_SPEED, BOT_ATTACK_COOLDOWN, BOT_DESIGNATION);
	boss->setHitPoints(BOSS_HITPOINTS);
	boss->setDamage(1);
	boss->setSpriteType(bossSpriteType);
	boss->setAlpha(255);
	boss->setCurrentState(IDLE_RIGHT);
	PhysicalProperties *bossProps = boss->getPhysicalProperties();
	bossProps->setX(1920);
	bossProps->setY(2176);
	bossProps->setVelocity(0.0f, 0.0f);
	bossProps->setAccelerationX(0);
	bossProps->setAccelerationY(0);
	boss->setOnTileThisFrame(false);
	boss->setOnTileLastFrame(false);
	boss->affixTightAABBBoundingVolume();

	//create a physics object for the boss
	game->getGSM()->getBoxPhysics()->getPhysicsFactory()->createEnemyObject(game,boss,false);
	physics->addCollidableObject(boss);
	spriteManager->addBot(boss);

	// Reset viewport
	game->getGUI()->getViewport()->setViewportX(0);
	game->getGUI()->getViewport()->setViewportY(0);
	//game->getGSM()->goToCutscene();
}

//@TODO: CREATE SET UP FOR LEVEL 3
void WalkaboutDataLoader::loadLevel3(Game *game) {
	// LOAD THE LEVEL'S BACKGROUND TILES
	TMXMapImporter tmxMapImporter;
	tmxMapImporter.loadWorld(game, W_LEVEL_3_DIR, W_LEVEL_3_NAME);

	//add all of the items to the physics system
	game->getGSM()->getWorld()->initWorldPhysicsSystem(game);

	// LOAD THE SPRITE IMAGES
	PoseurSpriteTypesImporter psti;
	psti.loadSpriteTypes(game, SPRITE_TYPES_LIST);

	// LET'S MAKE A PLAYER SPRITE
	GameStateManager *gsm = game->getGSM();
	Physics *physics = gsm->getPhysics();
	physics->setGravity(W_GRAVITY);
	SpriteManager *spriteManager = gsm->getSpriteManager();
	FORPlayer *player = (FORPlayer*)spriteManager->getPlayer();
	spriteManager->setPlayer(player);
	physics->addCollidableObject(player);

	// INITIALIZE THE PLAYER
	AnimatedSpriteType *playerSpriteType = spriteManager->getSpriteType(0);
	player->setSpriteType(playerSpriteType);
	player->setCurrentState(FALLING_RIGHT);
	player->setDirection(1);
	player->reset();

	PhysicalProperties *playerProps = player->getPhysicalProperties();
	playerProps->setX(256);
	playerProps->setY(2048);
	playerProps->setVelocity(0.0f, 0.0f);
	playerProps->setAccelerationX(0);
	playerProps->setAccelerationY(0);
	player->setOnTileThisFrame(false);
	player->setOnTileLastFrame(false);
	player->affixTightAABBBoundingVolume();

	//create the player object in the physics world
	game->getGSM()->getBoxPhysics()->getPhysicsFactory()->createPlayerObject(game,player,false);

	/*game->getGSM()->clearScenes();
	DirectXTextureManager* guiTextureManager = (DirectXTextureManager*)game->getGraphics()->getGUITextureManager();
	unsigned int imageIDs [6];
	OverlayImage* images [6];
	imageIDs[0] = guiTextureManager->loadTexture(W_CUTSCENE_1_1);
	imageIDs[1] = guiTextureManager->loadTexture(W_CUTSCENE_1_2);
	imageIDs[2] = guiTextureManager->loadTexture(W_CUTSCENE_1_3);
	imageIDs[3] = guiTextureManager->loadTexture(W_CUTSCENE_2_1);
	imageIDs[4] = guiTextureManager->loadTexture(W_CUTSCENE_2_2);
	imageIDs[5] = guiTextureManager->loadTexture(W_CUTSCENE_2_3);
	for (int i=0;i<6;i++){
		OverlayImage* imageToAdd = new OverlayImage();
		imageToAdd->x = 112;
		imageToAdd->y = 608;
		imageToAdd->z = 0;
		imageToAdd->alpha = 255;
		imageToAdd->width = 800;
		imageToAdd->height = 150;
		imageToAdd->imageID = imageIDs[i];
		images[i]=imageToAdd;
	}
	Cutscene* cuts = new Cutscene();
	cuts->addImage(images[0]);
	cuts->addImage(images[1]);
	game->getGSM()->addCutscene(cuts);
	cuts = new Cutscene();
	cuts->addImage(images[2]);
	cuts->addImage(images[3]);
	cuts->addImage(images[4]);
	cuts->addImage(images[5]);
	game->getGSM()->addCutscene(cuts);

	ScreenGUI* gui = game->getGUI()->getScreen(GS_CUTSCENE);
	gui->popOverlayImage(1);
	gui->addOverlayImage(game->getGSM()->initCutscene());
	*/

	// Create Monkeys
	AnimatedSpriteType *monkey = spriteManager->getSpriteType(15);
	createMonkeyBot(game, monkey, 1232, 2048, 256, -1);
	createMonkeyBot(game, monkey, 2000, 1920, 256, -1);
	createMonkeyBot(game, monkey, 400, 1664, 256, 1);
	createMonkeyBot(game, monkey, 1232, 1408, 256, -1);
	createMonkeyBot(game, monkey, 1744, 1472, 256, -1);
	createMonkeyBot(game, monkey, 848, 960, 256, 1);
	createMonkeyBot(game, monkey, 848, 704, 256, 1);
	createMonkeyBot(game, monkey, 464, 576, 256, 1);
	createMonkeyBot(game, monkey, 272, 448, 256, 1);
	createMonkeyBot(game, monkey, 2192, 384, 256, -1);


	// Create Bots
	AnimatedSpriteType *bot = spriteManager->getSpriteType(1);
	// Level 1
	createFORFloatingBot(game, bot, 896, 2176, 192);
	// Level 2
	createFORFloatingBot(game, bot, 1408, 2304, 192);
	createFORFloatingBot(game, bot, 1728, 2304, 192);
	createFORFloatingBot(game, bot, 2048, 2304, 192);
	// Level 3
	createFORFloatingBot(game, bot, 1664, 2048, 256);
	// Level 4
	createFORFloatingBot(game, bot, 768, 1792, 192);
	createFORFloatingBot(game, bot, 1152, 1792, 192);
	createFORFloatingBot(game, bot, 1536, 1792, 192);
	// Level 5
	createFORFloatingBot(game, bot, 834, 1536, 256);
	// Level 6
	createFORFloatingBot(game, bot, 1024, 1410, 256);
	// Level 7
	createFORFloatingBot(game, bot, 1216, 576, 192);
	createFORFloatingBot(game, bot, 1600, 576, 192);
	createFORFloatingBot(game, bot, 1984, 576, 192);
	// Level 8
	createFORFloatingBot(game, bot, 640, 384, 256);
	createFORFloatingBot(game, bot, 1984, 384, 256);


	// Create Boss
	AnimatedSpriteType *bossSpriteType = spriteManager->getSpriteType(6);
	BossBot *boss = new BossBot(BULLET_SPEED, BOT_ATTACK_RANGE, BULLET_SPEED, BOT_ATTACK_COOLDOWN, BOT_DESIGNATION);
	boss->setHitPoints(BOSS_HITPOINTS);
	boss->setDamage(1);
	boss->setSpriteType(bossSpriteType);
	boss->setAlpha(255);
	boss->setCurrentState(IDLE_RIGHT);
	PhysicalProperties *bossProps = boss->getPhysicalProperties();
	bossProps->setX(1280);
	bossProps->setY(256);
	bossProps->setVelocity(0.0f, 0.0f);
	bossProps->setAccelerationX(0);
	bossProps->setAccelerationY(0);
	boss->setOnTileThisFrame(false);
	boss->setOnTileLastFrame(false);
	boss->affixTightAABBBoundingVolume();

	//create a physics object for the boss
	game->getGSM()->getBoxPhysics()->getPhysicsFactory()->createEnemyObject(game,boss,false);
	physics->addCollidableObject(boss);
	spriteManager->addBot(boss);

	// Reset viewport
	game->getGUI()->getViewport()->setViewportX(0);
	game->getGUI()->getViewport()->setViewportY(2560);
	//game->getGSM()->goToCutscene();
}

/*
	Create a vine at a given x,y coordinate
*/
void WalkaboutDataLoader::createVine(Game* game, int x, int y) {
	// Create all parts of the vine
	vector<AnimatedSprite *> spritesList;
	AnimatedSpriteType *vineSpriteType = game->getGSM()->getSpriteManager()->getSpriteType(4);
	for(int i = 0; i < 10; i++){
		Vine *vine = new Vine(PROJECTILE_DESIGNATION);
		vine->setHitPoints(1);
		vine->setDamage(0);
		vine->setSpriteType(vineSpriteType);
		vine->setAlpha(255);
		vine->setCurrentState(IDLE_LEFT);
		PhysicalProperties *vineProps = vine->getPhysicalProperties();
		vineProps->setX(x);
		vineProps->setY(y);
		vineProps->setVelocity(0.0f, 0.0f);
		vineProps->setAccelerationX(0);
		vineProps->setAccelerationY(0);
		vine->setOnTileThisFrame(false);
		vine->setOnTileLastFrame(false);
		vine->affixTightAABBBoundingVolume();
		game->getGSM()->getSpriteManager()->addBot(vine);
		spritesList.push_back(vine);
	}
	//now create the vine
	game->getGSM()->getBoxPhysics()->getPhysicsFactory()->createStaticRope(game,spritesList,x,y);
}

void WalkaboutDataLoader::createSpikes(Game *game, AnimatedSpriteType *spriteType, int x, int y) {
	Spikes *spikes = new Spikes(SPIKES_DESIGNATION);
	spikes->setHitPoints(WALL_HITPOINTS);
	spikes->setDamage(PLAYER_HITPOINTS);
	spikes->setSpriteType(spriteType);
	spikes->setAlpha(255);
	spikes->setCurrentState(IDLE_LEFT);
	PhysicalProperties *spikesProps = spikes->getPhysicalProperties();
	spikesProps->setX(x);
	spikesProps->setY(y);
	spikesProps->setVelocity(0.0f, 0.0f);
	spikesProps->setAccelerationX(0);
	spikesProps->setAccelerationY(0);
	spikes->setOnTileThisFrame(false);
	spikes->setOnTileLastFrame(false);
	spikes->affixTightAABBBoundingVolume();

	//create a physics object for the spikes
	game->getGSM()->getBoxPhysics()->getPhysicsFactory()->createStaticWorldObject(game,spikes);
	game->getGSM()->getPhysics()->addCollidableObject(spikes);
	game->getGSM()->getSpriteManager()->addBot(spikes);
}

void WalkaboutDataLoader::createBreakableWall(Game *game, AnimatedSpriteType* spriteType, int x, int y) {
	BreakableWall *wall = new BreakableWall(WALL_DESIGNATION);
	wall->setHitPoints(WALL_HITPOINTS);
	wall->setDamage(0);
	wall->setSpriteType(spriteType);
	wall->setAlpha(255);
	wall->setCurrentState(FIVE);
	PhysicalProperties *wallProps = wall->getPhysicalProperties();
	wallProps->setX(x);
	wallProps->setY(y);
	wallProps->setVelocity(0.0f, 0.0f);
	wallProps->setAccelerationX(0);
	wallProps->setAccelerationY(0);
	wall->setOnTileThisFrame(false);
	wall->setOnTileLastFrame(false);
	wall->affixTightAABBBoundingVolume();

	//create a physics object for the wall
	game->getGSM()->getBoxPhysics()->getPhysicsFactory()->createStaticWorldObject(game,wall);
	game->getGSM()->getPhysics()->addCollidableObject(wall);
	game->getGSM()->getSpriteManager()->addBot(wall);
}


void WalkaboutDataLoader::createFORFloatingBot(Game *game, AnimatedSpriteType* spriteType, int x, int y, int range) {
	FORFloatingBot *bot = new FORFloatingBot(BOT_MIN_CYCLES, BOT_MAX_CYCLES, BOT_VELOCITY, x, range, BOT_DESIGNATION);
	bot->setHitPoints(BOT_HITPOINTS);
	bot->setDamage(BOT_DAMAGE);
	bot->setSpriteType(spriteType);
	bot->setAlpha(255);
	bot->setCurrentState(IDLE_RIGHT);
	PhysicalProperties *botProps = bot->getPhysicalProperties();
	botProps->setX(x);
	botProps->setY(y);
	botProps->setVelocity(0.0f, 0.0f);
	botProps->setAccelerationX(0);
	botProps->setAccelerationY(0);
	bot->setOnTileThisFrame(false);
	bot->setOnTileLastFrame(false);
	bot->affixTightAABBBoundingVolume();
			
	//create a physics object for the bot
	game->getGSM()->getBoxPhysics()->getPhysicsFactory()->createEnemyObject(game,bot,false);
	game->getGSM()->getPhysics()->addCollidableObject(bot);
	game->getGSM()->getSpriteManager()->addBot(bot);
}

void WalkaboutDataLoader::createMonkeyBot(Game *game, AnimatedSpriteType* spriteType, int x, int y, int range, int direction) {
	MonkeyBot *monkey = new MonkeyBot(BULLET_SPEED, BOT_ATTACK_RANGE, MONKEY_DAMAGE, MONKEY_ATTACK_COOLDOWN, BOT_DESIGNATION, direction);
	monkey->setHitPoints(MONKEY_HITPOINTS);
	monkey->setDamage(MONKEY_DAMAGE);
	monkey->setSpriteType(spriteType);
	monkey->setAlpha(255);
	monkey->setCurrentState(direction==1?IDLE_RIGHT:IDLE_LEFT);
	PhysicalProperties *monkeyProps = monkey->getPhysicalProperties();
	monkeyProps->setX(x);
	monkeyProps->setY(y);
	monkeyProps->setVelocity(0.0f, 0.0f);
	monkeyProps->setAccelerationX(0);
	monkeyProps->setAccelerationY(0);
	monkey->setOnTileThisFrame(false);
	monkey->setOnTileLastFrame(false);
	monkey->affixTightAABBBoundingVolume();

	//create a physics object for the monkey
	game->getGSM()->getBoxPhysics()->getPhysicsFactory()->createEnemyObject(game,monkey,false);
	game->getGSM()->getPhysics()->addCollidableObject(monkey);
	game->getGSM()->getSpriteManager()->addBot(monkey);
}

SwitchWall* WalkaboutDataLoader::createSwitchWall(Game *game, AnimatedSpriteType* spriteType, int x, int y){
	SwitchWall *wall = new SwitchWall(WALL_DESIGNATION);
	wall->setHitPoints(MAX_HITPOINTS);
	wall->setDamage(0);
	wall->setSpriteType(spriteType);
	wall->setAlpha(255);
	wall->setCurrentState(IDLE_LEFT);
	PhysicalProperties *wallProps = wall->getPhysicalProperties();
	wallProps->setX(x);
	wallProps->setY(y);
	wallProps->setVelocity(0.0f, 0.0f);
	wallProps->setAccelerationX(0);
	wallProps->setAccelerationY(0);
	wall->setOnTileThisFrame(false);
	wall->setOnTileLastFrame(false);
	wall->affixTightAABBBoundingVolume();

	//create a physics object for the wall
	game->getGSM()->getBoxPhysics()->getPhysicsFactory()->createStaticWorldObject(game,wall);
	game->getGSM()->getPhysics()->addCollidableObject(wall);
	game->getGSM()->getSpriteManager()->addBot(wall);

	return wall;
}

void WalkaboutDataLoader::createSwitch(Game *game, AnimatedSpriteType* spriteType, SwitchWall *target, int x, int y, bool dirL){
	Switch *_switch = new Switch(WALL_DESIGNATION, target);

	_switch->setHitPoints(1);
	_switch->setDamage(0);
	_switch->setSpriteType(spriteType);
	_switch->setAlpha(255);
	if(dirL)
		_switch->setCurrentState(IDLE_LEFT);
	else
		_switch->setCurrentState(IDLE_RIGHT);
	PhysicalProperties *_switchProps = _switch->getPhysicalProperties();
	_switchProps->setX(x);
	_switchProps->setY(y);
	_switchProps->setVelocity(0.0f, 0.0f);
	_switchProps->setAccelerationX(0);
	_switchProps->setAccelerationY(0);
	_switch->setOnTileThisFrame(false);
	_switch->setOnTileLastFrame(false);
	_switch->affixTightAABBBoundingVolume();

	//create a physics object for the wall
	game->getGSM()->getBoxPhysics()->getPhysicsFactory()->createStaticWorldObject(game,_switch);
	game->getGSM()->getPhysics()->addCollidableObject(_switch);
	game->getGSM()->getSpriteManager()->addBot(_switch);
}

void WalkaboutDataLoader::createCutscene(Game *game, AnimatedSpriteType *spriteType, int x, int y, int height, int width){
	CutsceneTrigger *trigger = new CutsceneTrigger(CUTSCENE_DESIGNATION);

	trigger->setHitPoints(1);
	trigger->setDamage(0);
	trigger->setSpriteType(spriteType);
	trigger->setAlpha(255);
	trigger->setCurrentState(IDLE_LEFT);

	PhysicalProperties *triggerProps = trigger->getPhysicalProperties();
	triggerProps->setX(x);
	triggerProps->setY(y);
	triggerProps->setVelocity(0.0f, 0.0f);
	triggerProps->setAccelerationX(0);
	triggerProps->setAccelerationY(0);
	trigger->setOnTileThisFrame(false);
	trigger->setOnTileLastFrame(false);
	trigger->affixTightAABBBoundingVolume();

	//create a physics object for the wall
	game->getGSM()->getBoxPhysics()->getPhysicsFactory()->createStaticWorldTrigger(game,trigger,width,height);
	game->getGSM()->getPhysics()->addCollidableObject(trigger);
	game->getGSM()->getSpriteManager()->addBot(trigger);
}