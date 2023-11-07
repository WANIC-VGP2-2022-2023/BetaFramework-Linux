//------------------------------------------------------------------------------
//
// File Name:	ColliderLine.cpp
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
#include "ColliderLine.h"

// Resources
#include "Vertex.h" // constructors

// Systems
#include "FileStream.h"
#include <EngineCore.h> // GetModule
#include <DebugDraw.h>	// AddLine
#include "Space.h"		// GetCamera

// Components
#include "RigidBody.h" // GetOldTranslation
#include "Transform.h" // GetTranslation
#include "GameObject.h" // GetTransform
#include "ColliderCircle.h" // GetRadius
#include "ColliderRectangle.h" // GetExtents
#include "Sprite.h"		// GetZDepth

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Create a new (line) collider component.
	// (Hint: Make sure to initialize the ColliderType correctly.)
	ColliderLine::ColliderLine(bool reflection)
		: Collider(ColliderTypeLine), reflection(reflection)
	{
		SetName("ColliderLine");
	}

	// Debug drawing for colliders.
	void ColliderLine::Draw()
	{
		DebugDraw* debugDraw = EngineGetModule(DebugDraw);

		for (unsigned i = 0; i < lineSegments.Size(); ++i)
		{
			LineSegment transformedSegment = GetLineWithTransform(i);

			// Draw line
			debugDraw->AddLineToList(transformedSegment.start, transformedSegment.end, Colors::Green);

			// Draw normal
			Vector2D midpoint = (transformedSegment.end + transformedSegment.start) / 2.0f;
			debugDraw->AddLineToList(midpoint, midpoint + transformedSegment.normal * 20.0f, Colors::Red);
		}

		debugDraw->SetCamera(GetOwner()->GetSpace()->GetCamera());
		float zDepth = 0.01f;
		if (sprite != nullptr)
			zDepth += sprite->GetZDepth();
		debugDraw->EndLineList(zDepth);
	}

	// Add a line segment to the line collider's line segment list.
	// Params:
	//	 collider = Pointer to the line collider component.
	//	 p0 = The line segment's starting position.
	//	 p1 = The line segment's ending position.
	void ColliderLine::AddLineSegment(const Vector2D & p0, const Vector2D & p1)
	{
		lineSegments.PushBack(LineSegment(p0, p1));
	}

	// Check for collision between a line collider and another arbitrary collider.
	// Params:
	//	 other = Reference to the other collider component.
	// Returns:
	//	 Return the results of the collision check.
	bool ColliderLine::IsCollidingWith(const Collider & other) const
	{
		Transform* transformOther = static_cast<Transform*>(other.GetOwner()->GetComponent<Transform>());
		Vector2D intersectionPoint;
		size_t numLines = lineSegments.Size();
		float t;
		LineSegment transformedSegment;
		using namespace Intersection2D;

		switch (other.GetColliderType())
		{
		case ColliderTypeCircle:
		{
			RigidBody* physicsOther = static_cast<RigidBody*>(other.GetOwner()->GetComponent<RigidBody>());
			LineSegment movingPoint(physicsOther->GetOldTranslation(), transformOther->GetTranslation());
			const ColliderCircle& circle = static_cast<const ColliderCircle&>(other);

			// If circle collides with any line segments, there is a collision
			for (size_t i = 0; i < numLines; ++i)
			{
				transformedSegment = GetLineWithTransform(static_cast<unsigned>(i));

				// If there's a collision...
				if (MovingCircleLineIntersection(transformedSegment, movingPoint, circle.GetRadius(), intersectionPoint, t))
				{
					// Perform reflection
					if (reflection)
					{
						MovingCircleLineReflection(*transformOther, *physicsOther, transformedSegment, movingPoint, circle.GetRadius(), intersectionPoint);
					}
					return true;
				}
			}

			// Otherwise, no collision.
			return false;
			break;
		}
		case ColliderTypeRectangle:
		{
			const ColliderRectangle& rect = static_cast<const ColliderRectangle&>(other);
			BoundingRectangle bounds = BoundingRectangle(transformOther->GetTranslation(), rect.GetExtents());

			// If circle collides with any line segments, there is a collision
			for (size_t i = 0; i < numLines; ++i)
			{
				transformedSegment = GetLineWithTransform(static_cast<unsigned>(i));

				// If there's a collision...
				if (RectangleLineIntersection(bounds, transformedSegment, t))
					return true;
			}
			return false;
			break;
		}
		case ColliderTypeLine:
		{
			const ColliderLine& lines = static_cast<const ColliderLine&>(other);
			size_t numLinesOther = lines.lineSegments.Size();
			LineSegment transformedSegmentOther;

			// If circle collides with any line segments, there is a collision
			for (size_t i = 0; i < numLines; ++i)
			{
				transformedSegment = GetLineWithTransform(static_cast<unsigned>(i));

				for (size_t j = 0; j < numLinesOther; ++j)
				{
					transformedSegmentOther = lines.GetLineWithTransform(static_cast<unsigned>(j));

					if (LineLineIntersection(transformedSegment, transformedSegmentOther,
						intersectionPoint, t))
					{
						return true;
					}
				}
			}
			return false;
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
	bool ColliderLine::IsIntersectingWith(const LineSegment & ray, float& t) const
	{
		size_t numLines = lineSegments.Size();
		Vector2D intersectionPoint;
		float minT = std::numeric_limits<float>::max();

		LineSegment transformedSegment;
		LineSegment transformedSegmentOther;
		bool result = false;

		// If circle collides with any line segments, there is a collision
		for (size_t i = 0; i < numLines; ++i)
		{
			transformedSegment = GetLineWithTransform(static_cast<unsigned>(i));

			if (Intersection2D::LineLineIntersection(transformedSegment, ray,
				intersectionPoint, t))
			{
				minT = std::min(t, minT);
				result = true;
			}
		}
		t = minT;
		return result;
	}

	// Save object data to file.
	// Params:
	//   stream = The stream object used to save the object's data.
	void ColliderLine::Serialize(FileStream & stream) const
	{
		stream.WriteVariable("lineCount", lineSegments.Size());
		stream.WriteValue("lines : ");
		stream.BeginScope();
		for (unsigned i = 0; i < lineSegments.Size(); ++i)
		{
			stream.WriteValue(lineSegments[i]);
		}
		stream.EndScope();
	}

	// Load object data from file
	// Params:
	//   stream = The stream object used to load the object's data.
	void ColliderLine::Deserialize(FileStream & stream)
	{
		unsigned count;
		stream.ReadVariable("lineCount", count);
		stream.ReadSkip("lines");
		stream.ReadSkip(':');
		stream.BeginScope();
		for (unsigned i = 0; i < count; ++i)
		{
			LineSegment segment;
			stream.ReadValue(segment);
			lineSegments.PushBack(segment);
		}
		stream.EndScope();
	}

	// Gets a line segment that incorporates the transform of the object
	LineSegment ColliderLine::GetLineWithTransform(unsigned index) const
	{
		Vector2D transformedStart = lineSegments[index].start;
		Vector2D transformedEnd = lineSegments[index].end;

		transformedStart = transform->GetMatrix() * transformedStart;
		transformedEnd = transform->GetMatrix() * transformedEnd;

		return LineSegment(transformedStart, transformedEnd);
	}

	// RTTI
	COMPONENT_SUBCLASS_SUBCLASS_DEFINITION(Collider, ColliderLine)
}
