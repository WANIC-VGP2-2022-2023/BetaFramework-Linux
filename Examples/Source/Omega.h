//------------------------------------------------------------------------------
//
// File Name:	Omega.h
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

#include "Level.h"

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Forward References:
	//------------------------------------------------------------------------------

	class GameObject;

	//------------------------------------------------------------------------------
	// Public Structures:
	//------------------------------------------------------------------------------

	class Omega : public Level
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Create an instance of the Omega game state.
		Omega();

		// Load archetypes, meshes
		void Load() override;

		// Initialize the memory associated with the Omega game state.
		void Initialize() override;

		// Update the Omega game state.
		// Params:
		//	 dt = Change in time (in seconds) since the last game loop.
		void Update(float dt) override;

		void Unload() override;

		// Increase the asteroids score by score value.
		// Params:
		//	 scoreValue = The amount to be added to the game score.
		static void IncreaseScore(unsigned int scoreValue);

	private:
		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		const int cAsteroidSpawnInitial = 8;
		const int cAsteroidSpawnMaximum = 20;
		static const int scoreStringLength = 128;

		static int asteroidScore;
		static int asteroidHighScore;
		int asteroidSpawnCount;
		static int asteroidWaveCount;

		static char score[scoreStringLength];

		Archetype asteroidArchetype;

		//------------------------------------------------------------------------------
		// Private Function Declarations:
		//------------------------------------------------------------------------------

		void SpawnAsteroidWave(void);
		void SpawnAsteroid(void);
		static void UpdateScore(void);
	};
}

//----------------------------------------------------------------------------
