//------------------------------------------------------------------------------
//
// File Name:	EngineCore.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright � 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "EngineCore.h"

// Systems
#include "Random.h"		// Init
#include "WindowSystem.h"	// Init
#include "Input.h"		// Init, CheckTriggered
#include "GraphicsEngine.h"	// StartFrame, EndFrame
#include "MeshFactory.h"	// Constructor
#include "DebugDraw.h"	// Draw
#include "FrameRateController.h" // GetFrameTime, FrameEnd

#define VK_ESCAPE 0
//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Starts the game loop and runs until quit state is reached.
	// Params:
	//   settings = The settings to use when starting the application.
	void EngineCore::Start(const Beta::StartupSettings& settings)
	{
		closeOnEscape = settings.closeOnEscape;

		// Initialize the WindowSystem (Windows, Message Handlers)
		system->Initialize(settings);

		// Init graphics
		const Vector2D& resolution = system->GetResolution(); // NOTE: WindowSystem may have changed resolution during initialization
		graphics->Initialize(static_cast<unsigned>(resolution.x), static_cast<unsigned>(resolution.y));
		graphics->SetUseVSync(settings.vSync);

		// Init debug draw
		debugDraw->Initialize();

		// Initialize the frame rate controller.
		frameRateController->Initialize(settings.framerateCap);

		// Initialize custom engine modules
		Initialize();

		// If game is running
		while (isRunning)
		{
			// Update the game using the previous frame running time
			Update(frameRateController->GetFrameTime());
		}

		// Shutdown custom engine modules
		Shutdown();
	}

	// Stops the engine and shuts everything down
	void EngineCore::Stop()
	{
		isRunning = false;
	}

	// Add an additional module to the engine, which will be updated.
	/*void EngineCore::AddModule(BetaObject * module)
	{
		modules.emplace(typeid(module), module);
		module->SetOwner(this);
	}*/

	// Returns the path that contains all assets
	const std::string & EngineCore::GetFilePath() const
	{
		return assetsPath;
	}

	// Sets the path used for assets
	void EngineCore::SetFilePath(const std::string & directory)
	{
		assetsPath = directory;
	}

	// Return true if the engine is set to close when escape is pressed.
	bool EngineCore::DoesCloseOnEscape() const
	{
		return closeOnEscape;
	}

	// Set whether pressing the escape key will close the program.
	void EngineCore::SetCloseOnEscape(bool closeOnEscape_)
	{
		closeOnEscape = closeOnEscape_;
	}

	// Return true if the engine is currently running, false otherwise.
	bool EngineCore::IsRunning() const
	{
		return isRunning;
	}

	// Retrieve the instance of the EngineCore singleton
	EngineCore& EngineCore::GetInstance()
	{
		static EngineCore instance;
		return instance;
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Constructor is private to prevent accidental instantiation
	EngineCore::EngineCore()
		: BetaObject("EngineCore"), isRunning(true), assetsPath("Assets/"), closeOnEscape(true),
		system(nullptr), input(nullptr), graphics(nullptr), meshFactory(nullptr), debugDraw(nullptr),
			frameRateController(nullptr)
	{
		// Initialize random number generator
		Random::Init();

		// Create the WindowSystem (Windows, Message Handlers)
		system = new WindowSystem();

		// Initialize input.
		input = new Input();

		// Create Graphics
		graphics = new GraphicsEngine();
		meshFactory = new MeshFactory();

		// Create DebugDraw
		debugDraw = new DebugDraw();

		// Create the frame rate controller.
		frameRateController = new FrameRateController();
	}

	// Ditto for destructor
	EngineCore::~EngineCore()
	{
		// Shutdown FRC
		delete frameRateController;

		// Shutdown graphics
		delete debugDraw;
		delete meshFactory;
		delete graphics;

		// Shutdown the WindowSystem (Windows, Event Handlers).
		delete input;
		delete system;
	}

	// Initialize custom modules
	void EngineCore::Initialize()
	{
		// Initialize all extra modules
		//size_t numModules = modules.Size();
		//for (size_t i = 0; i < numModules; ++i)
		for(auto it = moduleList.Begin(); it != moduleList.End(); ++it)
		{
			(*it)->Initialize();
		}
	}

	// Update all engine modules.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void EngineCore::Update(float dt)
	{
		// Handling Input
		input->Update();

		// Tell graphics module that a new frame is starting
		graphics->FrameStart();

		// Update extra modules
		//size_t numModules = modules.Size();
		//for (size_t i = 0; i < numModules; ++i)
		for (auto it = moduleList.Begin(); it != moduleList.End(); ++it)
		{
			(*it)->Update(dt);
		}

		// Draw extra modules
		//for (size_t i = 0; i < numModules; ++i)
		for (auto it = moduleList.Begin(); it != moduleList.End(); ++it)
		{
			(*it)->Draw();
		}

		// Draw debug lines
		debugDraw->Draw();

		// Swap buffers
		graphics->FrameEnd();

		// Complete the draw process for the current game loop.
		system->Draw();

		// Sleep
		frameRateController->FrameEnd();

		// Check if forcing the application to quit
		if ((closeOnEscape && input->CheckTriggered(VK_ESCAPE))
			|| !system->DoesWindowExist())
		{
			Stop();
		}
	}

	// Shutdown custom modules.
	void EngineCore::Shutdown()
	{
		//--------------------------------------------------------------------------
		// NOTE: Most modules should be shutdown in reverse order.
		//--------------------------------------------------------------------------

		for (auto it = moduleList.ReverseBegin(); it != moduleList.ReverseEnd(); )
		{
			(*it)->Shutdown();
			moduleMap.erase(typeid(**it));
			delete (*it);
			it = moduleList.Erase(it);
		}
		moduleList.Clear();
		moduleMap.clear();
	}
}

//------------------------------------------------------------------------------
