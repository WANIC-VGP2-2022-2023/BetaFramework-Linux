//------------------------------------------------------------------------------
//
// File Name:	Asteroid.h
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

	class Asteroid : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Asteroid constructor
		// Params:
		//   speedMin = Minimum of range for asteroid movement speed.
		//   speedMax = Maximum of range for asteroid movement speed.
		//   basePointsValue = Point value for largest asteroid.
		//   sizePointsModifier = Multiplicative scale applied to points for smaller asteroids.
		//   spawnScaleModifier = Percentage of original asteroid's scale to use for new asteroids.
		Asteroid(float speedMin = 1.0f, float speedMax = 2.0f, unsigned basePointsValue = 20,
			float sizePointsModifier = 0.5f, float spawnScaleModifier = 0.8f);

		// Initialize this component (happens at object creation).
		void Initialize() override;

		// Write object data to file
		// Params:
		//   stream = The stream that is writing this object to a file.
		void Serialize(FileStream& stream) const override;

		// Read object data from a file
		// Params:
		//   stream = The stream that is reading this object's data from a file.
		void Deserialize(FileStream& stream) override;

		// Get the value of this object for increasing the player's score.
		unsigned GetPointValue() const;

	private:
		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------

		// The collision handling function for Asteroids.
		// Params:
		//	 event = The collision event object.
		void CollisionHandler(const Event& event);

		// Set position of asteroid based on origin
		void SetPosition();

		// Set velocity based on size
		void SetVelocity();

		// Generate new asteroids based off this asteroid
		void SpawnNewAsteroids();

		//------------------------------------------------------------------------------
		// Private Structures:
		//------------------------------------------------------------------------------

		enum Size
		{
			SizeSmall,
			SizeMedium,
			SizeLarge,
		};

		enum Location
		{
			LocationTopLeft,
			LocationTopRight,
			LocationBottomLeft,
			LocationBottomRight,
		};

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Properties (save to/load from file)
		float speedMin;
		float speedMax;
		unsigned basePointsValue;
		float sizePointsModifier;
		float spawnScaleModifier;

		// Other variables
		Size size;
		Location location;

		// Components
		Transform* transform;
		RigidBody* physics;

		COMPONENT_SUBCLASS_DECLARATION(Asteroid)
	};
}

//------------------------------------------------------------------------------
