//------------------------------------------------------------------------------
//
// File Name:	Shapes2D.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Shapes2D.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Creates a line segment and calculates its direction and normal.
LineSegment LineSegmentCreate(const Vector2D* p0, const Vector2D* p1)
{
	LineSegment line;
	line.start = *p0;
	line.end = *p1;

	// Calculate the direction and normal to the line segment using the two points.
	line.direction = Vector2DSubtract(&line.end, &line.start);
	line.direction = Vector2DNormalized(&line.direction);
	line.normal = { line.direction.y, -line.direction.x };

	return line;
}

// Creates a bounding rectangle and calculates the left, right, top, bottom values.
BoundingRectangle BoundingRectangleCreate(const Vector2D* center, const Vector2D* extents)
{
	BoundingRectangle rect;
	rect.center = *center;
	rect.extents = *extents;

	rect.top = center->y + extents->y;
	rect.bottom = center->y - extents->y;
	rect.right = center->x + extents->x;
	rect.left = center->x - extents->x;

	return rect;
}

//------------------------------------------------------------------------------
