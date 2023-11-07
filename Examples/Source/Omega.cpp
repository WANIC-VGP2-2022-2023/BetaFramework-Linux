//------------------------------------------------------------------------------
//
// File Name:	Omega.h
// Author(s):	 Jeremy Kings (j.kings)
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
#include "Omega.h"

#include <EngineCore.h>			// GetModule
#include "GameObjectManager.h"	// AddObject, Shutdown
#include "Transform.h"			// SetTranslation
#include "GameObjectFactory.h"	// CreateObject
#include "ResourceManager.h"	// FreeMeshes
#include <Input.h>				// CheckTriggered
#include <GraphicsEngine.h>		// SetBackgroundColor
#include <WindowSystem.h>		// SetWindowTitle
#include "Space.h"				// GetStateManager

// Other levels
#include "ChangeLevel.h"

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	int Omega::asteroidScore = 0;
	int Omega::asteroidHighScore = 0;
	char Omega::score[scoreStringLength];
	int Omega::asteroidWaveCount = 0;

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Create an instance of the Omega game state.
	Omega::Omega()
		: Level("Omega"), asteroidSpawnCount(0)
	{
	}

	// Load archetypes, meshes
	void Omega::Load()
	{
		// Create archetypes
		//GetSpace()->GetObjectManager().AddArchetype(*EngineGetModule(GameObjectFactory)->CreateObject("Bullet", GetSpace()));
		//GetSpace()->GetObjectManager().AddArchetype(*EngineGetModule(GameObjectFactory)->CreateObject("Asteroid", GetSpace()));

		// Enable quadtree
		GetSpace()->GetObjectManager().SetQuadtreeEnabled(true);

		// Set Beta Framework’s background color to black
		GraphicsEngine& graphics = *EngineGetModule(GraphicsEngine);
		graphics.SetBackgroundColor(Colors::Black);

		// Set camera size to a little more than twice the default (~12)
		graphics.GetDefaultCamera().SetVerticalSize(12.0f);

		// Get the “Asteroid” archetype game object.
		asteroidArchetype = ResourceGetArchetype("Asteroid");
	}

	// Initialize the memory associated with the Omega game state.
	void Omega::Initialize()
	{
		// Create spaceship
		GameObject* ship = new GameObject(ResourceGetArchetype("PlayerShip"));
		ship->GetComponent<Transform>()->SetTranslation(Vector2D(-6.0f, 0.0f));
		GetSpace()->GetObjectManager().AddObject(*ship);

		// Create arena
		GetSpace()->GetObjectManager().AddObject(*new GameObject(ResourceGetArchetype("Arena")));
		GetSpace()->GetObjectManager().AddObject(*new GameObject(ResourceGetArchetype("Walls")));

		//Update the high score if the new score is greater.
		asteroidHighScore = std::max(asteroidHighScore, asteroidScore);
		//Initialize the score to 0.
		asteroidScore = 0;

		//Initialize the wave count to 0.
		asteroidWaveCount = 0;
		//Initialize the spawn count to cAsteroidSpawnInitial.
		asteroidSpawnCount = cAsteroidSpawnInitial;
		//Call GameStateAsteroidsSpawnAsteroidWave()
		SpawnAsteroidWave();
	}

	// Update the Omega game state.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Omega::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		// If there are no asteroids in the game object manager’s active list,
		if (GetSpace()->GetObjectManager().GetObjectByName("Asteroid") == nullptr)
			// Call GameStateAsteroidsSpawnAsteroidWave().
			SpawnAsteroidWave();

		ChangeLevel(this);
	}

	void Omega::Unload()
	{
		// Enable quadtree
		GetSpace()->GetObjectManager().SetQuadtreeEnabled(false);
	}

	// Increase the asteroids score by score value.
	// Params:
	//	 scoreValue = The amount to be added to the game score.
	void Omega::IncreaseScore(unsigned int scoreValue)
	{
		//Increase the current score by “scoreValue”.
		asteroidScore += scoreValue;

		//Call GameStateAsteroidsUpdateScore().
		UpdateScore();
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	void Omega::SpawnAsteroidWave(void)
	{
		//Increment asteroidWaveCount by 1.
		++asteroidWaveCount;

		//Call GameStateAsteroidsUpdateScore().
		UpdateScore();

		for (int i = 0; i < asteroidSpawnCount; ++i)
			//Call GameStateAsteroidsSpawnAsteroid() a number of times equal to asteroidSpawnCount.
			SpawnAsteroid();

		//Increment asteroidSpawnCount by 1, to a maximum of cAsteroidSpawnMaximum
		asteroidSpawnCount = std::min(cAsteroidSpawnMaximum, asteroidSpawnCount + 1);
	}

	void Omega::SpawnAsteroid(void)
	{
		// Clone the asteroid archetype.
		GameObject* asteroid = new GameObject(asteroidArchetype);

		// Add the cloned asteroid to the active game object list.
		GetSpace()->GetObjectManager().AddObject(*asteroid);

		// Set position of object closer to center
		Transform* transform = asteroid->GetComponent<Transform>();
		transform->SetTranslation(transform->GetTranslation() - transform->GetTranslation() * 0.1f);
	}

	void Omega::UpdateScore(void)
	{
		//Build a string with the following parameters :
		//"CS230 Project 8 - Omega, Wave = %d, Score = %d, High Score = %d"
		//asteroidWaveCount, asteroidScore, asteroidHighScore
		sprintf_s(score, scoreStringLength, "CS230 Project 8 - Omega, Wave = %d, Score = %d, High Score = %d",
			asteroidWaveCount, asteroidScore, asteroidHighScore);

		//Change the window tile using AESysSetWindowTitle().
		EngineGetModule(WindowSystem)->SetWindowTitle(score);
	}
}
