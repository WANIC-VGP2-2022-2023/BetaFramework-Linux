//------------------------------------------------------------------------------
//
// File Name:	SpriteSourceManager.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 5
// Course:		CS230SU19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct SpriteSource *SpriteSourcePtr;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// An example of the structure to be defined in SpriteSourceManager.c.
#if 0
// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
	typedef struct SpriteSourceManager
	{
		// The number of sprite sources currently in the list.
		unsigned int spriteSourceCount;

		// A list of all currently loaded sprite sources.
		// This list can be a fixed-length array (minimum size of 10 entries)
		// or a dynamically-sized array, such as a linked list.
		SpriteSourcePtr	spriteSourceList[spriteSourceListSize];

	} SpriteSourceManager;
#endif

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Initialize the Sprite Source Manager.
// (NOTE: Make sure to initialize all memory to zero.)
void SpriteSourceManagerInit();

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
SpriteSourcePtr SpriteSourceManagerBuild(const char * spriteSourceName);

// Free all SpriteSourcees in the SpriteSource Manager.
// (NOTE: Make sure to call SpriteSourceFree() to free each SpriteSource.)
// (HINT: The list should contain nothing but NULL pointers once this function is done.)
void SpriteSourceManagerFreeAll();

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif

