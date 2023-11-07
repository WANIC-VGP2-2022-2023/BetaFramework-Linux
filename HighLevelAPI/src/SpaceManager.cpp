//------------------------------------------------------------------------------
//
// File Name:	SpaceManager.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "SpaceManager.h"

//------------------------------------------------------------------------------

namespace Beta
{
	const std::string BE_HL_VERSION = "0.7.2";

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor - creates the default game space
	SpaceManager::SpaceManager()
		: BetaObject("Module:SpaceManager"), gameSpace(AddSpace("GameSpace", false))
	{
	}

	// Destructor - deletes all spaces.
	SpaceManager::~SpaceManager()
	{
		for (auto it = spaces.End() - 1; it != spaces.Begin() - 1; --it)
		{
			delete* it;
		}
		spaces.Clear();
	}

	// Initialize the space manager.
	void SpaceManager::Initialize()
	{
		std::cout << "Beta Framework High-Level API version: " << BE_HL_VERSION << std::endl << std::endl;
	}

	// Updates all spaces and destroys those that are ready for destruction.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void SpaceManager::Update(float dt)
	{
		// Update each space that is not destroyed
		size_t numSpaces = spaces.Size();
		for (size_t i = 0; i < numSpaces; ++i)
		{
			if (!spaces[i]->IsDestroyed())
				spaces[i]->Update(dt);
		}

		// Destroy spaces
		for (auto it = spaces.Begin(); it != spaces.End(); )
		{
			if ((*it)->IsDestroyed())
			{
				delete (*it);
				(*it) = nullptr;
				it = spaces.Erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	// Draw objects in all spaces
	void SpaceManager::Draw()
	{
		// Draw objects in each space
		size_t numSpaces = spaces.Size();
		for (size_t i = 0; i < numSpaces; ++i)
		{
			if (!spaces[i]->IsDestroyed())
				spaces[i]->Draw();
		}
	}

	// Shutdown the game engine.
	void SpaceManager::Shutdown()
	{
		for (auto it = spaces.End() - 1; it != spaces.Begin() - 1; --it)
		{
			(*it)->Shutdown();
		}
	}

	// Creates a new space with the given name
	Space* SpaceManager::AddSpace(const std::string& spaceName, bool useCustomCamera)
	{
		Space* newSpace = new Space(spaceName, useCustomCamera);
		newSpace->SetOwner(this);
		spaces.PushBack(newSpace);
		return newSpace;
	}

	// Gets a pointer to the space
	Space* SpaceManager::GetSpaceByName(const std::string& spaceName) const
	{
		size_t numSpaces = spaces.Size();
		for (size_t i = 0; i < numSpaces; ++i)
		{
			if (spaces[i]->GetName() == spaceName)
				return spaces[i];
		}

		return nullptr;
	}

	// Returns the default space for game objects
	Space& SpaceManager::GetDefaultSpace()
	{
		return *gameSpace;
	}
}
