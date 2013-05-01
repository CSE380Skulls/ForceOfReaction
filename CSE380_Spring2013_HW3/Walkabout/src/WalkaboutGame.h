#pragma once
#include "stdafx.h"

/*
	WalkaboutGame.h

	This file contains setup data for making the Walkabout game. Note that nearly
	all of this data would better serve us if loaded from files.
*/

// Paths of initialization files
static const wstring	W_LEVEL_1_DIR			= L"data/levels/earth/";
static const wstring	W_LEVEL_1_NAME			= L"earth_test.tmx";
static const wstring	W_LEVEL_1_PATH			= W_LEVEL_1_DIR + W_LEVEL_1_NAME;
static const wstring	W_INIT_FILE				= L"data/WalkaboutInit.txt";
static const wstring	W_GUI_INIT_FILE			= L"data/WalkaboutGUI.txt";
static const string		W_DEBUG_FILE			= "DebugOutput.txt";
static const char*		GAME_INIT_FILE          = "data/lua_scripts/game_init.lua";

// Button commands
static const wstring	W_GO_TO_MM_COMMAND		= L"Go To Main Menu";
static const wstring	W_START_COMMAND			= L"Start";
static const wstring	W_EXIT_COMMAND			= L"Exit";
static const wstring	W_QUIT_COMMAND			= L"Quit";
static const wstring	W_HELP_COMMAND			= L"Help";
static const wstring	W_CREDITS_COMMAND		= L"Credits";
static const wstring	W_RETURN_COMMAND		= L"Return";

// Lua stuff
static const char*		LUA_TITLE				= "title";
static const char*		LUA_SCREEN_WIDTH		= "screen_width";
static const char*		LUA_SCREEN_HEIGHT		= "screen_height";
static const char*		LUA_FULLSCREEN_MODE		= "fullscreen_mode";
static const char*		LUA_FONT_SIZE			= "font_size";
static const char*		LUA_VIEWPORT_OFFSET_X	= "viewport_offset_x";
static const char*		LUA_VIEWPORT_OFFSET_Y   = "viewport_offset_y";
static const char*		LUA_FONT_COLOR_RED		= "font_color_red";
static const char*		LUA_FONT_COLOR_GREEN	= "font_color_green";
static const char*		LUA_FONT_COLOR_BLUE		= "font_color_blue";
static const char*		LUA_COLOR_KEY_RED       = "color_key_red";
static const char*		LUA_COLOR_KEY_GREEN		= "color_key_green";
static const char*		LUA_COLOR_KEY_BLUE		= "color_key_blue";

// Menu screen paths
static const wstring    W_CURSOR_PATH           = L"data/gui/cursor/cursor.png";
static const wstring	W_SPLASH_SCREEN_PATH	= L"data/gui/overlays/for_splash_screen_overlay.png";
static const wstring	W_CREDITS_SCREEN_PATH	= L"data/gui/overlays/for_credits_overlay.png";
static const wstring	W_OPTION_SCREEN_PATH	= L"data/gui/overlays/for_help_overlay.png";
static const wstring	W_LOADING_SCREEN_PATH	= L"data/gui/overlays/for_loading_overlay.png";
static const wstring	W_MAIN_MENU_PATH		= L"data/gui/overlays/for_main_menu_overlay.png";
static const wstring	W_PAUSED_MENU_PATH		= L"data/gui/overlays/for_in_game_menu_overlay.png";
static const wstring    W_GAME_OVER_SCREEN_PATH = L"data/gui/overlays/for_game_over_overlay.png";
static const wstring    W_LEVEL_WON_SCREEN_PATH = L"data/gui/overlays/for_level_won_overlay.png";

// Button images 
static const wstring    W_NEXT_LEVEL_IMAGE_PATH = L"data/gui/buttons/next_level.png";
static const wstring	W_NEXT_LEVEL_IMAGE_MO_PATH = L"data/gui/buttons/next_level_mo.png";
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

// Music
static char				*C_INTRO				= "data/audio/music/Lullaby.mp3";
static char				*C_LEVEL1				= "data/audio/music/Hunting.mp3";
// Sounds
static char				*C_EXPLOSION			= "data/audio/sounds/explosion.wav";
static char				*C_DEATH				= "data/audio/sounds/death.wav";
static char				*C_EXPLOSION2			= "data/audio/sounds/explosion2.wav";
static char				*C_HIT					= "data/audio/sounds/hit.wav";
static char				*C_JUMP					= "data/audio/sounds/jump.wav";
static char				*C_MENUBUTTON			= "data/audio/sounds/menubutton.wav";
static char				*C_PLAYERHIT			= "data/audio/sounds/playerhit.wav";
static char				*C_SWING				= "data/audio/sounds/swing.wav";

// Element bars
static const wstring	W_STATUS_HEAD_PATH		= L"data/gui/ingame/mc_head.png";
static const wstring	W_STATUS_HP_10_PATH		= L"data/gui/ingame/hp_10.png";
static const wstring	W_STATUS_HP_9_PATH		= L"data/gui/ingame/hp_9.png";
static const wstring	W_STATUS_HP_8_PATH		= L"data/gui/ingame/hp_8.png";
static const wstring	W_STATUS_HP_7_PATH		= L"data/gui/ingame/hp_7.png";
static const wstring	W_STATUS_HP_6_PATH		= L"data/gui/ingame/hp_6.png";
static const wstring	W_STATUS_HP_5_PATH		= L"data/gui/ingame/hp_5.png";
static const wstring	W_STATUS_HP_4_PATH		= L"data/gui/ingame/hp_4.png";
static const wstring	W_STATUS_HP_3_PATH		= L"data/gui/ingame/hp_3.png";
static const wstring	W_STATUS_HP_2_PATH		= L"data/gui/ingame/hp_2.png";
static const wstring	W_STATUS_HP_1_PATH		= L"data/gui/ingame/hp_1.png";
static const wstring	W_STATUS_HP_0_PATH		= L"data/gui/ingame/hp_0.png";

static const wstring	W_STATUS_EARTH_SELECTED_PATH = L"data/gui/ingame/rock_selected.png";
static const wstring	W_STATUS_EARTH_UNSELECTED_PATH = L"data/gui/ingame/rock_unselected.png";
static const wstring	W_STATUS_WATER_SELECTED_PATH = L"data/gui/ingame/water_selected.png";
static const wstring	W_STATUS_WATER_UNSELECTED_PATH = L"data/gui/ingame/water_unselected.png";
static const wstring	W_STATUS_FIRE_SELECTED_PATH = L"data/gui/ingame/fire_selected.png";
static const wstring	W_STATUS_FIRE_UNSELECTED_PATH = L"data/gui/ingame/fire_unselected.png";

// FOR OUR SPRITE
static const wstring	SPRITE_TYPES_DIR = L"data/sprite_types/";
static const wstring	SPRITE_TYPES_LIST = SPRITE_TYPES_DIR + L"sprite_type_list.xml";

// CONSTANTS FOR THE GAME WORLD
static const float		MAX_VIEWPORT_AXIS_VELOCITY = 5.0f;
static const float		W_GRAVITY = 0.95f;
static const float		BOT_MIN_CYCLES = 45;
static const float		BOT_MAX_CYCLES = 75;
static const float		BOT_VELOCITY = 12;
static const float		BOT_INIT_X = 750;
static const float		BOT_INIT_Y = 250;
static const float		PLAYER_INIT_X = 250.0f;
static const float		PLAYER_INIT_Y = 250.0f;
static const float		JUMP_VELOCITY = 60.0f;
static const float		PLAYER_VELOCITY = 10.0f;
static const int		MIN_FPS = 5;
static const int		MAX_FPS = 100;
static const int		FPS_INC = 1;
static const float      BOX2D_CONVERSION_FACTOR = 32;
static const int        BOT_ATTACK_RANGE = 500;
static const int 		PROJECTILE_VELOCITY = 50;
static const int		PLAYER_HITPOINTS = 30;
static const int		BOT_HITPOINTS = 10;
static const int		BOT_DAMAGE = 1;
static const int		SEED_DAMAGE = 5;
static const int		VINE_DAMAGE = 10;
static const int		WALL_HITPOINTS = 50;
static const int		WALL1_X = 1152;
static const int		WALL_Y = 1792;
static const int		WALL2_X = 1984;
static const int		BULLET_SPEED = 100;
static const int		BULLET_DAMAGE = 50;
static const int		BOSS_HITPOINTS = 100;
static const int		BOSS_X = 500;
static const int		BOSS_Y = 2000;
static const int		BOT_ATTACK_COOLDOWN = 30;
static const int		PLAYER_ATTACK_COOLDOWN = 30;
static const int		PLAYER_DEATH_COOLDOWN = 60;
static const int		FIRE_BALL_DAMAGE = 25;


// Animated Sprite States
static const wstring	IDLE_RIGHT(L"IDLE_RIGHT");
static const wstring	IDLE_LEFT(L"IDLE_LEFT");
static const wstring	WALKING_RIGHT(L"WALKING_RIGHT");
static const wstring	WALKING_LEFT(L"WALKING_LEFT");
static const wstring	JUMPING_RIGHT(L"JUMPING_RIGHT");
static const wstring	JUMPING_LEFT(L"JUMPING_LEFT");
static const wstring	FALLING_RIGHT(L"FALLING_RIGHT");
static const wstring	FALLING_LEFT(L"FALLING_LEFT");
static const wstring	ATTACKING_RIGHT(L"ATTACKING_RIGHT");
static const wstring	ATTACKING_LEFT(L"ATTACKING_LEFT");
static const wstring	DEAD(L"DEAD");
static const wstring	ONE(L"ONE");
static const wstring	TWO(L"TWO");
static const wstring	THREE(L"THREE");
static const wstring	FOUR(L"FOUR");
static const wstring	FIVE(L"FIVE");

// Keys
const unsigned int W_KEY = (unsigned int)'W';
const unsigned int A_KEY = (unsigned int)'A';
const unsigned int S_KEY = (unsigned int)'S';
const unsigned int D_KEY = (unsigned int)'D';
const unsigned int P_KEY = (unsigned int)'P';
const unsigned int T_KEY = (unsigned int)'T';
const unsigned int Z_KEY = (unsigned int)'Z';
const unsigned int ONE_KEY = (unsigned int)'1';
const unsigned int TWO_KEY = (unsigned int)'2';
const unsigned int CTRL_KEY = VK_CONTROL;
const unsigned int UP_KEY = VK_UP;
const unsigned int DOWN_KEY = VK_DOWN;
const unsigned int LEFT_KEY = VK_LEFT;
const unsigned int RIGHT_KEY = VK_RIGHT;
const unsigned int SPACE_KEY = VK_SPACE;
const unsigned int ENTER_KEY = VK_RETURN;
const unsigned int MOUSE_LEFT = VK_LBUTTON;
const unsigned int MOUSE_RIGHT = VK_RBUTTON;
const unsigned int ESCAPE_KEY = VK_ESCAPE;
