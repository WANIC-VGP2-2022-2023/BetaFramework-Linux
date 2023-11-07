//------------------------------------------------------------------------------
//
// File Name:	RaycastTest.h
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

	//------------------------------------------------------------------------------
	// Public Structures:
	//------------------------------------------------------------------------------

	class RaycastTest : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		RaycastTest(float castDistance = 4.0f);

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

	private:
		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------

		// Cast a ray in the object's facing direction
		void CastRayForward();

		// Cast a ray in the object's facing direction and get the closest object
		void CastRayForwardClosest();

		// Draw the ray
		void DrawRay() const;

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		float castDistance; // Distance of raycast
		bool hitObject; // Whether the raycast succeeded this update

		// Components
		Transform* transform;

		COMPONENT_SUBCLASS_DECLARATION(RaycastTest)
	};
}

//------------------------------------------------------------------------------
