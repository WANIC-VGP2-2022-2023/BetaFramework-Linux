//------------------------------------------------------------------------------
//
// File Name:	Interpolation.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		Beta Engine
// Course:		WANIC VGP2
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Vector2D.h"
#include "Color.h"

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

typedef enum InterpolationMode
{
	IM_Linear,
	IM_Spherical,
	IM_LinearNormalized
}InterpolationMode;

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Clamp the given value between the minimum and maximum.
BE_C_API float Clamp(float value, float min, float max);

// Linearly interpolates between two floating point values.
BE_C_API float InterpolateFloat(float start, float end, float percent);

// Interpolates between two vectors using the specified interpolation method (defaults to linear interpolation).
BE_C_API Vector2D InterpolateVector(const Vector2D* start, const Vector2D* end, float percent, InterpolationMode mode);

// Linearly interpolates between two colors.
BE_C_API Color InterpolateColor(const Color* start, const Color* end, float percent);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif
