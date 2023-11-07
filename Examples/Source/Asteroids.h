//------------------------------------------------------------------------------
//
// File Name:	Asteroids.h
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

#include "Vector2D.h"
#include "Level.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

namespace Beta
{
	class PlayerShip;
	class GameObject;

	//------------------------------------------------------------------------------
	// Public Structures:
	//------------------------------------------------------------------------------

		class Asteroids : public Level
		{
		public:
			//------------------------------------------------------------------------------
			// Public Functions:
			//------------------------------------------------------------------------------

			// Creates an instance of the Asteroids level.
			Asteroids();

			// Load the resources associated with the Asteroids level.
			void Load() override;

			// Initialize the memory associated with the Asteroids level.
			void Initialize() override;

			// Update the Asteroids level.
			// Params:
			//	 dt = Change in time (in seconds) since the last game loop.
			void Update(float dt) override;

			// Shut down the Asteroids level.
			void Shutdown() override;

		private:

			//------------------------------------------------------------------------------
			// Private Functions:
			//------------------------------------------------------------------------------

			// Create a single asteroid object from the archetype
			void SpawnAsteroid(void);

			// Create a group of asteroids
			void SpawnAsteroidWave(void);

			// Update the score string
			void UpdateScore();

			//------------------------------------------------------------------------------
			// Private Variables:
			//------------------------------------------------------------------------------

			// Spawning
			const unsigned asteroidSpawnInitial;
			const unsigned asteroidSpawnMaximum;
			unsigned asteroidSpawnCount;
			unsigned asteroidWaveCount;

			// Player
			PlayerShip* playerShip;
			unsigned asteroidHighScore;

			// Archetypes
			Archetype asteroidArchetype;
		};
}

//----------------------------------------------------------------------------
