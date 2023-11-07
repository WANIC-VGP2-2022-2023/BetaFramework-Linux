//------------------------------------------------------------------------------
//
// File Name:	Level2.h
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

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

namespace Beta
{
	class Mesh;
	class Font;
	class GameObject;

	//------------------------------------------------------------------------------
	// Public Structures:
	//------------------------------------------------------------------------------

	class Level2 : public Level
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Creates an instance of Level 2.
		Level2();

		// Load the resources associated with Level2.
		void Load();

		// Initialize the memory associated with Level2.
		void Initialize();

		// Update Level2.
		// Params:
		//	 dt = Change in time (in seconds) since the last game loop.
		void Update(float dt);

		// Draw objects in Level 2.
		void Draw();

		// Shutdown any memory associated with Level2.
		void Shutdown();

		// Unload the resources associated with Level2.
		void Unload();

	private:
		//------------------------------------------------------------------------------
		// Private Function Declarations:
		//------------------------------------------------------------------------------

		GameObject* CreateSpaceship(void);
		void MoveSpaceship(void);
		void UpdateShip(float dt);

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		MeshPtr meshTriangle;
		GameObject* gameObjectShip;
		Font* font;

		const float spaceshipSpeed = 3.0f;

		float timeAccumulator;
	};
}

//----------------------------------------------------------------------------
