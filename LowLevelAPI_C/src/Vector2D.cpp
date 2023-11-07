//------------------------------------------------------------------------------
//
// File Name:	Vector2D.cpp
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
#include "Vector2D.h"

#include "Matrix2D.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------ 

// Return a vector with x and y set to 0.
Vector2D Vector2DZero()
{
	return  { 0.0f, 0.0f };
}

// Return the length of the vector squared.
float Vector2DMagnitudeSquared(const Vector2D* v)
{
	return v->x* v->x + v->y * v->y;
}

// Return the length of the vector.
float Vector2DMagnitude(const Vector2D* v)
{
	float lengthSquared = Vector2DMagnitudeSquared(v);
	return lengthSquared * lengthSquared;
}

// Return a unit vector in the direction of v.
Vector2D Vector2DNormalized(const Vector2D* v)
{
	float length = Vector2DMagnitude(v);
	if (length == 0.0f)
		return { v->x, v->y };
	return Vector2DScale(v, 1.0f / length);
}

// Return the distance between p0 and p1 squared.
float Vector2DDistanceSquared(const Vector2D* p0, const Vector2D* p1)
{
	Vector2D difference = Vector2DSubtract(p0, p1);
	return Vector2DMagnitudeSquared(&difference);
}

// Return the distance between p0 and p1.
float Vector2DDistance(const Vector2D* p0, const Vector2D* p1)
{
	float distanceSquared = Vector2DDistanceSquared(p0, p1);
	return distanceSquared * distanceSquared;
}

// Return the midpoint of p0 and p1.
Vector2D Vector2DMidpoint(const Vector2D* p0, const Vector2D* p1)
{
	Vector2D sum = Vector2DAdd(p0, p1);
	return Vector2DScale(&sum, 0.5f);
}

// Return the dot product of v0 and v1.
float Vector2DDotProduct(const Vector2D* v0, const Vector2D* v1)
{
	return v0->x* v1->x + v0->y * v1->y;
}

// Return the result of Rotating v by the given angle.
Vector2D Vector2DRotate(const Vector2D* v, float angle)
{
	Matrix2D rotation = Matrix2DRotationRadians(angle);
	return Matrix2DMultiplyVector2D(&rotation, v);
}

// Return the direction vector of an angle given in degrees.
Vector2D Vector2DFromAngleDegrees(float angle)
{
	return Vector2DFromAngleRadians(angle * (float)M_PI / 180.0f);
}

// Return the direction vector of an angle given in radians.
Vector2D Vector2DFromAngleRadians(float angle)
{
	return { cosf(angle), sinf(angle) };
}

// Return the sum of v0 and v1.
Vector2D Vector2DAdd(const Vector2D* v0, const Vector2D* v1)
{
	return { v0->x + v1->x, v0->y + v1->y };
}

// Return the difference of v0 and v1.
Vector2D Vector2DSubtract(const Vector2D* v0, const Vector2D* v1)
{
	return { v0->x - v1->x, v0->y - v1->y };
}

// Return v scaled by the given value.
Vector2D Vector2DScale(const Vector2D* v, float scalar)
{
	return { scalar * v->x, scalar * v->y };
}

// Return a vector in the opposite direction of v.
Vector2D Vector2DNegate(const Vector2D* v)
{
	return Vector2DScale(v, -1.0f);
}

// Return the sum of v0 and v1 scaled by s (v0 + v1 * s)
Vector2D Vector2DScaleAdd(const Vector2D* v0, const Vector2D* v1, float scalar)
{
	Vector2D result = Vector2DScale(v1, scalar);
	return Vector2DAdd(v0, &result);
}

// Add v0 and v1, store the result in v0 (v0 += v1)
void Vector2DAddAssign(Vector2D* v0, const Vector2D* v1)
{
	v0->x += v1->x;
	v0->y += v1->y;
}

// Subtract v0 and v1, store the result in v0 (v0 -= v1)
void Vector2DSubtractAssign(Vector2D* v0, const Vector2D* v1)
{
	v0->x -= v1->x;
	v0->y -= v1->y;
}

// Multiply the scalar and v, store the result in v (v *= s)
void Vector2DScaleAssign(Vector2D* v, float scalar)
{
	v->x *= scalar;
	v->y *= scalar;
}

// Check whether two floats are within an epsilon of each other.
bool FloatAlmostEqual(float first, float second, float epsilon)
{
	return fabsf(first - second) < epsilon;
}

// Check whether two vectors are within an epsilon of each other.
bool Vector2DAlmostEqual(const Vector2D* first, const Vector2D* second, float epsilon)
{
	return FloatAlmostEqual(first->x, second->x, epsilon) && FloatAlmostEqual(first->y, second->y, epsilon);
}
