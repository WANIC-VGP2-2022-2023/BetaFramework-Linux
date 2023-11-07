//------------------------------------------------------------------------------
//
// File Name:	Main.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

// BetaFramework Engine
#include <BetaHigh.h>

// Initial game state
#include "Test.h"
#include "Level1.h"

// Custom components
#include "Behaviors.h"

//------------------------------------------------------------------------------

using namespace Beta;

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Register custom components with the object factory
void RegisterComponents()
{
	EngineCore& engine = EngineCore::GetInstance();
	GameObjectFactory* factory = engine.AddModule<GameObjectFactory>();
	factory->RegisterComponent<Asteroid>();
	factory->RegisterComponent<Button>();
	factory->RegisterComponent<CameraFollow>();
	factory->RegisterComponent<Enemy>();
	factory->RegisterComponent<MonkeyAnimation>();
	factory->RegisterComponent<MonkeyMovement>();
	factory->RegisterComponent<PlayerProjectile>();
	factory->RegisterComponent<PlayerShip>();
	factory->RegisterComponent<RaycastTest>();
	factory->RegisterComponent<ScreenWrap>();
	factory->RegisterComponent<TimedDeath>();
}

// Main function
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif

	// Construct engine
	EngineCore& engine = EngineCore::GetInstance();

	// Add additional modules to engine
	engine.AddModule<SoundManager>();
	engine.AddModule<EventManager>();

	// Set initial level
	SpaceManager* spaceManager = engine.AddModule<SpaceManager>();
	spaceManager->GetDefaultSpace().SetLevel<Level1>();

	// Set default paths for resources
	engine.SetFilePath("../Assets/");

	// Register custom components with the object factory
	RegisterComponents();

	// Disable debug draw
	//EngineGetModule(DebugDraw)->SetEnabled(false);

	// Game engine goes!
	StartupSettings settings;
	settings.vSync = true;
	settings.framerateCap = 59;

	engine.Start(settings);

	return 0;
}

//------------------------------------------------------------------------------
