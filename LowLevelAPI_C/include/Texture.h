//------------------------------------------------------------------------------
//
// File Name:	Texture.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		Beta Engine
// Course:		WANIC VGP2
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Color.h"
#include "Vector2D.h"

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Public Constants:
//------------------------------------------------------------------------------

typedef enum TextureFilterMode
{
	TM_Nearest,
	TM_Bilinear,
	TM_Trilinear
}TextureFilterMode;

typedef struct Texture Texture;

//------------------------------------------------------------------------------
// Public Functions
//------------------------------------------------------------------------------

// Creates a 1x1 white texture
BE_C_API Texture* TextureCreateDefault();

// Loads a texture from an array
BE_C_API Texture* TextureCreateFromArray(Color* colors, size_t width, size_t height);

// Loads a texture from a file
BE_C_API Texture* TextureCreateFromFile(const char* filename,
	TextureFilterMode mode);

// Destroys texture data via OpenGL
BE_C_API void TextureDestroy(Texture** texture);

// Set this as the active texture
// Params:
//   uvOffset = The texture coordinate offset to use when rendering.
BE_C_API void TextureUse(Texture* texture, const Vector2D* uvOffset);

// Returns the name of the texture.
BE_C_API const char* TextureGetName(const Texture* texture);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif
