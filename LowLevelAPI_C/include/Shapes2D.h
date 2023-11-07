//------------------------------------------------------------------------------
//
// File Name:	Shapes2D.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Vector2D.h"

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

typedef struct LineSegment
{
	// Points marking each end of a single line segment.
	Vector2D start;
	Vector2D end;

	// Direction of the line segment, precomputed from start and end.
	Vector2D direction;

	// Normal of the line segment, precomputed from start and end.
	Vector2D normal;
}LineSegment;

typedef struct BoundingRectangle
{
	Vector2D center;  // Center of the rectangle.
	Vector2D extents; // HALF the width and height of the rectangle.
	float left;		  // X-coord of the left side of the rectangle
	float right;	  // X-coord of the right side of the rectangle
	float top;		  // Y-coord of the top side of the rectangle
	float bottom;	  // Y-coord of the bottom side of the rectangle
}BoundingRectangle;

typedef struct Circle
{
	Vector2D center; // Center point of circle.
	float radius;    // Radius of circle.
}Circle;

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Creates a line segment and calculates its direction and normal.
BE_C_API LineSegment LineSegmentCreate(const Vector2D* p0, const Vector2D* p1);

// Creates a bounding rectangle and calculates the left, right, top, bottom values.
BE_C_API BoundingRectangle BoundingRectangleCreate(const Vector2D* center, const Vector2D* extents);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif
