//------------------------------------------------------------------------------
//
// File Name:	Level1.cpp
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
#include "Level1.h"

// Systems
#include <EngineCore.h>		// GetModule
#include <GraphicsEngine.h>	// SetBackgroundColor
#include <Input.h>		// CheckTriggered
#include "Space.h"		// GetStateManager
#include "GameObjectFactory.h"	// CreateObject
#include "ResourceManager.h"	// AddMesh

// Resources
#include <MeshFactory.h>	// CreateQuad
#include "SpriteSource.h"	// SpriteSourceCreate
#include <Texture.h>		// Constructor
#include "Archetypes.h"

// Components
#include "GameObject.h"	// GameObjectCreate, GameObjectUpdate, etc.
#include <Vector2D.h>	// Vector2D
#include "Transform.h"	// Transform
#include "Sprite.h"		// Sprite
#include "Animation.h"	// Animation
#include "RigidBody.h"	// RigidBody
#include "MonkeyMovement.h"	 // MonkeyMovement
#include "MonkeyAnimation.h" // MonkeyAnimation

// Levels
#include "ChangeLevel.h"


//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Creates an instance of Level 1.
	Level1::Level1()
		: Level("Level1")
	{
	}

	// Initialize the memory associated with Level 1.
	void Level1::Initialize()
	{
		std::cout << "Level1::Initialize" << std::endl;

		GameObject* gameObjectMonkey = nullptr;

		// Use the class function to make the monkey object
		gameObjectMonkey = Archetypes::CreateMonkey();

		// Add monkey to object manager
		GetSpace()->GetObjectManager().AddObject(*gameObjectMonkey);

		// Set Beta Framework’s background color to white(1, 1, 1).
		GraphicsEngine& graphics = *EngineGetModule(GraphicsEngine);
		graphics.SetBackgroundColor(Colors::White);

		// Set camera size to default
		graphics.GetDefaultCamera().Reset();
	}

	// Update Level 1.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Level1::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		//std::cout << "Framerate: " << 1.0f / dt << std::endl;
		ChangeLevel(this);
	}
}
