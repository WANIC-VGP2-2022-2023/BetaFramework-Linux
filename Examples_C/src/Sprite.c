//------------------------------------------------------------------------------
//
// File Name:	Sprite.c
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 4
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BetaLow.h"
#include "Matrix2D.h"
#include "Sprite.h"
#include "SpriteSource.h"
#include "SpriteSourceManager.h"
#include "MeshManager.h"
#include "Stream.h"
#include "Transform.h"
#include "Vector2D.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

// SpriteText Demo Code:
//#define SPRITETEXT

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct Sprite
{
	// The frame currently being displayed (for sprite sheets).
	unsigned int frameIndex;

	// The alpha transparency to use when drawing the sprite.
	float alpha;

	// The sprite source used when drawing (NULL = simple colored mesh).
	SpriteSourcePtr spriteSource;

	// The mesh used to draw the sprite.
	Mesh* mesh;

	// Zero-terminated string used to display sprite text.
	const char * text;

} Sprite;

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new sprite object.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// (NOTE: Make sure to initialize the 'alpha' member variable to 1.0f.)
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
SpritePtr SpriteCreate(void)
{
	// Allocate the memory for the sprite object.
	SpritePtr sprite = calloc(1, sizeof(Sprite));

	// Initialize any non-zero member variables.
	if (sprite)
	{
		//sprite->name = name;
		sprite->alpha = 1.0f;
	}
	return sprite;
}

// Dynamically allocate a clone of an existing sprite.
// (Hint: Perform a shallow copy of the member variables.)
// Params:
//	 other = Pointer to the component to be cloned.
// Returns:
//	 If 'other' is valid and the memory allocation was successful,
//	   then return a pointer to the cloned component,
//	   else return NULL.
SpritePtr SpriteClone(const SpritePtr other)
{
	// Validate the pointer.
	if (other)
	{
		// Allocate the memory for the sprite object.
		SpritePtr sprite = calloc(1, sizeof(Sprite));

		if (sprite)
		{
			*sprite = *other;
		}

		// Return the allocated memory (NULL if unsuccessful).
		return sprite;
	}
	return NULL;
}

// Free the memory associated with a sprite object.
// (Also, set the sprite pointer to NULL.)
// Params:
//	 sprite = Pointer to the sprite pointer.
void SpriteFree(SpritePtr * sprite)
{
	// Verify that a valid sprite was specified.
	if (sprite)
	{
		// Free the allocated memory.
		free(*sprite);

		// Set the sprite pointer to NULL.
		*sprite = NULL;
	}
}

// Read the properties of a Sprite component from a file.
// [NOTE: Read the frameIndex value using StreamReadInt.]
// [NOTE: Read the alpha value using StreamReadFloat.]
// [NOTE: Read a token and call MeshCreate(), passing the token.]
// Params:
//	 sprite = Pointer to the Sprite component.
//	 stream = Pointer to the data stream used for reading.
void SpriteRead(SpritePtr sprite, Stream stream)
{
	if (sprite && stream)
	{
		sprite->frameIndex = StreamReadInt(stream);
		sprite->alpha = StreamReadFloat(stream);

		const char * meshName = StreamReadToken(stream);
		if (meshName && strcmp(meshName, "None") != 0)
		{
			Mesh* mesh = MeshManagerBuild(meshName);
			SpriteSetMesh(sprite, mesh);
		}

		const char * spriteSourceName = StreamReadToken(stream);
		if (spriteSourceName && strcmp(spriteSourceName, "None") != 0)
		{
			SpriteSourcePtr spriteSource = SpriteSourceManagerBuild(spriteSourceName);
			SpriteSetSpriteSource(sprite, spriteSource);
		}
	}
}

// Draw a sprite (Sprite can be textured or untextured).
// Params:
//	 sprite = Pointer to the sprite object.
//   transform = Pointer to the sprite object's transform.
void SpriteDraw(const Sprite* sprite, TransformPtr transform)
{
	// Verify the sprite and sprite->mesh pointers are valid.
	if (sprite && sprite->mesh)
	{
		// Set shader to sprite shader
		ShaderProgramUse(GraphicsGetSpriteShader());

		// Set transparency to alpha.
		// Set the blend color to (0, 0, 0, 0).
		Color blendColor; 
		blendColor.r = 1, blendColor.g = 1, blendColor.b = 1, blendColor.a = sprite->alpha;
		GraphicsSetSpriteBlendColor(&blendColor);

		/*if (sprite->text != NULL)
		{
			Matrix2D matrix = *TransformGetMatrix(transform);
			Matrix2D offset;
			Matrix2DTranslate(&offset, TransformGetScale(transform)->x, 0.0f);

			const char * textPtr = sprite->text;
			while (*textPtr != '\0')
			{
				unsigned charFrame = (*textPtr) - ' ';
				MeshRender(sprite->mesh, sprite->spriteSource, &matrix, charFrame);
				Matrix2DConcat(&matrix, &offset, &matrix);
				++textPtr;
			}
		}
		else*/
		{
			//MeshRender(sprite->mesh, sprite->spriteSource, TransformGetMatrix(transform), sprite->frameIndex);
			Vector2D uv;
			SpriteSourceGetUV(sprite->spriteSource, sprite->frameIndex, &uv.x, &uv.y);
			TextureUse(SpriteSourceGetTexture(sprite->spriteSource), &uv);
			GraphicsSetTransformMatrix(TransformGetMatrix(transform));
			MeshDraw(sprite->mesh);
		}
	}
}

// Get a sprite's alpha value.
// Params:
//	 sprite = Pointer to the sprite object.
// Returns:
//	 If the pointer is valid,
//		then return the sprite's alpha value (a value between 0.0f and 1.0f),
//		else return 0.0f.
float SpriteGetAlpha(SpritePtr sprite)
{
	if (sprite)
	{
		return sprite->alpha;
	}
	return 0;
}

// Set a sprite's alpha value.
// (NOTE: Make sure to clamp the resulting alpha value between 0.0f and 1.0f, as the
//	input value might be outside of this range.)
// (HINT: See AEClamp in AEMath.h.)
// Params:
//	 sprite = Pointer to the sprite object.
//   newAlpha = The sprite's new alpha value.
void SpriteSetAlpha(SpritePtr sprite, float newAlpha)
{
	sprite->alpha = Clamp(newAlpha, 0.0f, 1.0f);
}

// Set the sprite's current frame.
// (NOTE: You must verify that the frameIndex parameter is within the
//	range [0 to frame count] before changing the sprite's frame index!)
// Params:
//	 sprite = Pointer to the sprite object.
//   frameIndex = New frame index for the sprite (0 .. frame count).
// ADDITIONAL REQUIREMENTS:
// - This function must make the following function call:
//     TraceMessage("SpriteSetFrame: frame index = %d", frameIndex);
void SpriteSetFrame(SpritePtr sprite, unsigned int frameIndex)
{
	if (sprite)
	{
		if (frameIndex < SpriteSourceGetFrameCount(sprite->spriteSource))
		{
			sprite->frameIndex = frameIndex;
		}

		//printf("SpriteSetFrame: frame index = %d\n", frameIndex);
	}
}

// Set the sprite's mesh.
// (NOTE: This mesh may be textured or untextured.)
// (NOTE: This mesh may contain any number of triangles.)
// Params:
//	 sprite = Pointer to the sprite object.
//   mesh = Pointer to a mesh created using the Alpha Engine.
void SpriteSetMesh(SpritePtr sprite, Mesh* mesh)
{
	if (sprite)
	{
		sprite->mesh = mesh;
	}
}

// Set a new SpriteSource for the specified sprite.
// (NOTE: The spriteSource parameter may be NULL.  This will remove an existing
//	texture from a sprite and cause the sprite to be displayed as a colored mesh.)
// Params:
//	 sprite = Pointer to the sprite to be modified.
//	 spriteSource = A new sprite source for the sprite (this pointer may be NULL).
void SpriteSetSpriteSource(SpritePtr sprite, SpriteSourcePtr spriteSource)
{
	if (sprite)
	{
		sprite->spriteSource = spriteSource;
	}
}

#ifdef SPRITETEXT
// SpriteText Demo Code:
// Assign a text string to a sprite object.  This will allow a sequence of
//	 characters to be displayed as text.
// (NOTE: The text parameter may be NULL.  This will remove an existing text
//	string from a sprite and cause the sprite to be displayed normally.)
// Params:
//	 sprite = Pointer to the sprite to be modified.
//	 text = Pointer to a zero-terminated array of characters.
void SpriteSetText(SpritePtr sprite, const char * text)
{
	if (sprite)
	{
		sprite->text = text;
	}
}
#endif

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

