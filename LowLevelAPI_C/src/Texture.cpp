//------------------------------------------------------------------------------
//
// File Name:	Texture.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		Beta Engine
// Course:		WANIC VGP2
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Texture.h"

#include "../../LowLevelAPI/include/Texture.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions
//------------------------------------------------------------------------------

// Creates a 1x1 white texture
Texture* TextureCreateDefault()
{
	Beta::Texture* texture = new Beta::Texture();
	return reinterpret_cast<Texture*>(texture);
}

// Loads a texture from an array
Texture* TextureCreateFromArray(Color* colors, size_t width, size_t height)
{
	Beta::Array<Beta::Color> colorsArray;
	colorsArray.Reserve(width * height);
	for (size_t i = 0; i < width * height; ++i)
	{
		colorsArray.PushBack(Beta::Color(colors[i].r, colors[i].g, colors[i].b, colors[i].a));
	}

	Beta::Texture* texture = new Beta::Texture(colorsArray, width, height);
	return reinterpret_cast<Texture*>(texture);
}

// Loads a texture from a file
Texture* TextureCreateFromFile(const char* filename,
	TextureFilterMode mode)
{
	Beta::Texture* texture = Beta::Texture::CreateTextureFromFile(
		filename, static_cast<Beta::TextureFilterMode>(mode));
	return reinterpret_cast<Texture*>(texture);
}

// Destroys texture data via OpenGL
void TextureDestroy(Texture** texture)
{
	delete reinterpret_cast<Beta::Texture*>(*texture);
	*texture = NULL;
}

// Set this as the active texture
// Params:
//   uvOffset = The texture coordinate offset to use when rendering.
void TextureUse(Texture* texture, const Vector2D* uvOffset)
{
	reinterpret_cast<Beta::Texture*>(texture)->Use(
		*reinterpret_cast<const Beta::Vector2D*>(uvOffset));
}

// Returns the name of the texture.
const char* TextureGetName(const Texture* texture)
{
	return reinterpret_cast<const Beta::Texture*>(texture)->GetName().c_str();
}

//------------------------------------------------------------------------------
