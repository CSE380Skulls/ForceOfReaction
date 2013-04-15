#pragma once
#include "stdafx.h"

/*
	WalkaboutGame.h

	This file contains setup data for making the Walkabout game. Note that nearly
	all of this data would better serve us if loaded from files.
*/

// HERE IS THE Walkabout GAME SETUP/INIT FILE. EVERYTHING WOULD START FROM HERE
static const wstring	W_LEVEL_1_DIR			= L"data/levels/earth/";
static const wstring	W_LEVEL_1_NAME			= L"earth.tmx";
static const wstring	W_LEVEL_1_PATH			= W_LEVEL_1_DIR + W_LEVEL_1_NAME;
static const wstring	W_INIT_FILE				= L"data/WalkaboutInit.txt";
static const wstring	W_GUI_INIT_FILE			= L"data/WalkaboutGUI.txt";
static const string		W_DEBUG_FILE			= "DebugOutput.txt";
static const char*		GAME_INIT_FILE          = "data/lua_scripts/game_init.lua";

// HERE ARE THE GAME PROPERTIES WE ARE LOADING FROM FROM THE SETUP FILE
static const wstring	W_TITLE					= L"TITLE";
static const wstring	W_SCREEN_WIDTH			= L"SCREEN_WIDTH";
static const wstring	W_SCREEN_HEIGHT			= L"SCREEN_HEIGHT";
static const wstring	W_USE_FULLSCREEN_MODE	= L"USE_FULLSCREEN_MODE";
static const wstring	W_TEXT_FONT_SIZE		= L"TEXT_FONT_SIZE";
static const wstring	W_VIEWPORT_OFFSET_X		= L"VIEWPORT_OFFSET_X";
static const wstring	W_VIEWPORT_OFFSET_Y		= L"VIEWPORT_OFFSET_Y";
static const wstring	W_FONT_COLOR_RED		= L"FONT_COLOR_RED";
static const wstring	W_FONT_COLOR_GREEN		= L"FONT_COLOR_GREEN";
static const wstring	W_FONT_COLOR_BLUE		= L"FONT_COLOR_BLUE";
static const wstring	W_COLOR_KEY_RED			= L"COLOR_KEY_RED";
static const wstring	W_COLOR_KEY_GREEN		= L"COLOR_KEY_GREEN";
static const wstring	W_COLOR_KEY_BLUE		= L"COLOR_KEY_BLUE";

// CONSTANTS FOR ARTWORK - NOTE, THIS SHOULD REALLY BE LOADED FROM A FILE
static const wstring	W_GO_TO_MM_COMMAND		= L"Go To Main Menu";
static const wstring	W_START_COMMAND			= L"Start";
static const wstring	W_EXIT_COMMAND			= L"Exit";
static const wstring	W_QUIT_COMMAND			= L"Quit";
static const wstring	W_HELP_COMMAND			= L"Help";
static const wstring	W_CREDITS_COMMAND		= L"Credits";
static const wstring	W_RETURN_COMMAND		= L"Return";
//static const wstring	W_GREEN_CURSOR_PATH		= L"data/gui/cursor/green_cursor.png";
//static const wstring	W_RED_CURSOR_PATH		= L"data/gui/cursor/red_cursor.png";
static const wstring    W_CURSOR_PATH           = L"data/gui/cursor/cursor.png";
static const wstring	W_SPLASH_SCREEN_PATH	= L"data/gui/overlays/fon_splash_screen_overlay.png";
static const wstring	W_CREDITS_SCREEN_PATH	= L"data/gui/overlays/fon_credits_overlay.png";
static const wstring	W_OPTION_SCREEN_PATH	= L"data/gui/overlays/fon_help_overlay.png";
static const wstring	W_LOADING_SCREEN_PATH	= L"data/gui/overlays/fon_loading_overlay.png";
static const wstring	W_MAIN_MENU_PATH		= L"data/gui/overlays/fon_main_menu_overlay.png";
static const wstring	W_PAUSED_MENU_PATH		= L"data/gui/overlays/fon_in_game_menu_overlay.png";
static const wstring	W_START_IMAGE_PATH		= L"data/gui/buttons/start_game.png";
static const wstring	W_START_IMAGE_MO_PATH	= L"data/gui/buttons/start_game_mo.png";
static const wstring	W_EXIT_IMAGE_PATH		= L"data/gui/buttons/exit_game.png";
static const wstring	W_EXIT_IMAGE_MO_PATH	= L"data/gui/buttons/exit_game_mo.png";
static const wstring	W_QUIT_IMAGE_PATH		= L"data/gui/buttons/quit.png";
static const wstring	W_QUIT_IMAGE_MO_PATH	= L"data/gui/buttons/quit_mo.png";
static const wstring	W_OPTIONS_IMAGE_PATH	= L"data/gui/buttons/options.png";
static const wstring	W_OPTIONS_IMAGE_MO_PATH	= L"data/gui/buttons/options_mo.png";
static const wstring	W_CREDITS_IMAGE_PATH	= L"data/gui/buttons/credits.png";
static const wstring	W_CREDITS_IMAGE_MO_PATH	= L"data/gui/buttons/credits_mo.png";
static const wstring	W_RETURN_IMAGE_PATH		= L"data/gui/buttons/return.png";
static const wstring	W_RETURN_IMAGE_MO_PATH	= L"data/gui/buttons/return_mo.png";

static const wstring	W_STATUS_HEAD_PATH		= L"data/gui/ingame/mc_head.png";
static const wstring	W_STATUS_FIRE_PATH		= L"data/gui/ingame/fire_health.png";
static const wstring	W_STATUS_WATER_PATH		= L"data/gui/ingame/water_health.png";
static const wstring	W_STATUS_EARTH_PATH		= L"data/gui/ingame/earth_health.png";

// FOR OUR SPRITE
static const wstring	SPRITE_TYPES_DIR = L"data/sprite_types/";
static const wstring	SPRITE_TYPES_LIST = SPRITE_TYPES_DIR + L"sprite_type_list.xml";
//static const wstring	JUMPING = L"JUMPING";

// FOR THE GAME WORLD
static const float		MAX_VIEWPORT_AXIS_VELOCITY = 20.0f;
static const float		W_GRAVITY = 0.95f;
static const float		JUMPING_BOT_MIN_CYCLES = 30;
static const float		JUMPING_BOT_MAX_CYCLES = 120;
static const float		JUMPING_BOT_MAX_VELOCITY = 40;
static const float		PLAYER_INIT_X = 250.0f;
static const float		PLAYER_INIT_Y = 250.0f;
static const float		JUMP_SPEED = -18.0f;
static const float		PLAYER_SPEED = 6.0f;
//static const int		MIN_FPS = 5;
//static const int		MAX_FPS = 100;
//static const int		FPS_INC = 1;
//static const wstring	ATTACKING_RIGHT(L"ATTACKING_RIGHT");
//static const wstring	ATTACKING_LEFT(L"ATTACKING_LEFT");
//static const wstring	IDLE(L"IDLE");
static const wstring	IDLE_RIGHT(L"IDLE_RIGHT");
static const wstring	IDLE_LEFT(L"IDLE_LEFT");
static const wstring	WALKING_RIGHT(L"WALKING_RIGHT");
static const wstring	WALKING_LEFT(L"WALKING_LEFT");
static const wstring	JUMPING_RIGHT(L"JUMPING_RIGHT");
static const wstring	JUMPING_LEFT(L"JUMPING_LEFT");
static const wstring	FALLING_RIGHT(L"FALLING_RIGHT");
static const wstring	FALLING_LEFT(L"FALLING_LEFT");

// USED FOR MOVEMENT
const unsigned int W_KEY = (unsigned int)'W';
const unsigned int A_KEY = (unsigned int)'A';
const unsigned int S_KEY = (unsigned int)'S';
const unsigned int D_KEY = (unsigned int)'D';
const unsigned int P_KEY = (unsigned int)'P';
const unsigned int T_KEY = (unsigned int)'T';
const unsigned int UP_KEY = VK_UP;
const unsigned int DOWN_KEY = VK_DOWN;
const unsigned int LEFT_KEY = VK_LEFT;
const unsigned int RIGHT_KEY = VK_RIGHT;
const unsigned int SPACE_KEY = VK_SPACE;
const unsigned int ENTER_KEY = VK_RETURN;

// THIS IS JUST FOR SHOWING HOW THE CURSOR CAN BE CHANGED
//const unsigned int C_KEY = (unsigned int)'C';