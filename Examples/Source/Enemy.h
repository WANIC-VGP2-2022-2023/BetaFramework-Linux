//------------------------------------------------------------------------------
//
// File Name:	Enemy.h
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
#include <Color.h>	 // Color

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Forward Declarations:
	//------------------------------------------------------------------------------

	class Transform;
	class RigidBody;
	struct Event;

	//------------------------------------------------------------------------------
	// Public Structures:
	//------------------------------------------------------------------------------

	enum InnerStates
	{
		InnerStateEnter,
		InnerStateUpdate,
		InnerStateExit
	};

	class Enemy : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		Enemy();

		// Initialize data for this object.
		void Initialize() override;

		// Update function for this component.
		// Params:
		//   dt = The (fixed) change in time since the last step.
		void Update(float dt) override;

		// Map collision handler for Monkey objects.
		// Params:
		//   event = The map collision event.
		void OnMapCollision(const Event& collision);

		// Sets the next state of the enemy.
		// Params:
		//   nextState = The next state the enemy should be in.
		void SetState(unsigned nextState);

		// Write object data to file
		// Params:
		//   stream = The stream that is writing this object to a file.
		void Serialize(FileStream& stream) const override;

		// Read object data from a file
		// Params:
		//   stream = The stream that is reading this object's data from a file.
		void Deserialize(FileStream& stream) override;

	private:
		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		void Idle(float dt);
		void Wander(float dt);
		void Chase(float dt);

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// States
		unsigned currentState;  // The current state of the enemy. Ex: idle, wander, chase
		unsigned nextState;		// The next state of the enemy.

		unsigned innerState; // The inner state of the current state. Ex: enter, update, exit

		// Timers
		float timer; // Used for delaying state changes, etc.
		float stateChangeTime; // Amount of time to wait before changing states.

		// Components - Get from owner in Initialize
		Transform* transform;
		RigidBody* physics;
		Transform* playerTransform;

		// Properties
		float baseSpeed;
		float chaseMultiplier;
		float sightRange;

		COMPONENT_SUBCLASS_DECLARATION(Enemy)
	};
}
