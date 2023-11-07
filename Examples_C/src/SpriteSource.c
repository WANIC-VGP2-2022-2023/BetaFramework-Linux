//------------------------------------------------------------------------------
//
// File Name:	SpriteSource.c
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 2
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BetaLow.h"
#include "SpriteSource.h"
#include "Stream.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct SpriteSource
{
	// The name of the sprite source.
	// A buffer is used to allow each sprite source to have a unique name.
	// The buffer is hardcoded to an arbitrary length that will be sufficient
	//	 for all CS230 assignments.  You may, instead, use dynamically-allocated
	//	 arrays for this purpose but the additional complexity is unnecessary
	//	 and it is your responsibility to ensure that the memory is successfully
	//	 allocated and deallocated in all possible situations.
	char name[32];

	// The dimensions of the sprite sheet.
	int	numRows;
	int	numCols;

	// Pointer to a texture created using the Alpha Engine.
	Texture* pTexture;

} SpriteSource;

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new sprite source object.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// Params:
//	 numCols = The number of columns in the sprite sheet.
//	 numRows = The number of rows in the sprite sheet.
//	 pTexture = A pointer to a texture that has been loaded by the Alpha Engine.
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
SpriteSourcePtr SpriteSourceCreate(int numCols, int numRows, Texture* pTexture)
{
	SpriteSourcePtr spriteSource = calloc(1, sizeof(SpriteSource));

	if (spriteSource)
	{
		spriteSource->numRows = numRows;
		spriteSource->numCols = numCols;
		spriteSource->pTexture = pTexture;
	}
	return spriteSource;
}

// Free the memory associated with a sprite source object.
// (NOTE: The texture should now be freed by this function.)
// (Also, set the sprite source pointer to NULL.)
// Params:
//	 spriteSource = Pointer to the sprite source pointer.
void SpriteSourceFree(SpriteSourcePtr * spriteSource)
{
	// Verify that a valid pointer was specified.
	if (spriteSource)
	{
		// Free any loaded texture.
		if ((*spriteSource)->pTexture)
		{
			TextureDestroy(&(*spriteSource)->pTexture);
		}

		// Free the allocated memory.
		free(*spriteSource);

		// Set the sprite source pointer to NULL.
		*spriteSource = NULL;
	}
}

// Create a SpriteSource using data read from a file.
// Params:
//	 stream = Pointer to the data stream used for reading.
// Returns:
//	 If the sprite source was created successfully,
//	   then return a pointer to the created sprite source,
//	   else return NULL.
SpriteSourcePtr SpriteSourceRead(Stream stream)
{
	SpriteSourcePtr spriteSource = NULL;

	if (stream)
	{
		const char * token = StreamReadToken(stream);
		if (strncmp(token, "SpriteSource", _countof("SpriteSource")) == 0)
		{
			spriteSource = SpriteSourceCreate(1, 1, NULL);

			token = StreamReadToken(stream);
			strncpy_s(spriteSource->name, _countof(spriteSource->name), token, strlen(token));

			spriteSource->numCols = StreamReadInt(stream);
			spriteSource->numRows = StreamReadInt(stream);

			token = StreamReadToken(stream);
			spriteSource->pTexture = TextureCreateFromFile(token, TM_Nearest);
			if(spriteSource->pTexture == NULL)
				printf("Failed to create texture [%s]!", token);
		}
	}

	return spriteSource;
}

//// Returns a pointer to the name of the specified sprite source.
//// Params:
////	 spriteSource = Pointer to the sprite source object.
//// Returns:
////	 If the sprite source pointer is valid,
////		then return a pointer to its name,
////		else return NULL.
//const char * SpriteSourceGetName(SpriteSourcePtr spriteSource)
//{
//	// Verify that a valid sprite source was specified.
//	if (spriteSource)
//	{
//		return spriteSource->name;
//	}
//	return NULL;
//}

// Determines if a sprite source has the specified name.
// Params:
//	 spriteSource = Pointer to the sprite source object.
//	 name = Pointer to the name to be compared.
// Returns:
//	 If the sprite source and name pointers are valid,
//		then perform a string comparison and return the result (match = true),
//		else return false.
bool SpriteSourceIsNamed(SpriteSourcePtr spriteSource, const char * name)
{
	bool result = false;

	// Verify that a valid sprite source was specified.
	if (spriteSource && name)
	{
		result = strncmp(spriteSource->name, name, _countof(spriteSource->name)) == 0;
	}
	return result;
}

// Returns a pointer to the texture, for the purposes of rendering a sprite.
// Params:
//	 spriteSource = Pointer to the sprite source object.
// Returns:
//	 If the sprite source pointer is valid,
//		then return a pointer to its texture,
//		else return NULL.
Texture* SpriteSourceGetTexture(SpriteSourcePtr spriteSource)
{
	// Verify that a valid sprite source was specified.
	if (spriteSource)
	{
		return spriteSource->pTexture;
	}
	return NULL;
}

// Returns the maximum number of frames possible, given the dimensions of the sprite sheet.
// (Hint: Frame count = numCols * numRows.)
// Params:
//	 spriteSource = Pointer to the sprite source object.
// Returns:
//	 If the sprite source pointer is valid,
//		then return the calculated frame count,
//		else return 0.
unsigned int SpriteSourceGetFrameCount(SpriteSourcePtr spriteSource)
{
	if (spriteSource)
	{
		return spriteSource->numCols * spriteSource->numRows;
	}

	return 0;
}

// Returns the UV coordinates of the specified frame in a sprite sheet.
// (Hint: Refer to the Week 2 lecture slides for the correct calculations.)
// Params:
//	 spriteSource = Pointer to the sprite source object.
//	 frameIndex = A frame index within a sprite sheet.
//   u = Pointer to a float to contain the U coordinate. 
//   v = Pointer to a float to contain the V coordinate. 
void SpriteSourceGetUV(SpriteSourcePtr spriteSource, unsigned int frameIndex, float * u, float * v)
{
	// Verify that all pointers are valid.
	if (spriteSource && u && v)
	{
		unsigned int maxIndex = SpriteSourceGetFrameCount(spriteSource) - 1;
		if (frameIndex > maxIndex)
		{
			frameIndex = 0;
		}
		*u = (1.0f / spriteSource->numCols) * (frameIndex % spriteSource->numCols);
		*v = (1.0f / spriteSource->numRows) * (frameIndex / spriteSource->numCols);
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

