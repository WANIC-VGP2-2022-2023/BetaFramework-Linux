//------------------------------------------------------------------------------
//
// File Name:	DebugDraw.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaEngine
// Course:		VGP2
//
// Copyright � 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "DebugDraw.h"

#include "EngineModulesGlobal.h"
#include "../../LowLevelAPI/include/DebugDraw.h"

//------------------------------------------------------------------------------

using Beta::DebugDraw;

//------------------------------------------------------------------------------
// Public Functions
//------------------------------------------------------------------------------

// Adds a line to the list of lines to be drawn. Note that actual drawing does not occur
// unless EndLineList is called.
// Params:
//   start = The start point of the line segment.
//   end   = The end point of the line segment.
//   color = The color of the line. Defaults to white.
void DebugDrawAddLineToList(const Vector2D* start, const Vector2D* end, const Color* color)
{
	Beta::debugDraw->AddLineToList({ start->x, start->y }, { end->x, end->y },
		{color->r, color->g, color->b, color->a });
}

// Ends the line strip formed using previous calls to AddLineToList.
// Params:
//   camera = The camera used to determine where debug objects will be drawn.
//   zDepth = Depth of the object (when using a perspective camera).
void DebugDrawEndLineList(float zDepth)
{
	Beta::debugDraw->EndLineList(zDepth);
}

// Adds a line to the list of lines to be drawn. Note that actual drawing does not occur
// until EndLineList is called.
// Params:
//   center = The center point of the circle.
//   radius = The radius of the circle.
//   camera = The camera used to determine where debug objects will be drawn.
//   color  = The color of the circle. Defaults to white.
//   zDepth = Depth of the object (when using a perspective camera).
void DebugDrawAddCircle(const Vector2D* center, float radius,
	const Color* color, float zDepth)
{
	Beta::debugDraw->AddCircle({ center->x, center->y }, radius,
		{ color->r, color->g, color->b, color->a }, zDepth);
}

// Adds a line to the list of lines to be drawn. Note that actual drawing does not occur
// until EndLineList is called.
// Params:
//   center  = The center point of the rectangle.
//   extents = The extents (half-width, half-height) of the rectangle.
//   camera  = The camera used to determine where debug objects will be drawn.
//   color   = The color of the circle. Defaults to white.
//   zDepth = Depth of the object (when using a perspective camera).
void DebugDrawAddRectangle(const Vector2D* center, const Vector2D* extents,
	const Color* color, float zDepth)
{
	Beta::debugDraw->AddRectangle({ center->x, center->y }, { extents->x, extents->y },
		{ color->r, color->g, color->b, color->a }, zDepth);
}

void DebugDrawSetCamera(Camera* camera_)
{
	Beta::debugDraw->SetCamera(*reinterpret_cast<Beta::Camera*>(camera_));
}

// Enables or disables debug drawing.
// Note that debug drawing is only possible in debug mode.
void DebugDrawSetEnabled(bool value)
{
	Beta::debugDraw->SetEnabled(value);
}

// Returns a boolean value that indicates whether debug drawing is enabled.
bool DebugDrawIsEnabled()
{
	return Beta::debugDraw->IsEnabled();
}

//------------------------------------------------------------------------------
