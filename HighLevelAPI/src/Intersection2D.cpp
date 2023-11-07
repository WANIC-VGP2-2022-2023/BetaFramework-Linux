//------------------------------------------------------------------------------
//
// File Name:	Intersection2D.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		Project 7
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Intersection2D.h"

// STD
#include <limits>

// Components
#include "RigidBody.h" // GetOldTranslation
#include "Transform.h" // GetTranslation

//------------------------------------------------------------------------------

namespace Beta
{
	namespace Intersection2D
	{
		//------------------------------------------------------------------------------
		// Private Function Declarations:
		//------------------------------------------------------------------------------

		// Checks whether a point is between two parallel lines.
		// Params:
		//   point = The point in question.
		//   firstLine = A point on the first line.
		//   secondLine = A point on the second line.
		//   normal = Normal vector to the two lines.
		// Returns:
		//   True if the point is between the two lines, false otherwise.
		bool PointIsBetweenLines(const Vector2D& point, const Vector2D& firstLine, const Vector2D& secondLine, const Vector2D& normal, float distance);

		// Reflects a point over a line.
		// Params:
		//   point = The point being reflected.
		//   line = The line the point will be reflected over.
		// Returns:
		//   The reflected point.
		Vector2D ReflectPointOverLine(const Vector2D& point, const LineSegment& line, float radius);

		//------------------------------------------------------------------------------
		// Public Function Definitions:
		//------------------------------------------------------------------------------

		// Check whether a point and rectangle intersect.
		// Params:
		//  point = The point in the intersection.
		//  rect  = The rectangle in the intersection.
		// Returns:
		//   True if intersection, false otherwise.
		bool PointRectangleIntersection(const Vector2D& point, const BoundingRectangle& rect)
		{
			// X < left side
			if (point.x < rect.left)
				return false;

			// X > right side
			if (point.x > rect.right)
				return false;

			// Y < bottom side
			if (point.y < rect.bottom)
				return false;

			// Y > top side
			if (point.y > rect.top)
				return false;

			// Point is intersecting!
			return true;
		}

		// Check whether two rectangles intersect.
		// Params:
		//  rect1 = The first rectangle.
		//	rect2 = The second rectangle.
		// Returns:
		//   True if intersection, false otherwise.
		bool RectangleRectangleIntersection(const BoundingRectangle& rect1, const BoundingRectangle& rect2)
		{
			if (rect1.bottom > rect2.top)
				return false;
			if (rect1.top < rect2.bottom)
				return false;
			if (rect1.left > rect2.right)
				return false;
			if (rect1.right < rect2.left)
				return false;

			// All other cases eliminated, so there must be an intersection.
			return true;
		}

		// Check whether a rectangle and circle intersect.
		// Params:
		//  rect = The rectangle.
		//  circle  = The circle.
		// Returns:
		//   True if intersection, false otherwise.
		bool RectangleCircleIntersection(const BoundingRectangle& rect, const Circle& circle)
		{
			// Find point on rectangle closest to circle
			Vector2D closest = rect.center;
			if (circle.center.x < rect.center.x)
				closest.x = std::max(rect.left, circle.center.x);
			else if (circle.center.x > rect.center.x)
				closest.x = std::min(rect.right, circle.center.x);

			if (circle.center.y < rect.center.y)
				closest.y = std::max(rect.bottom, circle.center.y);
			else if (circle.center.y > rect.center.y)
				closest.y = std::min(rect.top, circle.center.y);

			// If distance from closest point to circle center <= radius
			if (closest.DistanceSquared(circle.center) <= circle.radius * circle.radius)
			{
				// Intersection
				return true;
			}
			else
			{
				// No intersection
				return false;
			}
		}

		// Check whether a rectangle and line intersect.
		// Params:
		//  first   = The rectangle.
		//  second  = Start and end of second line segment.
		//  t = T value of intersection (for checking closest intersection).
		// Returns:
		//   True if intersection, false otherwise.
		bool RectangleLineIntersection(const BoundingRectangle& first, const LineSegment& second, float& t)
		{
			// First do simple bounds check
			Vector2D center = second.start.Midpoint(second.end);
			Vector2D extents = (second.end - second.start) / 2.0f;
			extents.x = fabsf(extents.x); extents.y = fabsf(extents.y);
			BoundingRectangle rayBounds = BoundingRectangle(center, extents);

			// Not even close
			if (!RectangleRectangleIntersection(first, rayBounds))
				return false;

			// Create 4 line segments and perform more intensive collision tests
			bool result = false;
			Vector2D intersection;
			float minT = std::numeric_limits<float>::max();

			const size_t numLines = 4;
			LineSegment lines[numLines];

			// left
			lines[0] = LineSegment(Vector2D(first.left, first.top), Vector2D(first.left, first.bottom));
			// top
			lines[1] = LineSegment(Vector2D(first.left, first.top), Vector2D(first.right, first.top));
			// right
			lines[2] = LineSegment(Vector2D(first.right, first.top), Vector2D(first.right, first.bottom));
			// bottom
			lines[3] = LineSegment(Vector2D(first.left, first.bottom), Vector2D(first.right, first.bottom));

			for (size_t i = 0; i < numLines; ++i)
			{
				if (LineLineIntersection(lines[i], second, intersection, t))
				{
					result = true;
					minT = std::min(minT, t);
				}
			}
			t = minT;
			return result;
		}

		// Check whether two circles intersect.
		// Params:
		//  circle1 = The first circle.
		//  circle2 = The second circle.
		// Returns:
		//   True if intersection, false otherwise.
		bool CircleCircleIntersection(const Circle& circle1, const Circle& circle2)
		{
			// Compare distance between centers with the sum of the radii
			// (Squared distance and sum to avoid sqrt)
			float distanceSquared = circle1.center.DistanceSquared(circle2.center);
			float radiusSumSquared = (circle1.radius + circle2.radius) * (circle1.radius + circle2.radius);

			// Return result
			return distanceSquared <= radiusSumSquared;
		}

		// Check whether a static circle and line intersect.
		// Params:
		//  line   = Start and end of first line segment.
		//  circle = Start and end of second line segment (can be moving point or circle).
		//  t = T value of intersection (for checking closest intersection).
		// Returns:
		//   True if intersection, false otherwise.
		bool StaticCircleLineIntersection(const LineSegment & line, const Circle & circle, float& t)
		{
			Vector2D d = line.start - circle.center;
			Vector2D D = line.end - line.start;
			float a = D.DotProduct(D);
			float b = d.DotProduct(D);
			float c = d.DotProduct(d) - circle.radius * circle.radius;
			float disc = b * b - a * c;

			// Circle does not intersect with infinite line
			if (disc < 0.0f) {
				return false;
			}

			// Calculate t values for intersection(s)
			float sqrtDisc = sqrtf(disc);
			float invA = 1.0f / a;
			float t0 = (-b - sqrtDisc) * invA;
			float t1 = (-b + sqrtDisc) * invA;

			// Are intersections within segment?
			bool onSegmentT0 = (t0 >= 0.0f && t0 <= 1.0f);
			bool onSegmentT1 = (t1 >= 0.0f && t1 <= 1.0f);

			// Neither point is on the line segment
			if (!onSegmentT0 && !onSegmentT1)
				return false;
			// One point is on line segment
			else if (onSegmentT0 && !onSegmentT1)
				t = t0;
			else if (onSegmentT1 && !onSegmentT0)
				t = t1;
			// Both points on line segment
			else
				t = std::min(t0, t1);

			// Intersection!
			return true;
		}

		// Check whether a moving point and line intersect.
		// Params:
		//  staticLine   = Start and end of first line segment.
		//  movingCircle = Start and end of second line segment (can be moving point or circle).
		//  radius		 = Radius of the circle. Should be 
		//  intersection = Intersection point, if any.
		// Returns:
		//   True if intersection, false otherwise.
		bool MovingCircleLineIntersection(const LineSegment & staticLine, const LineSegment & movingCircle, float radius, Vector2D & intersection, float& t)
		{
			// 1. Calculate the second object's change in translation during the current frame.
			Vector2D velocity = movingCircle.end - movingCircle.start;

			// 2. Return false if the second object is stationary or is moving parallel to the line segment.
			if (staticLine.normal.DotProduct(velocity) == 0.0f)
				return false;

			// 3-1. Figure out which side of the line the circle is on...
			bool inside = staticLine.normal.DotProduct((movingCircle.end - staticLine.start).Normalized()) > 0.0f;
			// If inside, use radius, if outside use -radius
			float distance = inside ? radius : -radius;

			// 3-2. Check for non-collision (both points of second segment on one side of the first)
			// If first.start is not between second.start and second.end (using first.normal),
			//		return false
			// (Use the PointIsBetweenLines function!)
			if (!PointIsBetweenLines(staticLine.start, movingCircle.start, movingCircle.end, staticLine.normal, distance))
				return false;

			//////////////////////////////////////////////////////////////////////////////
			//																			//
			// Now we calculate the time of intersection.								//
			// Get ready for lots of dot products!										//
			//																			//
			// FIRST LINE - NORMAL EQ													//
			// NOTE: a = normal.x, b = normal.y, p = first.start						//
			// a * x + b * y = c														//
			// c = a * p.x + b * p.y = normal dot p										//
			// Thus, normal dot xy = normal dot p										//
			//																			//
			// SECOND LINE - VECTOR EQ													//
			// xy = start + (start - end) * t											//
			// From earlier, we know that start - end = velocity						//
			// Thus, xy = start + velocity * t											//
			//																			//
			// COMBINED EQs																//
			// normal dot (start + velocity * t) = normal dot p							//
			// normal dot start + normal dot velocity * t = normal dot p				//
			//																			//
			//////////////////////////////////////////////////////////////////////////////

			// 4. Using the above information, we can easily solve for t:
			// t = (normal dot p - normal dot start) / normal dot velocity
			t = (staticLine.normal.DotProduct(staticLine.start) - staticLine.normal.DotProduct(movingCircle.start) + distance)
				/ staticLine.normal.DotProduct(velocity);

			// 5. Calculate the point of intersection.
			// intersection = start + velocity * t
			intersection = movingCircle.start + velocity * t;

			// 6. Verify intersection point is on first segment
			// If intersection point is not between first.start and first.end (using first.direction),
			//		return false
			if (!PointIsBetweenLines(intersection, staticLine.start, staticLine.end, staticLine.direction, distance))
				return false;

			// 7. Other possibilities have been eliminated, so
			// this must be an intersection. Return true!
			return true;
		}

		// Modifies object's position, velocity, and rotation using simple point-line reflection.
		// Params:
		//  transform = Transform of the object that is being reflected.
		//  physics   = Physics of the object being reflected.
		//  staticLine   = Start and end of first line segment.
		//  movingCircle = Start and end of second line segment (can be moving point or circle).
		//  radius		 = Radius of the circle. (Use 0 for points.)
		//  intersection = Intersection point of the line and circle. 
		void MovingCircleLineReflection(Transform & transform, RigidBody & physics, const LineSegment & staticLine, const LineSegment & movingCircle, float radius, const Vector2D & intersection)
		{
			// 0. Figure out which side of the line the circle is on...
			bool inside = staticLine.normal.DotProduct((movingCircle.end - staticLine.start).Normalized()) > 0.0f;
			// If inside, use radius, if outside use -radius
			float distance = inside ? radius : -radius;

			// 1. Find correct position of object by reflecting its end point over the line.
			// (Use the ReflectPointOverLine function!)
			// Once found, set the object's translation to this reflected point.
			Vector2D reflectedPosition = ReflectPointOverLine(movingCircle.end, staticLine, distance);
			transform.SetTranslation(reflectedPosition);

			// 2. Use the reflected position and intersection point to find the
			// direction of the reflected velocity. Make sure to normalize this vector!
			Vector2D reflectedDirection = (reflectedPosition - intersection).Normalized();

			// 3. Set the velocity of the object to this direction times
			// the magnitude of the current velocity.
			physics.SetVelocity(reflectedDirection * physics.GetVelocity().Magnitude());

			// 4. Find the object's new rotation by using atan2 with the reflected direction.
			// Once found, set the rotation of the object to this angle.
			float reflectedAngle = atan2f(reflectedDirection.y, reflectedDirection.x);
			transform.SetRotation(reflectedAngle);
		}

		// Check whether two line segments intersect.
		// Params:
		//  first   = Start and end of first line segment.
		//  second  = Start and end of second line segment.
		//  intersection = Intersection point, if any.
		// Returns:
		//   True if intersection, false otherwise.
		bool LineLineIntersection(const LineSegment & first, const LineSegment & second, Vector2D & intersection, float& t)
		{
			return MovingCircleLineIntersection(first, second, 0, intersection, t);
		}

		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------

		// Checks whether a point is between two parallel lines.
		// Params:
		//   point = The point in question.
		//   firstLine = A point on the first line.
		//   secondLine = A point on the second line.
		//   normal = Normal vector to the two lines.
		// Returns:
		//   True if the point is between the two lines, false otherwise.
		bool PointIsBetweenLines(const Vector2D & point, const Vector2D & firstLine, const Vector2D & secondLine, const Vector2D & normal, float distance)
		{
			//////////////////////////////////////////////////////////////////////////////
			//																			//
			// To determine whether the point is between the two lines, we'll be		//
			// comparing distances of parallel lines going through each point			//
			// to the origin.															//
			//																			//
			// Normally, distance from a point P to a line with point Q and normal n	//
			// is calculated as follows:												//
			//																			//
			//		abs(n dot P - n dot Q) / magnitude(n)								//
			//																			//
			// However, in this case, P is (0, 0), which simplifies the equation:		//
			//																			//
			//		n dot Q / magnitude(n)												//
			//																			//
			// Furthermore, N is normalized, further simplifying the equation:			//
			//																			//
			//		n dot Q																//
			//																			//
			//////////////////////////////////////////////////////////////////////////////

			// 1. Calculate distances between the line through each point and the origin, 
			// store each result in its own variable.
			// (Distance from origin to line through any point = normal dot point)
			float nDotFirst = normal.DotProduct(firstLine);
			float nDotSecond = normal.DotProduct(secondLine);
			float nDotPoint = normal.DotProduct(point);

			// 2. If the distance to the line through "point" is less than both the others,
			// it is not between them. Return false.
			if (nDotFirst - nDotPoint < distance && nDotSecond - nDotPoint < distance)
				return false;

			// 3. If the distance to the line through "point" is greater than both the others,
			// it is not between them. Return false.
			if (nDotFirst - nDotPoint > distance && nDotSecond - nDotPoint > distance)
				return false;

			// 4. All other cases eliminated, so it must be between them. Return true!
			return true;
		}

		// Reflects a point over a line.
		// Params:
		//   point = The point being reflected.
		//   line = The line the point will be reflected over.
		// Returns:
		//   The reflected point.
		Vector2D ReflectPointOverLine(const Vector2D & point, const LineSegment & line, float radius)
		{
			// 1. Pretend everything is at the origin by subtracting one of the line's 
			// points from the point we are reflecting.
			// translated point p = point - line.start
			Vector2D translated = point - line.start;

			//////////////////////////////////////////////////////////////////////////////
			//																			//
			// Equation for reflected point R is as follows, given point P,				//
			// and a line with normal n:												//
			//		R = P - 2 * projection of P onto n									//
			//																			//
			// Equation for projected point J (projection of P onto N) is as follows:	//
			//																			//
			//		J = (P dot n) / (magnitude(n) squared) * n							//
			//																			//
			// However, since our normal is normalized (magnitude = 1), this simplifies //
			// the projection (and hence the reflection) significantly:					//
			//																			//
			//		J = (P dot n) * n													//
			//																			//
			//////////////////////////////////////////////////////////////////////////////

			// 2. Calculate the reflected point and store it in a variable.
			// reflected point = P - 2 * (P dot n) * n
			Vector2D reflected = translated - (2.0f * (line.normal.DotProduct(translated) - radius)) * line.normal;

			// 3. We need to remember to move things back away from the origin before returning.
			// Return the reflected point + line.start
			return reflected + line.start;
		}
	}
}
