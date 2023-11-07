//------------------------------------------------------------------------------
//
// File Name:	MonkeyMovement.h
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

#include "Vector2D.h" // Vector2D

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

namespace Beta
{
	class Transform;
	class RigidBody;
	struct Event;

	//------------------------------------------------------------------------------
	// Public Structures:
	//------------------------------------------------------------------------------

	class MonkeyMovement : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		MonkeyMovement();

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

		// Determine whether the monkey is on the ground
		bool IsOnGround() const;

	private:
		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------

		// Moves horizontally based on input
		void MoveHorizontal() const;

		// Moves vertically based on input
		void MoveVertical();

		// Collision start event handler.
		// Params:
		//	 event = The collision event object.
		void OnCollisionStarted(const Event& event);

		// Map collision event handler.
		// Params:
		//	 event = The collision event object.
		void OnMapCollision(const Event& event);

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Movement properties
		float monkeyWalkSpeed;
		float monkeyJumpSpeed;
		float monkeyFallSpeedMax;
		Vector2D gravity;

		// Components
		Transform* transform;
		RigidBody* physics;

		// Misc variables
		bool onGround;
		bool inLevel1;

		COMPONENT_SUBCLASS_DECLARATION(MonkeyMovement)
	};
}

//------------------------------------------------------------------------------
