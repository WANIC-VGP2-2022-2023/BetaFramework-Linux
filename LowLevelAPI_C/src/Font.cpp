//------------------------------------------------------------------------------
//
// File Name:	Font.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		Beta Engine
// Course:		WANIC VGP2
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Font.h"

#include "../../LowLevelAPI/include/Font.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions
//------------------------------------------------------------------------------

// Load a font from a file
Font* FontCreateFromFile(const char* filename,
	unsigned size)
{
	Beta::Font* font = Beta::Font::CreateFontFromFile(
		filename, size);
	return reinterpret_cast<Font*>(font);
}

// Destroys font data via OpenGL
void FontDestroy(Font** font)
{
	delete reinterpret_cast<Beta::Font*>(*font);
	*font = NULL;
}

// Draw text to the screen using this font.
// (NOTE: Use GraphicsSetTransform to define where the
// text should appear on screen, as well as its world scale.)
// Params:
//   text = The string of text that should be drawn.
void FontDrawText(const Font* font, const char* text)
{
	reinterpret_cast<const Beta::Font*>(font)->DrawText(text);
}

// Returns the name of the font.
const char* FontGetName(const Font* font)
{
	return reinterpret_cast<const Beta::Font*>(font)->GetName().c_str();
}

// Return the size of the font.
unsigned FontGetSize(const Font* font)
{
	return reinterpret_cast<const Beta::Font*>(font)->GetSize();
}

//------------------------------------------------------------------------------
