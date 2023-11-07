//------------------------------------------------------------------------------
//
// File Name:	Interpolation.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		Beta Engine
// Course:		WANIC VGP2
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Interpolation.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

Vector2D InterpolateSpherical(const Vector2D* start, const Vector2D* end, float percent);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Clamp the given value between the minimum and maximum.
float Clamp(float value, float minimum, float maximum)
{
	if (value < minimum)
		value = minimum;
	if (value > maximum)
		value = maximum;
	return value;
}

// Linearly interpolates between two floating point values.
float InterpolateFloat(float start, float end, float percent)
{
	return (start + percent * (end - start));
}

// Interpolates between two vectors using the specified interpolation method (defaults to linear interpolation).
Vector2D InterpolateVector(const Vector2D* start, const Vector2D* end, float percent, InterpolationMode mode)
{
	Vector2D result = *start;

	switch (mode)
	{
	case IM_Linear:
		result.x = InterpolateFloat(start->x, end->x, percent);
		result.y = InterpolateFloat(start->y, end->y, percent);
		break;
	case IM_LinearNormalized:
		result.x = InterpolateFloat(start->x, end->x, percent);
		result.y = InterpolateFloat(start->y, end->y, percent);
		result = Vector2DNormalized(&result);
		break;
	case IM_Spherical:
		result = InterpolateSpherical(start, end, percent);
		break;
	default:
		break;
	}

	return result;
}

// Linearly interpolates between two colors.
Color InterpolateColor(const Color* start, const Color* end, float percent)
{
	Color result;
	result.r = InterpolateFloat(start->r, end->r, percent);
	result.g = InterpolateFloat(start->g, end->g, percent);
	result.b = InterpolateFloat(start->b, end->b, percent);
	result.a = InterpolateFloat(start->a, end->a, percent);
	return result;
}

//------------------------------------------------------------------------------
// Private Function Definitions:
//------------------------------------------------------------------------------

Vector2D InterpolateSpherical(const Vector2D* start, const Vector2D* end, float percent)
{
	// Dot product - the cosine of the angle between 2 vectors.
	float dot = Vector2DDotProduct(start, end);

	// Clamp it to be in the range of Acos()
	// This may be unnecessary, but floating point
	// precision can be a fickle mistress.
	dot = std::clamp(dot, -1.0f, 1.0f);

	// Acos(dot) returns the angle between start and end,
	// And multiplying that by percent returns the angle between
	// start and the final result.
	float theta = acosf(dot) * percent;
	Vector2D relativeVec = Vector2DScale(start, dot);
	relativeVec = Vector2DSubtract(end, &relativeVec);

	// Orthonormal basis
	relativeVec = Vector2DNormalized(&relativeVec);

	// The final result.
	Vector2D result = Vector2DScale(start, cosf(theta));
	relativeVec = Vector2DScale(&relativeVec, sinf(theta));
	return Vector2DAdd(&result, &relativeVec);
}

//------------------------------------------------------------------------------
