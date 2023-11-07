//------------------------------------------------------------------------------
//
// File Name:	WindowSystem.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		Beta Engine
// Course:		CS230
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "Vector2D.h"

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

typedef struct StartupSettings StartupSettings;

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Set the title of the window being used by Beta Framework.
// Params: 
//   text = The string that the window title will be set to.
BE_C_API void SystemSetWindowTitle(const char* text);

// Test whether the application is in fullscreen mode.
BE_C_API bool SystemIsFullScreen();

// Set whether the window is fullscreen.
BE_C_API void SystemSetFullScreen(bool fullscreen);

// Set the resolution of the window.
// Params:
//   width = The new width of the window.
//   height = The new height of the window.
BE_C_API void SystemSetResolution(unsigned width, unsigned height);

// Get the current resolution of the window.
BE_C_API Vector2D SystemGetResolution();

//-----------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif
