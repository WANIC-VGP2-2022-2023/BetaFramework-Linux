//------------------------------------------------------------------------------
//
// File Name:	ColliderRectangle.h
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
#include "ColliderRectangle.h"

// Math
#include "Intersection2D.h" // Circle collision
#include "Vertex.h"			// constructors

// Systems
#include <EngineCore.h>	// GetModule
#include <DebugDraw.h>	// AddCircle
#include "Space.h"		// GetCamera
#include "FileStream.h"

// Components
#include "GameObject.h" // GetTransform
#include "Transform.h"	// GetTranslation
#include "ColliderCircle.h"	// GetRadius
#include "Sprite.h"		// GetZDepth

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor for circle collider.
	// Params:
	//   extents = The rectangle's extents (half-width, half-height).
	ColliderRectangle::ColliderRectangle(const Vector2D& extents)
		: Collider(ColliderTypeRectangle), extents(extents)
	{
		SetName("ColliderRectangle");
	}

	// Debug drawing for colliders.
	void ColliderRectangle::Draw()
	{
		float zDepth = 0.01f;
		if (sprite)
			zDepth += sprite->GetZDepth();

		DebugDraw& debugDraw = *EngineGetModule(DebugDraw);
		debugDraw.SetCamera(GetOwner()->GetSpace()->GetCamera());
		debugDraw.AddRectangle(transform->GetTranslation(), extents, Colors::Green, zDepth);
	}

	// Get the rectangle collider's extents (half-width, half-height).
	// Returns:
	//	 The extents of the rectangle.
	const Vector2D& ColliderRectangle::GetExtents() const
	{
		return extents;
	}

	// Set the rectangle collider's extents (half-width, half-height).
	// Params:
	//   extents = The new extents of the rectangle.
	void ColliderRectangle::SetExtents(const Vector2D& extents_)
	{
		extents = extents_;
	}

	// Check for collision between a circle and an arbitrary collider.
	// Params:
	//	 other = Reference to the second circle collider component.
	bool ColliderRectangle::IsCollidingWith(const Collider& other) const
	{
		Transform* transformOther = other.GetOwner()->GetComponent<Transform>();
		using namespace Intersection2D;

		switch (other.GetColliderType())
		{
		case ColliderTypeRectangle:
		{
			const Vector2D& extentsOther = static_cast<const ColliderRectangle&>(other).extents;
			return RectangleRectangleIntersection(BoundingRectangle(transform->GetTranslation(), extents),
				BoundingRectangle(transformOther->GetTranslation(), extentsOther));
			break;
		}
		case ColliderTypeCircle:
		{
			float radiusOther = static_cast<const ColliderCircle&>(other).GetRadius();
			return RectangleCircleIntersection(BoundingRectangle(transform->GetTranslation(), extents),
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
	bool ColliderRectangle::IsIntersectingWith(const LineSegment& ray, float& t) const
	{
		return Intersection2D::RectangleLineIntersection(BoundingRectangle(transform->GetTranslation(), extents), ray, t);
	}

	// Save object data to file.
	// Params:
	//   stream = The stream object used to save the object's data.
	void ColliderRectangle::Serialize(FileStream& stream) const
	{
		stream.WriteVariable("extents", extents);
	}

	// Load object data from file
	// Params:
	//   stream = The stream object used to load the object's data.
	void ColliderRectangle::Deserialize(FileStream& stream)
	{
		stream.ReadVariable("extents", extents);
	}

	// RTTI
	COMPONENT_SUBCLASS_SUBCLASS_DEFINITION(Collider, ColliderRectangle)
}
