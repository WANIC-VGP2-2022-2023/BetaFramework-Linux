//------------------------------------------------------------------------------
//
// File Name:	PlayerProjectile.cpp
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
#include "PlayerProjectile.h"

// Systems
#include <GameObject.h> // GetComponent
#include <EngineCore.h>		// GetModule

// Components
#include <Collider.h>	// CollisionEvent
#include "PlayerShip.h" // IncreaseScore
#include "Asteroid.h"	// GetPointValue

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Default constructor
	PlayerProjectile::PlayerProjectile()
		: Component("PlayerProjectile"), player()
	{
	}

	// Initialize this component (happens at object creation).
	void PlayerProjectile::Initialize()
	{
		// Set collision handler
		RegisterEventHandler(
			GetOwner(), "CollisionStarted", &PlayerProjectile::OnCollisionStarted);
	}

	// Set the object that spawned this object
	void PlayerProjectile::SetSpawner(PlayerShip* ship)
	{
		player = ship->GetID();
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// The collision handling function for Asteroids.
	// Params:
	//	 event = The collision event object.
	void PlayerProjectile::OnCollisionStarted(const Event& event)
	{
		const CollisionEvent& collisionEvent = static_cast<const CollisionEvent&>(event);

		if (collisionEvent.otherObject.GetName() == "Asteroid")
		{
			// Get asteroid points value
			Asteroid* asteroid = collisionEvent.otherObject.GetComponent<Asteroid>();
			unsigned points = asteroid->GetPointValue();

			// Increase player score
			PlayerShip* ship = static_cast<PlayerShip*>(BetaObject::GetObjectByID(player));
			if (ship != nullptr)
				ship->IncreaseScore(points);

			// Destroy projectile
			GetOwner()->Destroy();
		}
	}

	COMPONENT_SUBCLASS_DEFINITION(PlayerProjectile)
}

//------------------------------------------------------------------------------
