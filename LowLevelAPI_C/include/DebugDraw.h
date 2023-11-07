//------------------------------------------------------------------------------
//
// File Name:	DebugDraw.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaEngine
// Course:		VGP2
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Shapes2D.h"
#include "Color.h"

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct Vector2D Vector2D;
typedef struct Mesh Mesh;
typedef struct Camera Camera;

//------------------------------------------------------------------------------
// Public Functions
//------------------------------------------------------------------------------

// Adds a line to the list of lines to be drawn. Note that actual drawing does not occur 
// unless EndLineList is called.
// Params:
//   start = The start point of the line segment.
//   end   = The end point of the line segment.
//   color = The color of the line. Defaults to white.
BE_C_API void DebugDrawAddLineToList(const Vector2D* start, const Vector2D* end, const Color* color);

// Ends the line strip formed using previous calls to AddLineToList.
// Params:
//   zDepth = Depth of the object (when using a perspective camera).
BE_C_API void DebugDrawEndLineList(float zDepth);

// Adds a line to the list of lines to be drawn. Note that actual drawing does not occur 
// until EndLineList is called.
// Params:
//   center = The center point of the circle.
//   radius = The radius of the circle.
//   color  = The color of the circle. Defaults to white.
//   zDepth = Depth of the object (when using a perspective camera).
BE_C_API void DebugDrawAddCircle(const Vector2D* center, float radius,
	const Color* color, float zDepth);

// Adds a line to the list of lines to be drawn. Note that actual drawing does not occur 
// until EndLineList is called.
// Params:
//   center  = The center point of the rectangle.
//   extents = The extents (half-width, half-height) of the rectangle.
//   color   = The color of the circle. Defaults to white.
//   zDepth = Depth of the object (when using a perspective camera).
BE_C_API void DebugDrawAddRectangle(const Vector2D* center, const Vector2D* extents,
	const Color* color, float zDepth);

// Set the camera used for debug drawing. Any shapes added before the next
// call to SetCamera will use the provided camera.
// Params:
//   camera = The camera used to determine where debug objects will be drawn.
BE_C_API void DebugDrawSetCamera(Camera* camera);

// Enables or disables debug drawing. 
// Note that debug drawing is only possible in debug mode.
BE_C_API void DebugDrawSetEnabled(bool value);

// Returns a boolean value that indicates whether debug drawing is enabled.
BE_C_API bool DebugDrawIsEnabled();

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif
