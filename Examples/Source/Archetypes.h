//------------------------------------------------------------------------------
//
// File Name:	Archetypes.h
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

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Forward Declarations:
	//------------------------------------------------------------------------------

	class GameObject;
	class Mesh;
	typedef std::shared_ptr<Mesh> MeshPtr;

	namespace Archetypes
	{
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Create a game object that uses the Ship texture.
		// Params:
		//   space = The space used to access the resource manager.
		// Returns:
		//	 A pointer to the newly constructed game object
		GameObject* CreateShip(void);

		// Create a game object archetype that uses the Bullet texture.
		// Params:
		//   space = The space used to access the resource manager.
		// Returns:
		//	 A pointer to the newly constructed game object
		GameObject* CreateBulletArchetype(void);

		// Create a game object archetype that uses the Asteroid texture.
		// Params:
		//   space = The space used to access the resource manager.
		// Returns:
		//	 A pointer to the newly constructed game object
		GameObject* CreateAsteroidArchetype(void);

		// Create the monkey object
		// Params:
		//   space = The space used to access the resource manager.
		// Returns:
		//   A pointer to the newly constructed game object.
		GameObject* CreateMonkey(void);

		// Create the spikes game object.
		// Params:
		//   mesh  = The mesh to use for the object's sprite.
		// Returns:
		//	 A pointer to the newly constructed game object
		GameObject* CreateSpikes(MeshPtr mesh);

		// Create the coin game object.
		// Params:
		//   space = Space used to access the resource manager.
		// Returns:
		//	 A pointer to the newly constructed game object
		GameObject* CreateCoin(void);

		// Create the enemy game object.
		// Params:
		//   mesh = Custom mesh used for enemy object.
		// Returns:
		//	 A pointer to the newly constructed game object
		GameObject* CreateEnemy(MeshPtr mesh);
	}
}

//----------------------------------------------------------------------------
