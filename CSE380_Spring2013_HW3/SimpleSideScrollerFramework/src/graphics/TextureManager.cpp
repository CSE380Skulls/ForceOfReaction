/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	TextureManager.cpp

	See TextureManager.h for a class description.
*/

#include "stdafx.h"
#include "src\game\WStringTable.h"
#include "src\graphics\TextureManager.h"

/*
	TextureManager - Default Constructor, it creates the WStringTable.
*/
TextureManager::TextureManager()  
{

}

/*
	~TextureManager - Destructor, it cleans up the WStringTable pointer.
*/
TextureManager::~TextureManager() 
{
	// TODO Delete strings in string table
}