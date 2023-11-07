//------------------------------------------------------------------------------
//
// File Name:	PlayerShip.h
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

#include "Component.h" // base class

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Forward Declarations:
	//------------------------------------------------------------------------------

	class Transform;
	class GameObject;
	typedef std::shared_ptr<const GameObject> Archetype;

	class RigidBody;
	struct Event;

	//------------------------------------------------------------------------------
	// Public Structures:
	//------------------------------------------------------------------------------

	class PlayerShip : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		// Params:
		//   forwardThrust	= Amount of force added to the ship when thrusters are activated.
		//   maximumSpeed	= Maximum attainable speed of the ship.
		//   rotationSpeed	= Speed at which the ship rotates.
		//   bulletSpeed	= Speed at which bullets move when fired by ship.
		//   deathDuration	= Length of death "animation" in seconds.
		//   spreadNumber	= Number of bullets to create when shooting.
		//   spreadAngle	= Angle between each bullet in the spread.
		PlayerShip(float forwardThrust = 3.0f, float rotationSpeed = (float)M_PI,
			float bulletSpeed = 4.0f, float deathDuration = 4.0f, int spreadNumber = 3,
			float spreadAngle = (float)(M_PI) / 6.0f);

		// Initialize this component (happens at object creation).
		void Initialize() override;

		// Update function for this component.
		// Params:
		//   dt = The (fixed) change in time since the last step.
		void Update(float dt) override;

		// Write object data to file
		// Params:
		//   stream = The stream that is writing this object to a file.
		void Serialize(FileStream& stream) const override;

		// Read object data from a file
		// Params:
		//   stream = The stream that is reading this object's data from a file.
		void Deserialize(FileStream& stream) override;

		// Returns player's score
		unsigned GetScore() const;

		// Increase player's score
		// Params:
		//   amount = This value is added to the player's current score.
		void IncreaseScore(unsigned amount);

	private:
		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------

		// Move forward when up arrow is pressed
		void Move() const;

		// Rotate when left or right arrow key is pressed
		void Rotate() const;

		// Shoot projectiles when space is pressed
		void Shoot();

		// Play death "animation"
		void DeathSequence(float dt);

		// Collision start event handler.
		// Params:
		//	 event = The collision event object.
		void OnCollisionStarted(const Event& event);

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Properties (save to/load from file)
		float forwardThrust;
		float rotationSpeed;
		float bulletSpeed;
		float deathDuration;
		int spreadNumber;
		float spreadAngle;

		// Bullet archetype
		Archetype bulletArchetype;

		// Components
		Transform* transform;
		RigidBody* physics;

		// Other variables
		float timer;
		bool isDying;
		unsigned score;
		bool invincible;

		COMPONENT_SUBCLASS_DECLARATION(PlayerShip)
	};
}

//------------------------------------------------------------------------------
