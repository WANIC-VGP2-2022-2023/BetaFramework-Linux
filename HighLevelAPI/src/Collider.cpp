//------------------------------------------------------------------------------
//
// File Name:	Collider.cpp
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
#include "Collider.h"

// Math
#include "Vector2D.h" // Vector2DSquareDistance
#include "Intersection2D.h" // CircleCircleIntersection

// Systems
#include "EngineCore.h" // GetModule

// Components
#include "GameObject.h" // GetComponent
#include "Transform.h"	// GetTranslation, GetRotation
#include "RigidBody.h"	// GetComponent
#include "Sprite.h"		// GetComponent

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor for CollisionEvent
	CollisionEvent::CollisionEvent(GameObject& otherObject, const std::string& type)
		: Event(type), otherObject(otherObject)
	{
	}

	// Allocate a new collider component.
	// Params:
	//   type = The type of collider (circle, line, etc.).
	Collider::Collider(ColliderType cType)
		: Component("Collider"), transform(nullptr), physics(nullptr), sprite(nullptr), cType(cType),
		processed(false)
	{
	}

	// Set component pointers.
	void Collider::Initialize()
	{
		transform = GetOwner()->GetComponent<Transform>();
		physics = GetOwner()->GetComponent<RigidBody>();
		sprite = GetOwner()->GetComponent<Sprite>();
		eventManager = EngineGetModule(EventManager);
	}

	// Logic update for this component.
	void Collider::FixedUpdate(float dt)
	{
		UNREFERENCED_PARAMETER(dt);
		processed = false;

		// For all previous colliders
		for (auto it = collidersPrevious.begin(); it != collidersPrevious.end(); ++it)
		{
			// If it is also in current, it has not stopped colliding
			if (collidersCurrent.find(*it) != collidersCurrent.end())
				continue;

			Collider * other = static_cast<Collider*>(BetaObject::GetObjectByID(*it));

			// Other collider still exists
			if (other != nullptr)
			{
				eventManager->SendEvent(
					new CollisionEvent(*other->GetOwner(), "CollisionEnded"), GetOwner());
				eventManager->SendEvent(
					new CollisionEvent(*GetOwner(), "CollisionEnded"), other->GetOwner());
			}
		}

		// Clear previous colliders
		collidersPrevious.clear();

		// Store current colliders in previous
		std::swap(collidersCurrent, collidersPrevious);
	}

	// Check if two objects are colliding and send collision events.
	// Params:
	//	 other = Pointer to the second collider component.
	void Collider::CheckCollision(const Collider& other)
	{
		bool colliding = false;

		// Perform the actual collision math for collisions between objects
		colliding = IsCollidingWith(other);

		// If they collide, call respective handlers
		if (colliding)
		{
			// Look for collider in previous colliders
			auto locPrev = collidersPrevious.find(other.GetID());
			auto locCurrent = collidersCurrent.find(other.GetID());

			// Add to current
			collidersCurrent.insert(other.GetID());

			// Collided during last frame
			if (locPrev != collidersPrevious.end())
			{
				eventManager->SendEvent(
					new CollisionEvent(*other.GetOwner(), "CollisionPersisted"), GetOwner());
				eventManager->SendEvent(
					new CollisionEvent(*GetOwner(), "CollisionPersisted"), other.GetOwner());
			}
			// Collision started
			else if (locCurrent == collidersCurrent.end())
			{
				eventManager->SendEvent(
					new CollisionEvent(*other.GetOwner(), "CollisionStarted"), GetOwner());
				eventManager->SendEvent(
					new CollisionEvent(*GetOwner(), "CollisionStarted"), other.GetOwner());
			}
		}
	}

	// Get the type of this component.
	ColliderType Collider::GetColliderType() const
	{
		return cType;
	}

	// Has this component been checked for collisons?
	bool Collider::WasProcesed() const
	{
		return processed;
	}

	// Set this collider's processed bool.
	void Collider::SetProcessed(bool value)
	{
		processed = value;
	}

	// RTTI
	COMPONENT_ABSTRACT_DEFINITION(Collider)
}

//------------------------------------------------------------------------------
