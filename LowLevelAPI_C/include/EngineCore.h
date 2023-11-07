//------------------------------------------------------------------------------
//
// File Name:	EngineC.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

typedef struct StartupSettings
{
	// Width of the window. Set to 0 to use native resolution.
	unsigned windowWidth;
	// Height of the window. Set to 0 to use native resolution.
	unsigned windowHeight;
	// Maximum framerate. Has no effect if vSync is enabled.
	unsigned framerateCap;
	// Whether to show the window immediately when the engine starts.
	bool showWindow;
	// Whether to create a debug console in Debug mode.
	bool debugConsole;
	// Whether to start in fullscreen mode.
	bool fullscreen;
	// Whether the user is allowed to maximize the window.
	bool allowMaximize;
	// Whether to close the application when escape is pressed.
	bool closeOnEscape;
	// Whether to synchronize sync the frame rate with the refresh rate.
	bool vSync;
}StartupSettings;

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

typedef struct BetaObject BetaObject;

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Starts the game loop and runs until quit state is reached.
// Params:
//   settings = The settings to use when starting the application.
BE_C_API void EngineStart(const StartupSettings* settings);

// Stops the engine and shuts everything down
BE_C_API void EngineStop();

// Add an additional module to the engine, which will be updated.
// Params:
//   module = A pointer to the module that will be added to the engine. 
//      This module should be dynamically allocated.
BE_C_API void EngineAddModule(BetaObject* module);

// Retrieves the module with the given name if it exists.
// Params:
//   name = The name of the module you want to retrieve.
// Return:
//   A pointer to the specified module, or NULL if it does not exist.
BE_C_API BetaObject* EngineGetModule(const char* name);

// Returns the path that contains all assets
BE_C_API const char* EngineGetFilePath();

// Sets the path used for assets
// Params:
//   directory = The relative path to the master assets directory. Textures, 
//      objects, shaders, and sound files should be placed in subdirectories within this directory.
BE_C_API void EngineSetFilePath(const char* directory);

// Set whether pressing the escape key will close the program.
BE_C_API void EngineSetCloseOnEscape(bool closeOnEscape);

//-----------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif
