//------------------------------------------------------------------------------
//
// File Name:	ColliderCircle.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "ColliderCircle.h"

// Math
#include "Intersection2D.h" // Circle collision

// Systems
#include <EngineCore.h> // GetModule
#include "FileStream.h"
#include <DebugDraw.h>	// AddCircle
#include "Space.h"		// GetCamera

// Components
#include "GameObject.h" // GetTransform
#include "Transform.h"  // GetTranslation
#include "RigidBody.h"	// GetOldTranslation
#include "Sprite.h"		// GetZDepth

//------------------------------------------------------------------------------
namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Create a new (circle) collider component.
	// (Hint: Make sure to initialize the ColliderType correctly.)
	// Params:
	//   owner = Reference to the object that owns this component.
	ColliderCircle::ColliderCircle(float radius)
		: Collider(ColliderTypeCircle), radius(radius)
	{
		SetName("ColliderCircle");
	}

	// Debug drawing for colliders.
	void ColliderCircle::Draw()
	{
		DebugDraw& debugDraw = *EngineGetModule(DebugDraw);

		float zDepth = 0.01f;
		if (sprite)
			zDepth += sprite->GetZDepth();

		debugDraw.SetCamera(GetOwner()->GetSpace()->GetCamera());
		debugDraw.AddCircle(transform->GetTranslation(), radius, Colors::Green, zDepth);
	}

	// Get the circle collider's radius.
	// Returns:
	//	 The circle collider's radius.
	float ColliderCircle::GetRadius() const
	{
		return radius;
	}

	// Set the circle collider's radius.
	// Params:
	//   radius = The circle collider's new radius.
	void ColliderCircle::SetRadius(float radius_)
	{
		radius = radius_;
	}

	// Check for collision between a circle and an arbitrary collider.
	// Params:
	//	 other = Reference to the second circle collider component.
	bool ColliderCircle::IsCollidingWith(const Collider& other) const
	{
		Transform* transformOther = other.GetOwner()->GetComponent<Transform>();

		switch (other.GetColliderType())
		{
		case ColliderTypeCircle:
		{
			float radiusOther = static_cast<const ColliderCircle&>(other).radius;
			return Intersection2D::CircleCircleIntersection(Circle(transform->GetTranslation(), radius),
				Circle(transformOther->GetTranslation(), radiusOther));
			break;
		}
		default:
			return other.IsCollidingWith(*this);
			break;
		}
	}

	// Perform intersection test with ray.
	// Params:
	//   ray = The ray with which to test intersection.
	//   t   = The t value for the intersection.
	bool ColliderCircle::IsIntersectingWith(const LineSegment& ray, float& t) const
	{
		Vector2D intersection;
		LineSegment movingPoint(physics->GetOldTranslation(), transform->GetTranslation());
		return Intersection2D::StaticCircleLineIntersection(ray, Circle(transform->GetTranslation(), radius), t);
	}

	// Save object data to file.
	// Params:
	//   stream = The stream object used to save the object's data.
	void ColliderCircle::Serialize(FileStream& stream) const
	{
		stream.WriteVariable("radius", radius);
	}

	// Load object data from file
	// Params:
	//   stream = The stream object used to load the object's data.
	void ColliderCircle::Deserialize(FileStream& stream)
	{
		stream.ReadVariable("radius", radius);
	}

	// RTTI
	COMPONENT_SUBCLASS_SUBCLASS_DEFINITION(Collider, ColliderCircle)
}
