//------------------------------------------------------------------------------
//
// File Name:	Main.c
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2019
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <BetaLow.h>

#include "GameStateManager.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Main function
int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE previous, 
	_In_ LPSTR commandLine, _In_ int commandShow)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif

	UNREFERENCED_PARAMETER(instance);
	UNREFERENCED_PARAMETER(previous);
	UNREFERENCED_PARAMETER(commandLine);
	UNREFERENCED_PARAMETER(commandShow);

	// Set initial game state

	// Add additional modules to engine

	// Set default paths for resources
	EngineSetFilePath("../Assets/");

	// Register custom components with the object factory

	// Game engine goes!
	StartupSettings settings =
	{
		800, // window width
		600, // window height
		60,  // framerateCap (no effect due to vsync)
		true, // show the window when the engine starts
		true, // create a debug console in Debug mode
		false, // do not start in fullscreen mode
		false, // no maximize button
		true, // close application automatically when escape is pressed
		true, // vsync is on
	};

	// Create GSM
	GameLoopFunctionTable table =
	{
		NULL,						// load
		GameStateManagerInit,		// init
		GameStateManagerUpdate,		// update
		NULL,						// fixed update
		NULL,						// draw
		GameStateManagerShutdown,	// shutdown
		NULL,						// unload
	};
	BetaObject* gameStateManager = BetaObjectCreate("GameStateManager", NULL, &table, NULL);

	// Set initial game state
	GameStateManagerSetNextState(GsLevel1);
	
	// Add modules
	EngineAddModule(gameStateManager);

	// Start the engine!
	EngineStart(&settings);

	return 0;
}

//------------------------------------------------------------------------------
