//------------------------------------------------------------------------------
//
// File Name:	Vector2D.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#define FP_EPSILON 0.0001f

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// Frequently, vector and matrix structures are declared publicly.  While an
// interface is typically provided with these structures, these are two common
// exceptions to the coding best practice of "hiding information".
typedef struct Vector2D
{
	float x;
	float y;
}Vector2D;

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Return a vector with x and y set to 0.
BE_C_API Vector2D Vector2DZero();

// Return the length of the vector squared.
BE_C_API float Vector2DMagnitudeSquared(const Vector2D* v);
// Return the length of the vector.
BE_C_API float Vector2DMagnitude(const Vector2D* v);
// Return a unit vector in the direction of v.
BE_C_API Vector2D Vector2DNormalized(const Vector2D* v);

// Return the distance between p0 and p1 squared.
BE_C_API float Vector2DDistanceSquared(const Vector2D* p0, const Vector2D* p1);
// Return the distance between p0 and p1.
BE_C_API float Vector2DDistance(const Vector2D* p0, const Vector2D* p1);
// Return the midpoint of p0 and p1.
BE_C_API Vector2D Vector2DMidpoint(const Vector2D* p0, const Vector2D* p1);

// Return the dot product of v0 and v1.
BE_C_API float Vector2DDotProduct(const Vector2D* v0, const Vector2D* v1);

// Return the result of Rotating v by the given angle.
BE_C_API Vector2D Vector2DRotate(const Vector2D* v, float angle);
// Return the direction vector of an angle given in degrees.
BE_C_API Vector2D Vector2DFromAngleDegrees(float angle);
// Return the direction vector of an angle given in radians.
BE_C_API Vector2D Vector2DFromAngleRadians(float angle);

// Return the sum of v0 and v1.
BE_C_API Vector2D Vector2DAdd(const Vector2D* v0, const Vector2D* v1);
// Return the difference of v0 and v1 (v0 - v1).
BE_C_API Vector2D Vector2DSubtract(const Vector2D* v0, const Vector2D* v1);

// Return v scaled by the given value.
BE_C_API Vector2D Vector2DScale(const Vector2D* v, float scalar);
// Return a vector in the opposite direction of v (-v).
BE_C_API Vector2D Vector2DNegate(const Vector2D* v);
// Return the sum of v0 and v1 scaled by s (v0 + v1 * s)
BE_C_API Vector2D Vector2DScaleAdd(const Vector2D* v0, const Vector2D* v1, float scalar);

// Add v0 and v1, store the result in v0 (v0 += v1)
BE_C_API void Vector2DAddAssign(Vector2D* v0, const Vector2D* v1);
// Subtract v0 and v1, store the result in v0 (v0 -= v1)
BE_C_API void Vector2DSubtractAssign(Vector2D* v0, const Vector2D* v1);
// Multiply the scalar and v, store the result in v (v *= s)
BE_C_API void Vector2DScaleAssign(Vector2D* v, float scalar);

// Check whether two floats are within an epsilon of each other.
BE_C_API bool FloatAlmostEqual(float first, float second, float epsilon);
// Check whether two vectors are within an epsilon of each other.
BE_C_API bool Vector2DAlmostEqual(const Vector2D* first, const Vector2D* second, float epsilon);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif
