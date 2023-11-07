//------------------------------------------------------------------------------
//
// File Name:	Asteroid.cpp
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
#include "Asteroid.h"

// Systems
#include <GameObject.h>		// Destroy
#include <FileStream.h>			// WriteVariable, ReadVariable
#include <Random.h>			// Random::Range
#include <Space.h>			// GetObjectManager
#include <GraphicsEngine.h>	// GetScreenWorldDimensions

// Components
#include <Transform.h>		// GetTranslation, SetTranslation
#include <RigidBody.h>		// SetVelocity
#include <ColliderCircle.h>

//---------------------------------------------------------------------------------------------------------------------------------------------

namespace Beta
{
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
	Asteroid::Asteroid(float speedMin, float speedMax, unsigned basePointsValue,
		float sizePointsModifier, float spawnScaleModifier)
		: Component("Asteroid"), speedMin(speedMin), speedMax(speedMax), basePointsValue(basePointsValue),
		sizePointsModifier(sizePointsModifier), spawnScaleModifier(spawnScaleModifier),
		size(SizeLarge), location(LocationTopLeft)
	{
	}

	// Initialize this component (happens at object creation).
	void Asteroid::Initialize()
	{
		// Get components
		transform = GetOwner()->GetComponent<Transform>();
		physics = GetOwner()->GetComponent<RigidBody>();

		// Set collision handler
		RegisterEventHandler(
			GetOwner(), "CollisionStarted", &Asteroid::CollisionHandler);

		// Call the SetPosition function
		SetPosition();
		// Call the SetVelocity function
		SetVelocity();
	}

	// Write object data to file
	// Params:
	//   stream = The stream that is writing this object to a file.
	void Asteroid::Serialize(FileStream& stream) const
	{
		stream.WriteVariable("speedMin", speedMin);
		stream.WriteVariable("speedMax", speedMax);
		stream.WriteVariable("basePointsValue", basePointsValue);
		stream.WriteVariable("sizePointsModifier", sizePointsModifier);
		stream.WriteVariable("spawnScaleModifier", spawnScaleModifier);
	}

	// Read object data from a file
	// Params:
	//   stream = The stream that is reading this object's data from a file.
	void Asteroid::Deserialize(FileStream& stream)
	{
		stream.ReadVariable("speedMin", speedMin);
		stream.ReadVariable("speedMax", speedMax);
		stream.ReadVariable("basePointsValue", basePointsValue);
		stream.ReadVariable("sizePointsModifier", sizePointsModifier);
		stream.ReadVariable("spawnScaleModifier", spawnScaleModifier);
	}

	// Get the value of this object for increasing the player's score.
	unsigned Asteroid::GetPointValue() const
	{
		float scoreValue = static_cast<float>(basePointsValue);
		for (int sizeCategory = size; sizeCategory > 0; --sizeCategory)
			scoreValue *= sizePointsModifier;

		return static_cast<unsigned>(scoreValue);
	}

	// This is a new, private function for resolving a collision between two objects.
	// Params:
	//	 event = The collision event object.
	void Asteroid::CollisionHandler(const Event & event)
	{
		const CollisionEvent& collisionEvent = (CollisionEvent&)event;

		//If gameObject2’s name is either “Bullet” or “Spaceship”,
		if (collisionEvent.otherObject.GetName() == "Bullet" ||
			collisionEvent.otherObject.GetName() == "Spaceship")
		{
			// If asteroid is large, create smaller asteroids
			SpawnNewAsteroids();

			// Destroy the asteroid
			GetOwner()->Destroy();
		}
	}

	void Asteroid::SetPosition()
	{
		// If the asteroid is not large, return
		if (size != SizeLarge) return;

		// Set the asteroid behavior’s “location” variable to a random number between 0 and 3.
		location = static_cast<Location>(Random::Range(LocationTopLeft, LocationBottomRight));

		Vector2D position;

		// Get the screen's world dimensions
		const BoundingRectangle & screenRect = GetOwner()->GetSpace()->GetCamera().GetScreenWorldDimensions();

		// Set the asteroid’s position to one of the four corners of the screen
		// depending upon its “location” variable.
		switch (location)
		{
			// Bottom left
		case LocationBottomLeft:
			position.x = screenRect.left;
			position.y = screenRect.bottom;
			break;
			// Bottom right
		case LocationBottomRight:
			position.x = screenRect.right;
			position.y = screenRect.bottom;
			break;
			// Top left
		case LocationTopLeft:
			position.x = screenRect.left;
			position.y = screenRect.top;
			break;
			// Top right
		case LocationTopRight:
			position.x = screenRect.right;
			position.y = screenRect.top;
			break;
		default:
			break;
		}

		// Move asteroids inward in Omega
		if (GetOwner()->GetSpace()->GetLevelName() == "Level:Omega")
		{
			position *= 0.8f;
		}

		physics->MovePosition(position);
	}

	void Asteroid::SetVelocity()
	{
		float angle = 0.0f;

		// If the asteroid’s “size” is cAsteroidLarge,
		if (size == SizeLarge)
		{
			switch (location)
			{
				//If “origin” is bottom - left corner,
				//Generate a random angle between 10 and 80 degrees.
			case LocationBottomLeft:
				angle = Random::Range(10.0f, 80.0f);
				break;
				//If “origin” is bottom - right corner,
				//Generate a random angle between 100 and 170 degrees.
			case LocationBottomRight:
				angle = Random::Range(100.0f, 170.0f);
				break;
				//If “origin” is top - left corner,
				//Generate a random angle between - 10 and -80 degrees.
			case LocationTopLeft:
				angle = Random::Range(-80.0f, -10.0f);
				break;
				//If “origin” is top - right corner,
				//Generate a random angle between - 100 and -170 degrees.
			case LocationTopRight:
				angle = Random::Range(-170.0f, -100.0f);
				break;
			default:
				break;
			}
		}
		// If the asteroid’s “size” is cAsteroidMedium or cAsteroidSmall,
		else
		{
			//Generate a random angle between 0 and 359 degrees.
			angle = Random::Range(0.0f, 359.0f);
		}

		// Set the asteroid’s velocity in the direction of the random angle, 
		// with a random speed between asteroidSpeedMin and asteroidSpeedMax.
		Vector2D velocity = Vector2D::FromAngleDegrees(angle);
		float speed = Random::Range(speedMin, speedMax);
		physics->SetVelocity(velocity * speed);

		// Set the asteroid's rotational velocity
		// Rotational velocity = PI / 4.0f
		physics->SetAngularVelocity(Random::Range(-(float)M_PI / 2.0f, (float)M_PI / 2.0f));
	}

	void Asteroid::SpawnNewAsteroids()
	{
		int numAsteroids = 0;

		switch (size)
		{
		case SizeLarge:
			numAsteroids = Random::Range(2, 3);
			//numAsteroids = 3;
			break;
		case SizeMedium:
			numAsteroids = Random::Range(1, 2);
			//numAsteroids = 2;
			break;
		default:
			// Do nothing
			break;
		}

		for (int i = 0; i < numAsteroids; ++i)
		{
			// Clone asteroid
			GameObject* clone = new GameObject(*GetOwner());

			// Decrease size category by one
			Asteroid* behavior = clone->GetComponent<Asteroid>();
			behavior->size = static_cast<Size>(behavior->size - 1);

			// Scale asteroid down
			Transform * spawnTransform = clone->GetComponent<Transform>();
			spawnTransform->SetScale(spawnTransform->GetScale() * spawnScaleModifier);

			ColliderCircle * collider = clone->GetComponent<ColliderCircle>();
			collider->SetRadius(spawnTransform->GetScale().x / 2.0f);

			// Add to object manager
			GetOwner()->GetSpace()->GetObjectManager().AddObject(*clone);
		}
	}

	COMPONENT_SUBCLASS_DEFINITION(Asteroid)
}

//------------------------------------------------------------------------------
