/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	TiledLayer.h

	A TiledLayer manages a tiled background, which is basically
	a grid of images that together create a game background.
	TiledLayers are packed, meaning there is a tile in every
	grid cell.
*/
#pragma once
#include "stdafx.h"
#include "src\graphics\RenderList.h"
#include "src\gsm\world\Tile.h"
#include "src\gsm\world\WorldLayer.h"
#include "src\gui\Viewport.h"
#include "src\gsm\physics\CollidableObject.h"
#include "src\gsm\physics\Physics.h"
#include "src\gsm\physics\BoxPhysics.h"

/* Tile type determines what edges are collidable and on what sides 
	of the tile they are collidable. This will be used here for now
	later on when a new level loader is created I will use these there,
	and everything will be loaded in binary*/

struct TileProps{
	bool left_collidable;
	bool right_collidable;
	bool top_collidable;
	bool bottom_collidable;
};

// THIS CLASS IS-A WorldLayer

class TiledLayer: public WorldLayer
{
protected:
	// NOTE: tileLayout stores a grid of Tiles (rows X columns),
	// but does so using a one-dimensional vector. So, when
	// accessing and changing tiles in this data structure,
	// rows and columns must be converted into indices.
	vector<Tile*> *tileLayout;
	int columns;
	int rows;
	int tileWidth;
	int tileHeight;
	int layerWidth;
	int layerHeight;
	int z;

	vector<TileProps> * tilePropertyVector;

	// NOTE: z IS HERE IN CASE WE WANT TO LAYER SPRITES
	// IN FRONT OF OR BEHIND LAYERS IN A GAME WORLD

	// HELPER METHODS
	void initOverlappingCellRange(AABB *aabb, int &startCol, int &endCol, int &startRow, int &endRow);

public:
	// INLINED ACCESSOR METHODS
	int getColumns()			{ return columns;		}
	int getRows()				{ return rows;			}
	int getTileHeight()			{ return tileHeight;	}
	int getTileWidth()			{ return tileWidth;		}
	int getLayerWidth()			{ return layerWidth;	}
	int getLayerHeight()		{ return layerHeight;	}
	int getZ()					{ return z;				}

	// INLINED MUTATOR METHOD
	void setZ(int initZ)		{ z = initZ;			}

	// METHODS DEFINED IN TiledLayer.cpp
	TiledLayer(	int initColumns,	int initRows, 
				int initTileWidth,	int initTileHeight, 
				int initZ, 
				bool initCollidableTiles, 
				int initWorldWidth, int initWorldHeight,vector<TileProps> * tileProps);
	~TiledLayer();
	void	addRenderItemsToRenderList(RenderList *renderList, Viewport *viewport);
	void	addItemsToPhysicsSystem(Game *game);
	void	addTile(Tile *initTile);
	int		calculateAndSetLayerHeight();
	int		calculateAndSetLayerWidth();
	int		calculateViewportEndColumn(int viewportX, int viewportWidth);
	int		calculateViewportEndRow(int viewportY, int viewportHeight);
	int		calculateViewportStartColumn(int viewportX, int viewportWidth);
	int		calculateViewportStartRow(int viewportY, int viewportHeight);
	Tile*	getTile(int row, int column);
	int		getTileColumn(int index);
	int		getTileRow(int index);
	void	init(	int initColumns,	int initRows, 
					int initTileWidth,	int initTileHeight, 
					int initZ, 
					bool collidableTiles, 
					int worldWidth, int worldHeight, vector<TileProps> * tileProps);
	void	setTile(Tile *initTile, int row, int column);
	void	findTileCollisionsForSprite(	Physics *physics,
											CollidableObject *dynamicObject);
	bool	willSpriteCollideOnTile(	Physics *physics,
										CollidableObject *dynamicObject);

};