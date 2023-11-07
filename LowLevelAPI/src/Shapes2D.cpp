//------------------------------------------------------------------------------
//
// File Name:	Shapes2D.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Shapes2D.h"

//------------------------------------------------------------------------------

namespace Beta
{
	// Default constructor for ColliderLineSegment
	LineSegment::LineSegment()
	{
	}

	// Non-default constructor for ColliderLineSegment
	LineSegment::LineSegment(const Vector2D& p0, const Vector2D& p1)
		: start(p0), end(p1)
	{
		// Set points in point array to given points.

		// Calculate the direction and normal to the line segment using the two points.
		direction = (end - start).Normalized();
		normal = Vector2D(direction.y, -direction.x);
	}

	std::ostream & operator<<(std::ostream & os, const LineSegment & line)
	{
		os << "{ " << line.start << ", " << line.end << " }";
		return os;
	}

	std::istream& operator>>(std::istream & is, LineSegment & line)
	{
		is.ignore(10, '{'); is >> line.start;
		is.ignore(10, ','); is >> line.end;
		is.ignore(10, '}');

		line.direction = (line.end - line.start).Normalized();
		// Calculate normal
		line.normal = Vector2D(line.direction.y, -line.direction.x);

		return is;
	}

	BoundingRectangle::BoundingRectangle()
		: center(), extents(), bottom(0), left(0), right(0), top(0)
	{
	}

	BoundingRectangle::BoundingRectangle(const Vector2D & center, const Vector2D & extents_)
		: center(center), extents(Vector2D(abs(extents_.x), abs(extents_.y)))
	{
		top = center.y + extents.y;
		bottom = center.y - extents.y;
		right = center.x + extents.x;
		left = center.x - extents.x;
	}

	Circle::Circle(const Vector2D & center, float radius)
		: center(center), radius(radius)
	{
	}

	Circle::Circle()
		: center(), radius(0.0f)
	{
	}
}
