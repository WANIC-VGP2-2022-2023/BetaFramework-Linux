//------------------------------------------------------------------------------
//
// File Name:	RaycastTest.cpp
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
#include "RaycastTest.h"

// Systems
#include <FileStream.h>
#include <DebugDraw.h>
#include <Space.h>		// GetCamera
#include <Quadtree.h>	// CastResult
#include <EngineCore.h> // GetModule

// Components
#include <GameObject.h> // GetComponent
#include <Transform.h>	// GetTranslation

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	// Params:
	//   castDistance = The distance of the raycast.
	RaycastTest::RaycastTest(float castDistance)
		: Component("RaycastTest"), castDistance(castDistance), hitObject(false)
	{
	}

	// Initialize this component (happens at object creation).
	void RaycastTest::Initialize()
	{
		// Get components
		transform = GetOwner()->GetComponent<Transform>();
	}

	// Fixed update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void RaycastTest::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		// Find objects in ray
		//CastRayForward();

		// Find first object in ray
		CastRayForwardClosest();

		// Draw the ray
		DrawRay();
	}

	// Write object data to file
	// Params:
	//   stream = The stream that is writing this object to a file.
	void RaycastTest::Serialize(FileStream& stream) const
	{
		stream.WriteVariable("castDistance", castDistance);
	}

	// Read object data from a file
	// Params:
	//   stream = The stream that is reading this object's data from a file.
	void RaycastTest::Deserialize(FileStream& stream)
	{
		stream.ReadVariable("castDistance", castDistance);
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Cast a ray in the object's facing direction
	void RaycastTest::CastRayForward()
	{
		hitObject = false;

		Vector2D direction = Vector2D::FromAngleRadians(transform->GetRotation());
		const Vector2D& start = transform->GetTranslation();

		// Cast ray and ignore owner
		Array<CastResult> results;
		GetOwner()->GetSpace()->GetObjectManager().CastRay(
			start, direction, castDistance, results, GetOwner()->GetName());

		if (results.Size() != 0)
		{
			hitObject = true;
			//std::cout << "Raycast succeeded!" << std::endl;
		}
	}

	void RaycastTest::CastRayForwardClosest()
	{
		hitObject = false;

		Vector2D direction = Vector2D::FromAngleRadians(transform->GetRotation());
		const Vector2D& start = transform->GetTranslation();

		// Cast ray and ignore owner
		GameObject* closest = GetOwner()->GetSpace()->GetObjectManager().CastRayClosest(
			start, direction, castDistance, GetOwner()->GetName());

		if (closest != nullptr)
		{
			hitObject = true;
			//std::cout << "Raycast succeeded: " << closest->GetName() << std::endl;
		}
	}

	// Draw the ray
	void RaycastTest::DrawRay() const
	{
		Color drawColor = hitObject ? Colors::Red : Colors::Green;

		Vector2D direction = Vector2D::FromAngleRadians(transform->GetRotation());
		const Vector2D& start = transform->GetTranslation();
		Vector2D end = start + direction * castDistance;

		DebugDraw& debugDraw = *EngineGetModule(DebugDraw);
		debugDraw.SetCamera(GetOwner()->GetSpace()->GetCamera());

		debugDraw.AddLineToList(start, end, drawColor);
		debugDraw.EndLineList();
	}

	COMPONENT_SUBCLASS_DEFINITION(RaycastTest)
}

//------------------------------------------------------------------------------
