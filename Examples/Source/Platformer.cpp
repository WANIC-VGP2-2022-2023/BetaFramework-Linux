//------------------------------------------------------------------------------
//
// File Name:	Platformer.cpp
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
#include "Platformer.h"

// Resources
#include <Tilemap.h>
#include <Texture.h>
#include <SpriteSource.h>
#include <MeshFactory.h>
#include <Vertex.h>

// Systems
#include <GameObject.h>
#include <Space.h>
#include <Input.h>
#include <EngineCore.h>
#include <GameObjectFactory.h>
#include <GraphicsEngine.h>

// Levels
#include "ChangeLevel.h"

// Objects
#include "Archetypes.h"
#include <Transform.h>
#include <Sprite.h>		// MeshManager

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Creates an instance of the Platformer level.
	Platformer::Platformer()
		: Level("Platformer")
	{
	}

	// Load the resources associated with the Platformer level.
	void Platformer::Load()
	{
		// Systems
		GameObjectManager& gom = GetSpace()->GetObjectManager();

		// SPIKES
		EngineGetModule(MeshFactory)->AddTriangle(
			Vertex(Vector2D(-0.5f, -0.5f), Colors::Red),
			Vertex(Vector2D(0.5f, 0.0f), Colors::Red),
			Vertex(Vector2D(-0.5f, 0.5f), Colors::Red));
		meshSpikes = MeshPtr(EngineGetModule(MeshFactory)->EndCreate());
		Sprite::GetMeshManager().Add("Spikes", meshSpikes);

		// ENEMY
		EngineGetModule(MeshFactory)->AddTriangle(
			Vertex(Vector2D(-0.5f, -0.5f), Colors::Violet),
			Vertex(Vector2D(0.5f, 0.0f), Colors::Violet),
			Vertex(Vector2D(-0.5f, 0.5f), Colors::Violet));
		meshEnemy = MeshPtr(EngineGetModule(MeshFactory)->EndCreate());
		Sprite::GetMeshManager().Add("Enemy", meshEnemy);

		// Disable quadtree - doesn't work well with tilemap
		gom.SetQuadtreeEnabled(false);

		// Set Beta Framework’s background color to black
		GraphicsEngine & graphics = *EngineGetModule(GraphicsEngine);
		graphics.SetBackgroundColor(Colors::Black);

		// Set camera size to a little more than the default (7.0f)
		graphics.GetDefaultCamera().SetVerticalSize(7.0f);
	}

	// Initialize the memory associated with the Platformer level.
	void Platformer::Initialize()
	{
		GameObjectManager& gom = GetSpace()->GetObjectManager();

		// Create the map object
		GameObject* map = new GameObject(ResourceGetArchetype("Tilemap"));
		gom.AddObject(*map);
	}

	// Update the Platformer level.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Platformer::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		if (GetSpace()->GetObjectManager().GetObjectCount("Coin") == 0)
			EngineCore::GetInstance().Stop();
		else
			ChangeLevel(this);
	}
}

//----------------------------------------------------------------------------
