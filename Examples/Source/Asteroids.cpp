//------------------------------------------------------------------------------
//
// File Name:	Asteroids.cpp
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
#include "Asteroids.h"

#include <sstream>		// Print window title

// Systems
#include "Archetypes.h"	// CreateShip, CreateBulletArchetype, CreateAsteroidArchetype

// Components
#include "Asteroid.h"
#include "PlayerShip.h"

// Levels
#include "ChangeLevel.h"

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Creates an instance of the Asteroids level.
	Asteroids::Asteroids()
		: Level("Asteroids"), asteroidSpawnInitial(8), asteroidSpawnMaximum(20),
		asteroidSpawnCount(asteroidSpawnInitial), asteroidWaveCount(0), asteroidHighScore(0), playerShip(nullptr)
	{
	}

	// Load the resources associated with the Asteroids levle.
	void Asteroids::Load()
	{
		std::cout << "Asteroids::Load" << std::endl;

		// Create bullet archetype and add to object manager
		//GameObject* bulletArchetype = EngineGetModule(GameObjectFactory)->CreateObject("Bullet", GetSpace());
		//GetSpace()->GetObjectManager().AddArchetype(*bulletArchetype);

		// Create asteroid archetype and add to object manager
		asteroidArchetype = ResourceGetArchetype("Asteroid");
		//GetSpace()->GetObjectManager().AddArchetype(*asteroidArchetype);

		// Set Beta Framework’s background color to black
		GraphicsEngine& graphics = *EngineGetModule(GraphicsEngine);
		graphics.SetBackgroundColor(Colors::Black);

		// Set camera size to a little more than twice the default (~12)
		graphics.GetDefaultCamera().SetVerticalSize(12.0f);
	}

	// Initialize the memory associated with the Asteroids level.
	void Asteroids::Initialize()
	{
		std::cout << "Asteroids::Initialize" << std::endl;
		// Enable quadtree
		GetSpace()->GetObjectManager().SetQuadtreeEnabled(false);

		// Create spaceship and add to object manager
		GameObject* ship = new GameObject(ResourceGetArchetype("PlayerShip"));
		GetSpace()->GetObjectManager().AddObject(*ship);

		// Store player ship component of ship object in pointer
		playerShip = ship->GetComponent<PlayerShip>();

		// Reset the wave count to 0.
		asteroidWaveCount = 0;
		// Reset the spawn count to asteroidSpawnInitial.
		asteroidSpawnCount = asteroidSpawnInitial;
	}

	// Update the Asteroids level.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Asteroids::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		// If there are no asteroids in existence
		if (GetSpace()->GetObjectManager().GetObjectByName("Asteroid") == nullptr)
			// Spawn a wave of asteroids
			SpawnAsteroidWave();

		// Update the score string
		UpdateScore();

		// Switch levels if keys are pressed
		ChangeLevel(this);
	}

	void Asteroids::Shutdown()
	{
		// Update the high score if the new score is greater.
		asteroidHighScore = std::max(asteroidHighScore, playerShip->GetScore());

		// Disable quadtree
		GetSpace()->GetObjectManager().SetQuadtreeEnabled(false);
	}

	void Asteroids::SpawnAsteroid(void)
	{
		//Clone the asteroid archetype.
		GameObject* asteroid = new GameObject(asteroidArchetype);
		//Add the cloned asteroid to the active game object list.
		GetSpace()->GetObjectManager().AddObject(*asteroid);
	}

	void Asteroids::SpawnAsteroidWave(void)
	{
		//Increment asteroidWaveCount by 1.
		++asteroidWaveCount;

		for (unsigned i = 0; i < asteroidSpawnCount; ++i)
			//Call GameStateAsteroidsSpawnAsteroid() a number of times equal to asteroidSpawnCount.
			SpawnAsteroid();

		//Increment asteroidSpawnCount by 1, to a maximum of cAsteroidSpawnMaximum
		asteroidSpawnCount = std::min(asteroidSpawnMaximum, asteroidSpawnCount + 1);
	}

	// Update the score string
	void Asteroids::UpdateScore()
	{
		// Get frame time for FPS
		float dt = EngineGetModule(FrameRateController)->GetFrameTime();

		// Get score from player
		unsigned asteroidScore = playerShip->GetScore();

		std::stringstream windowTitle;
		windowTitle << "Asteroids: Wave = " << asteroidWaveCount << ", Score = " 
			<< asteroidScore << ", High Score = " << asteroidHighScore << ", FPS: " 
			<< static_cast<int>(1.0f / dt) << std::endl;

		// Show number of bullets in existence in window title
		EngineGetModule(WindowSystem)->SetWindowTitle(windowTitle.str());
	}
}
