//------------------------------------------------------------------------------
//
// File Name:	Space.cpp
// Author(s):	Jeremy Kings
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Space.h"

#include <string>
#include "EventManager.h"
#include "SoundManager.h"
#include "ResourceManager.h"
#include <EngineCore.h> // GetModule
#include <GraphicsEngine.h> // GetDefaultCamera

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor(s)
	Space::Space(const std::string& name, bool customCamera)
		: BetaObject(name), objectManager(this), currentLevel(nullptr),
		nextLevel(nullptr), camera(nullptr), customCamera(customCamera), isDestroyed(false), paused(false)
	{
		if (customCamera)
		{
			camera = new Camera();
		}
		else
		{
			camera = &EngineGetModule(GraphicsEngine)->GetDefaultCamera();
		}
	}

	// Destructor
	Space::~Space()
	{
		// Shutdown all systems
		Shutdown();

		// Delete the custom camera
		if (customCamera)
			delete camera;

		// Delete level if it exists
		if (currentLevel)
			delete currentLevel;
	}

	// Updates the state manager and object manager.
	void Space::Update(float dt)
	{
		// Initiate level change
		if (nextLevel != nullptr)
			ChangeLevel();

		// Update the current level
		if (currentLevel && !paused)
			currentLevel->Update(dt);

		// Update the game object manager.
		objectManager.Update(dt);
	}

	void Space::Draw()
	{
		// Tell graphics to use our camera
		camera->Use();

		// Draw the current level
		if (currentLevel)
			currentLevel->Draw();

		// Draw objects in the manager
		objectManager.Draw();
	}

	// Shuts down the object manager
	void Space::Shutdown()
	{
		// Shutdown current level
		if (currentLevel)
		{
			currentLevel->Shutdown();
			currentLevel->Unload();
			delete currentLevel;
			currentLevel = nullptr;
		}

		// Destroy objects
		objectManager.Shutdown();
	}

	// Accessors
	bool Space::IsPaused() const
	{
		return paused;
	}

	// Returns the name of the level currently running in this space.
	std::string Space::GetLevelName() const
	{
		if (currentLevel != nullptr)
			return currentLevel->GetName();
		else
			return "no level currently loaded in space";
	}

	// Returns the object manager, which you can use to retrieve and add objects.
	GameObjectManager& Space::GetObjectManager()
	{
		return objectManager;
	}

	// Pauses the space, preventing objects from being updated, but objects are still drawn.
	void Space::SetPaused(bool value)
	{
		paused = value;
	}

	// Restarts the current level (next level = current)
	void Space::RestartLevel()
	{
		nextLevel = currentLevel;
	}

	bool Space::IsDestroyed() const
	{
		return isDestroyed;
	}

	void Space::Destroy()
	{
		isDestroyed = true;
	}

	Camera& Space::GetCamera() const
	{
		return *camera;
	}

	// Game State Update
	void Space::ChangeLevel()
	{
		// Shutdown level
		if (currentLevel)
			currentLevel->Shutdown();

		// Destroy all remaining objects in space
		objectManager.Shutdown();

		// If next level is not the same as current
		if (currentLevel != nextLevel)
		{
			// Unload level
			if (currentLevel)
				currentLevel->Unload();

			// Delete current level
			delete currentLevel;
			// Set current level to next
			currentLevel = nextLevel;

			// Load level
			currentLevel->Load();
		}

		// Initialize level
		currentLevel->Initialize();

		// Reset next level pointer to null
		nextLevel = nullptr;
	}
}
