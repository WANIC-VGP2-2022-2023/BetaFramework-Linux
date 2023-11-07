//------------------------------------------------------------------------------
//
// File Name:	Font.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		Beta Engine
// Course:		WANIC VGP2
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

// Un-Windows
#ifdef DrawText
#undef DrawText
#endif

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

typedef struct Font Font;

//------------------------------------------------------------------------------
// Public Functions
//------------------------------------------------------------------------------

// Load a font from a file
BE_C_API Font* FontCreateFromFile(const char* filename, unsigned size);

// Destroys font data via FreeType
BE_C_API void FontDestroy(Font** font);

// Draw text to the screen using this font.
// (NOTE: Use GraphicsSetTransform to define where the
// text should appear on screen, as well as its world scale.)
// Params:
//   text = The string of text that should be drawn.
BE_C_API void FontDrawText(const Font* font, const char* text);

// Return the filename of the font.
BE_C_API const char* FontGetName(const Font* font);

// Return the size of the font.
BE_C_API unsigned FontGetSize(const Font* font);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif
