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

// Low Level API C++ Systems
#include "EngineModulesGlobal.h"
#include "Random.h"
#include "../../LowLevelAPI/include/EngineCore.h"
#include "../../LowLevelAPI/include/WindowSystem.h"
#include "../../LowLevelAPI/include/Input.h"
#include "../../LowLevelAPI/include/GraphicsEngine.h"
#include "../../LowLevelAPI/include/DebugDraw.h"
#include "../../LowLevelAPI/include/FrameRateController.h"

// Objects
#include "BetaObject.h"

//------------------------------------------------------------------------------

namespace
{
	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	// Custom C-style modules
	Beta::Array<BetaObject*> modules;

	//------------------------------------------------------------------------------
	// Private Function Declarations:
	//------------------------------------------------------------------------------

	void EngineInitialize();
	void EngineUpdate(float dt);
	void EngineShutdown();
}

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Starts the game loop and runs until quit state is reached.
// Params:
//   settings = The settings to use when starting the application.
void EngineStart(const StartupSettings* settings)
{
	EngineSetCloseOnEscape(settings->closeOnEscape);

	// Initialize random number generator
	RandomInit();

	// Initialize the WindowSystem (Windows, Message Handlers)
	Beta::windows = EngineGetModule(Beta::WindowSystem);
	Beta::windows->Initialize(*reinterpret_cast<const Beta::StartupSettings*>(settings));

	// Initialize input.
	Beta::input = EngineGetModule(Beta::Input);

	// Initialize Graphics
	const Beta::Vector2D& resolution = Beta::windows->GetResolution(); // NOTE: WindowSystem may have changed resolution during initialization
	Beta::graphics = EngineGetModule(Beta::GraphicsEngine);
	Beta::graphics->Initialize(static_cast<unsigned>(resolution.x), static_cast<unsigned>(resolution.y));
	Beta::graphics->SetUseVSync(settings->vSync);
	Beta::meshFactory = EngineGetModule(Beta::MeshFactory);

	// Initialize DebugDraw
	Beta::debugDraw = EngineGetModule(Beta::DebugDraw);
	Beta::debugDraw->Initialize();

	// Initialize the frame rate controller.
	Beta::frameRateController = EngineGetModule(Beta::FrameRateController);
	Beta::frameRateController->Initialize(settings->framerateCap);

	// Initialize custom engine modules
	EngineInitialize();

	// If game is running
	while (Beta::engine.IsRunning())
	{
		// Update the game using the previous frame running time
		EngineUpdate(Beta::frameRateController->GetFrameTime());
	}

	// Shutdown custom engine modules
	EngineShutdown();
}

// Stops the engine and shuts everything down
void EngineStop()
{
	Beta::EngineCore::GetInstance().Stop();
}

// Add an additional module to the engine, which will be updated.
void EngineAddModule(BetaObject* module)
{
	modules.PushBack(module);
}

#ifdef EngineGetModule
#undef EngineGetModule

// Retrieves the module with the given name if it exists.
// Params:
//   name = The name of the module you want to retrieve.
// Return:
//   A pointer to the specified module.
BetaObject* EngineGetModule(const char* name)
{
	size_t numModules = modules.Size();
	for (size_t i = 0; i < numModules; ++i)
	{
		// If this is the one we're looking for, return it
		if (BetaObjectGetName(modules[i]) == name)
		{
			return modules[i];
		}
	}

	return nullptr;
}

#endif

// Returns the path that contains all assets
const char* EngineGetFilePath()
{
	return Beta::engine.GetFilePath().c_str();
}

// Sets the path used for assets
void EngineSetFilePath(const char* directory)
{
	Beta::engine.SetFilePath(directory);
}

// Set whether pressing the escape key will close the program.
void EngineSetCloseOnEscape(bool closeOnEscape_)
{
	Beta::engine.SetCloseOnEscape(closeOnEscape_);
}

//------------------------------------------------------------------------------
// Private Function Definitions:
//------------------------------------------------------------------------------

namespace
{
	void EngineInitialize()
	{
		size_t numModules = modules.Size();
		for (size_t i = 0; i < numModules; ++i)
		{
			const GameLoopFunctionTable* table = BetaObjectGetFunctionTable(modules[i]);
			if (table->initialize)
			{
				table->initialize(modules[i]);
			}
		}
	}

	void EngineUpdate(float dt)
	{
		// Handling Input
		Beta::input->Update();

		// Tell graphics module that a new frame is starting
		Beta::graphics->FrameStart();

		// Update extra modules
		size_t numModules = modules.Size();
		for (size_t i = 0; i < numModules; ++i)
		{
			const GameLoopFunctionTable* table = BetaObjectGetFunctionTable(modules[i]);
			if(table->update != NULL)
				table->update(modules[i], dt);

			if (table->draw != NULL)
				table->draw(modules[i]);
		}

		// Draw debug lines
		Beta::debugDraw->Draw();

		// Swap buffers
		Beta::graphics->FrameEnd();

		// Complete the draw process for the current game loop.
		Beta::windows->Draw();

		// Sleep
		Beta::frameRateController->FrameEnd();

		// Check if forcing the application to quit
		if ((Beta::engine.DoesCloseOnEscape() && Beta::input->CheckTriggered(VK_ESCAPE))
			|| !Beta::windows->DoesWindowExist())
		{
			Beta::engine.Stop();
		}
	}

	void EngineShutdown()
	{
		// Shutdown and destroy extra modules
		int numModules = static_cast<int>(modules.Size());
		for (int i = numModules - 1; i >= 0; --i)
		{
			const GameLoopFunctionTable* table = BetaObjectGetFunctionTable(modules[i]);
			if (table->shutdown)
			{
				table->shutdown(modules[i]);
			}
			BetaObjectDestroy(&modules[i]);
		}
	}
}
