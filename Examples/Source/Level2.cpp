//------------------------------------------------------------------------------
//
// File Name:	Level2.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Level2.h"

// Systems
#include <MeshFactory.h>	// AddTriangle
#include <GraphicsEngine.h> // SetBackgroundColor
#include <Input.h>			// GetCursorPosition
#include <EngineCore.h>		// GetModule
#include <Space.h>			// GetCamera

// Resources
#include <Mesh.h>		// MeshCreateQuad
#include <Vertex.h>
#include <Font.h>		// CreateFontFromFile

// Components
#include <GameObject.h> // GameObjectCreate
#include "Transform.h"	// Transform
#include "Sprite.h"		// Sprite
#include "RigidBody.h"	// RigidBody

// Other levels
#include "ChangeLevel.h"

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Creates an instance of Level 2.
	Level2::Level2()
		: Level("Level 2"), meshTriangle(nullptr), gameObjectShip(nullptr),
		timeAccumulator(0.0f)
	{
	}

	// Load the resources associated with the Level2 game state.
	void Level2::Load()
	{
		// Create a "unit"-sized triangular mesh.
		EngineGetModule(MeshFactory)->AddTriangle(
			Vertex(Vector2D(-0.5f, -0.5f), Colors::Red),
			Vertex(Vector2D(0.5f, 0.0f), Colors::Yellow),
			Vertex(Vector2D(-0.5f, 0.5f), Colors::Red));
		meshTriangle = MeshPtr(EngineGetModule(MeshFactory)->EndCreate());

		font = Font::CreateFontFromFile("Arial.ttf", 40);

		// Set Beta Framework’s background color to white(1, 1, 1).
		GraphicsEngine & graphics = *EngineGetModule(GraphicsEngine);
		graphics.SetBackgroundColor(Colors::Black);

		// Set camera size to twice default (5 * 2)
		graphics.GetDefaultCamera().SetVerticalSize(10.0f);
	}

	// Initialize the memory associated with the Level2 game state.
	void Level2::Initialize()
	{
		gameObjectShip = CreateSpaceship();
	}

	// Update the Level2 game state.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Level2::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		// Call GameStateLevel2MoveSpaceship().
		MoveSpaceship();

		// Update the ship object
		UpdateShip(dt);

		ChangeLevel(this);
	}

	// Draw objects in Level 2.
	void Level2::Draw()
	{
		// Call GameObjectDraw() with the ship game object.
		gameObjectShip->Draw();

		// Draw some text
		GraphicsEngine& graphics = *EngineGetModule(GraphicsEngine);
		graphics.SetTransform(Vector2D(-4, 4), Vector2D(0.5f, 0.5f));
		graphics.SetSpriteBlendColor(Colors::LightBlue);
		font->DrawText("Use your mouse to direct the ship.");
	}

	// Shutdown any memory associated with the Level2 game state.
	void Level2::Shutdown()
	{
		// Free the ship game object.
		delete gameObjectShip;
	}

	// Unload the resources associated with the Level2 game state.
	void Level2::Unload()
	{
		delete font;
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Create a spaceship object.
	//	Returns:
	//		Pointer to the newly created spaceship.
	GameObject* Level2::CreateSpaceship(void)
	{
		//Create a game object with the following parameter :
		//"Spaceship"
		GameObject* ship = new GameObject("Spaceship");
		//Create a transform object with the following parameters :
		//Translation : 0, 0
		Transform* transform = new Transform(0.0f, 0.0f);
		//Rotation : 0
		transform->SetRotation(0.0f);
		//Scale : 100, 100
		transform->SetScale(Vector2D(1.0f, 1.0f));
		//Create a sprite object with the following parameter :
		//"Spaceship Sprite"
		Sprite* sprite = new Sprite();
		//Set sprite’s mesh.
		sprite->SetMesh(meshTriangle);
		//Create a physics object without any additional changes.
		RigidBody* physics = new RigidBody();

		//Set the game object’s transform, sprite, and physics components using the created objects.
		ship->AddComponent(transform);
		ship->AddComponent(sprite);
		ship->AddComponent(physics);

		// Initialize spaceship object
		ship->SetOwner(GetSpace());
		ship->Initialize();

		return ship;
	}

	// Handle logic for spaceship movement
	void Level2::MoveSpaceship(void)
	{
		//Get the Spaceship’s physics and transform components from the game object.
		Transform* transform = dynamic_cast<Transform*>(gameObjectShip->GetComponent<Transform>());
		RigidBody* physics = dynamic_cast<RigidBody*>(gameObjectShip->GetComponent<RigidBody>());

		//Verify that the pointers are valid.
		if (transform == nullptr || physics == nullptr) return;

		//Get the mouse cursor position :
		Vector2D mouseScreenPosition = EngineGetModule(Input)->GetCursorPosition();

		//Convert the screen coordinates to world coordinates :
		Vector2D cursorWorld = GetSpace()->GetCamera().ScreenToWorldPosition(mouseScreenPosition);

		//Get the spaceship’s current translation.
		const Vector2D & shipPosition = transform->GetTranslation();
		//Calculate a direction vector from the spaceship to the mouse position
		Vector2D direction = cursorWorld - shipPosition;

		//Normalize the direction vector
		direction = direction.Normalized();

		//Set the transform’s rotation, as follows :
		//Angle(radians) = atan2f(direction Y, direction X)
		transform->SetRotation(atan2f(direction.y, direction.x));

		//Set the physics’s objects velocity = direction vector * spaceshipSpeed
		Vector2D shipVelocity = direction * spaceshipSpeed;
		physics->SetVelocity(shipVelocity);
	}

	void Level2::UpdateShip(float dt)
	{
		// Call GameObjectUpdate() with the ship game object.
		gameObjectShip->Update(dt);

		// Fixed update - physics, animation
		float fixedUpdateDt = 1.0f / 120.0f;
		timeAccumulator += dt;

		while (timeAccumulator >= fixedUpdateDt)
		{
			// Decrease accumulator
			timeAccumulator -= fixedUpdateDt;

			// Fixed update for physics, animation
			gameObjectShip->FixedUpdate(fixedUpdateDt);
		}

		//std::cout << dt << std::endl;
	}
}
