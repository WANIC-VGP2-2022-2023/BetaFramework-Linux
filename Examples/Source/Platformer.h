//------------------------------------------------------------------------------
//
// File Name:	Platformer.h
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

namespace Beta
{
	//------------------------------------------------------------------------------
	// Forward References:
	//------------------------------------------------------------------------------

	class Mesh;

	//------------------------------------------------------------------------------
	// Public Structures:
	//------------------------------------------------------------------------------

	class Platformer : public Beta::Level
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Creates an instance of the Platformer level.
		Platformer();

		// Load the resources associated with the Platformer level.
		void Load();

		// Initialize the memory associated with the Platformer level.
		void Initialize();

		// Update the Platformer level.
		// Params:
		//	 dt = Change in time (in seconds) since the last game loop.
		void Update(float dt);

	private:
		//------------------------------------------------------------------------------
		// Private Variable Declarations:
		//------------------------------------------------------------------------------

		// Spikes
		MeshPtr meshSpikes;

		// Enemy
		MeshPtr meshEnemy;
	};
}

//----------------------------------------------------------------------------
