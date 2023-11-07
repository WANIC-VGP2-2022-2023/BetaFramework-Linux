//------------------------------------------------------------------------------
//
// File Name:	Main.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework Map Editor
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

// BetaFramework Engine
#include <BetaHigh.h>

// Initial game state
#include "EditorLevel.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Main function
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif

	using namespace Beta;

	// Set initial game state
	

	// Add additional modules to engine
	EngineCore& engine = EngineCore::GetInstance();
	engine.AddModule<EventManager>();
	engine.AddModule<GameObjectFactory>();
	SpaceManager* spaceManager = engine.AddModule<SpaceManager>();
	spaceManager->GetDefaultSpace().SetLevel<Editor::EditorLevel>();

	// Set default paths for resources
	engine.SetFilePath("../Assets/");

	// Disable debug draw
	//EngineGetModule(DebugDraw)->SetEnabled(false);

	// Game engine goes!
	Beta::StartupSettings settings;
	settings.fullscreen = false;
	settings.windowWidth = 1024;
	settings.windowHeight = 768;
	engine.Start(settings);

	return 0;
}

//------------------------------------------------------------------------------
