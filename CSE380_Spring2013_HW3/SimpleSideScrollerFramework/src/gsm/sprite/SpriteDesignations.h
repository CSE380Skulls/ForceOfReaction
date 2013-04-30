#pragma once

/**
	The GameState dictates how to handle user input with regards to 
	processing GUI interactions. Each state potentially represents
	a different GUIScreen.
**/
enum SpriteDesignations
{
	// Main Player
	PLAYER_DESIGNATION,
	// A Bot
	BOT_DESIGNATION, 
	// A Projectile
	PROJECTILE_DESIGNATION,
	// A Wall (not a tile, this type of wall can be destroyed)
	WALL_DESIGNATION,
	// This sprite got hit
	SPRITE_HIT,
	// This sprite just died
	SPRITE_DEAD,
};