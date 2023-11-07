//------------------------------------------------------------------------------
//
// File Name:	PlayerProjectile.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright � 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Component.h" // base class

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

namespace Beta
{
	class PlayerShip;
	struct Event;

	//------------------------------------------------------------------------------
	// Public Structures:
	//------------------------------------------------------------------------------


	class PlayerProjectile : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Default constructor
		PlayerProjectile();

		// Initialize this component (happens at object creation).
		void Initialize() override;

		// Set the object that spawned this object
		void SetSpawner(PlayerShip* player);

	private:
		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------

		// The collision handling function for projectiles.
		// Params:
		//	 event = The collision event object.
		void OnCollisionStarted(const Event& event);

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Who spawned this object?
		IDType player;

		COMPONENT_SUBCLASS_DECLARATION(PlayerProjectile)
	};
}

//------------------------------------------------------------------------------
