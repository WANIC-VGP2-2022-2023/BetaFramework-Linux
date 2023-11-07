//------------------------------------------------------------------------------
//
// File Name:	SpriteSourceManager.c
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 5
// Course:		CS230SU19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "SpriteSource.h"
#include "SpriteSourceManager.h"
#include "Stream.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

#define spriteSourceListSize 10

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct SpriteSourceManager
{
	// The number of sprite sources currently in the list.
	unsigned int spriteSourceCount;

	// A list of all currently loaded sprite sources.
	// This list can be a fixed-length array (minimum size of 10 entries)
	// or a dynamically-sized array, such as a linked list.
	SpriteSourcePtr	spriteSourceList[spriteSourceListSize];

} SpriteSourceManager;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

static SpriteSourceManager spriteSources;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static void SpriteSourceManagerAdd(SpriteSourcePtr spriteSource);
static SpriteSourcePtr SpriteSourceManagerFind(const char * spriteSourceName);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Initialize the Sprite Source Manager.
// (NOTE: Make sure to initialize all memory to zero.)
void SpriteSourceManagerInit()
{
	// Clear out the entire memory structure.
	memset(&spriteSources, 0, sizeof(spriteSources));
}

// Create a SpriteSource and add it to the SpriteSource manager.
// 1: Call SpriteSourceManagerFind() to determine if the sprite source already exists
// 2: If the named sprite source does not already exist,
//	  a: Use sprintf_s() to construct a path name using SpriteSourceName
//	     (HINT: The correct path name should be constructed using "Data/&s.txt".)
//	  b: Call StreamOpen(), passing the pathname
//    c: If the stream was opened successfully,
//		 1: Call SpriteSourceRead() to construct a SpriteSource using data read from the file
//		 2: Call SpriteSourceManagerAdd(), passing the created SpriteSource
// 3: Return the SpriteSource (already existing or just created)
// Params:
//	 SpriteSourceName = The name of the SpriteSource to be created.
// Returns:
//	 If the SpriteSource already exists or was created successfully,
//	   then return a pointer to the SpriteSource,
//	   else return NULL.
SpriteSourcePtr SpriteSourceManagerBuild(const char * spriteSourceName)
{
	// Verify that a valid name was specified.
	if (!spriteSourceName)
	{
		return NULL;
	}

	// Search for an existing copy of the requested sprite source.
	SpriteSourcePtr spriteSource = SpriteSourceManagerFind(spriteSourceName);

	// If an existing copy was not found, then create a new one from the data file.
	if (spriteSource == NULL)
	{
		// Construct the file pathname.
		char pathName[FILENAME_MAX] = "";
		sprintf_s(pathName, _countof(pathName), "Data/%s.txt", spriteSourceName);

		// Open the stream.
		Stream stream = StreamOpen(pathName);
		if (stream)
		{
			// Read the contents of the sprite source from the stream.
			spriteSource = SpriteSourceRead(stream);
			SpriteSourceManagerAdd(spriteSource);
		}
	}
	return spriteSource;
}

// Free all SpriteSourcees in the SpriteSource Manager.
// (NOTE: Make sure to call SpriteSourceFree() to free each SpriteSource.)
// (HINT: The list should contain nothing but NULL pointers once this function is done.)
void SpriteSourceManagerFreeAll()
{
	for (int i = 0; i < spriteSourceListSize; i++)
	{
		if (spriteSources.spriteSourceList[i] != NULL)
		{
			SpriteSourceFree(&(spriteSources.spriteSourceList[i]));
		}
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

static void SpriteSourceManagerAdd(SpriteSourcePtr spriteSource)
{
	if (spriteSource)
	{
		if (spriteSources.spriteSourceCount < _countof(spriteSources.spriteSourceList))
		{
			spriteSources.spriteSourceList[spriteSources.spriteSourceCount] = spriteSource;
			++spriteSources.spriteSourceCount;
		}
	}
}

static SpriteSourcePtr SpriteSourceManagerFind(const char * spriteSourceName)
{
	for (int i = 0; i < spriteSourceListSize; i++)
	{
		if (spriteSources.spriteSourceList[i] != NULL)
		{
			if (SpriteSourceIsNamed(spriteSources.spriteSourceList[i], spriteSourceName))
			{
				return spriteSources.spriteSourceList[i];
			}
		}
	}
	return NULL;
}
