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

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------ 

	// Constructors (3)
	Vector2D::Vector2D()
		: x(0.0f), y(0.0f)
	{
	}

	Vector2D::Vector2D(float value)
		: x(value), y(value)
	{
	}

	Vector2D::Vector2D(float x_, float y_)
		: x(x_), y(y_)
	{
	}

	// Other functions (5)
	float Vector2D::MagnitudeSquared() const
	{
		return x * x + y * y;
	}

	float Vector2D::Magnitude() const
	{
		return sqrtf(MagnitudeSquared());
	}

	float Vector2D::DistanceSquared(const Vector2D& other) const
	{
		float xDiff = x - other.x;
		float yDiff = y - other.y;
		return xDiff * xDiff + yDiff * yDiff;
	}

	float Vector2D::Distance(const Vector2D& other) const
	{
		return sqrtf(DistanceSquared(other));
	}

	Vector2D Vector2D::Midpoint(const Vector2D& other) const
	{
		return (*this + other) * 0.5f;
	}

	float Vector2D::DotProduct(const Vector2D& other) const
	{
		return x * other.x + y * other.y;
	}

	Vector2D Vector2D::Rotate(float angle) const
	{
		Matrix2D rotation = Matrix2D::RotationMatrixRadians(angle);
		return rotation * *this;
	}

	Vector2D Vector2D::Normalized() const
	{
		if (Magnitude() == 0.0f)
			return *this;
		return *this / Magnitude();
	}

	// Static functions (2)
	Vector2D Vector2D::FromAngleDegrees(float angle)
	{
		return Vector2D::FromAngleRadians(angle * (float)M_PI / 180.0f);
	}

	Vector2D Vector2D::FromAngleRadians(float angle)
	{
		return Vector2D(cosf(angle), sinf(angle));
	}

	// Operators (7)
	Vector2D Vector2D::operator+(const Vector2D& other) const
	{
		return Vector2D(x + other.x, y + other.y);
	}

	Vector2D Vector2D::operator-(const Vector2D& other) const
	{
		return *this + -other;
	}

	Vector2D Vector2D::operator*(float scalar) const
	{
		return Vector2D(x * scalar, y * scalar);
	}

	Vector2D Vector2D::operator/(float scalar) const
	{
		return Vector2D(x / scalar, y / scalar);
	}

	Vector2D Vector2D::operator-() const
	{
		return Vector2D(-x, -y);
	}

	Vector2D& Vector2D::operator+=(const Vector2D& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	Vector2D& Vector2D::operator-=(const Vector2D& other)
	{
		return *this += -other;
	}

	Vector2D& Vector2D::operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	Vector2D & Vector2D::operator/=(float scalar)
	{
		x /= scalar;
		y /= scalar;
		return *this;
	}

	//------------------------------------------------------------------------------
	// Public Functions (non-member, non-friend)
	//------------------------------------------------------------------------------

	std::ostream& operator<<(std::ostream& os, const Vector2D& vector)
	{
		os << "{ " << vector.x << ", " << vector.y << " }";
		return os;
	}

	std::istream & operator>>(std::istream& is, Vector2D& vector)
	{
		float x, y;
		constexpr long long maxStreamSize = std::numeric_limits<std::streamsize>::max();
		is.ignore(maxStreamSize, '{'); is >> x;
		is.ignore(maxStreamSize, ','); is >> y;
		is.ignore(maxStreamSize, '}');
		vector.x = x; vector.y = y;
		return is;
	}

	Vector2D operator*(float scalar, const Vector2D& vector)
	{
		return vector * scalar;
	}

	// Utility functions for floating point comparisons
	bool AlmostEqual(float first, float second, float epsilon)
	{
		return fabsf(first - second) < epsilon;
	}

	bool AlmostEqual(Vector2D first, Vector2D second, float epsilon)
	{
		return AlmostEqual(first.x, second.x, epsilon) && AlmostEqual(first.y, second.y, epsilon);
	}
}
