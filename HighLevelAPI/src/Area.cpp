//------------------------------------------------------------------------------
//
// File Name:	Area.cpp
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
#include "Area.h"

// Systems
#include <EngineCore.h> // GetModule
#include <DebugDraw.h>	// AddRectangle
#include "GameObject.h" // GetSpace
#include "Space.h"		// GetCamera

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Create a new transform object.
	// (Hint: Failing to initialize the scale values to non-zero values will result in invisible sprites.)
	// Params:
	//	 x = Initial world position on the x-axis.
	//	 y = Initial world position on the y-axis.
	Area::Area(float x, float y)
		: Transform(x, y), size(Vector2D(1, 1)), offset(Vector2D(0, 0)), zDepth(0.0f)
	{
		SetName("Area");
	}

	// Area constructor.
	// Params:
	//	 translation = World position of the object.
	//   scale		 = Width and height of the object.
	//   rotation	 = Rotation of the object about the z-axis.
	Area::Area(Vector2D translation, Vector2D scale, float rotation)
		: Transform(translation, scale, rotation), size(Vector2D(1, 1)),
		offset(Vector2D(0, 0)), zDepth(0.0f)
	{
		SetName("Area");
	}

	// Draw the bounding rectangle of the area.
	void Area::Draw()
	{
		const BoundingRectangle& rect = GetBounds();
		DebugDraw& debugDraw = *EngineGetModule(DebugDraw);
		debugDraw.SetCamera(GetOwner()->GetSpace()->GetCamera());
		debugDraw.AddRectangle(rect.center, rect.extents,  Colors::White, zDepth);
	}

	// Set the offset of a transform component.
	// Params:
	//	 transform = Pointer to the transform component.
	//	 offset = Pointer to a offset vector.
	void Area::SetOffset(const Vector2D& offset_)
	{
		offset = offset_;
	}

	// Set the scale of a transform component.
	// Params:
	//	 transform = Pointer to the transform component.
	//	 translation = Pointer to a scale vector.
	void Area::SetSize(const Vector2D& size_)
	{
		size = size_;
	}

	// Set the zDepth used for debug drawing.
	// Params:
	//   depth = The z depth that will be used.
	void Area::SetZDepth(float depth)
	{
		zDepth = depth;
	}

	// Returns a bounding rectangle using translation and (half) scale.
	const BoundingRectangle Area::GetBounds() const
	{
		Vector2D extents = GetScale() / 2.0f;
		extents.x = fabsf(extents.x);
		extents.y = fabsf(extents.y);
		extents.x *= size.x;
		extents.y *= size.y;

		return BoundingRectangle(GetTranslation() + offset, extents);
	}

	// RTTI
	COMPONENT_SUBCLASS_SUBCLASS_DEFINITION(Transform, Area)
}
