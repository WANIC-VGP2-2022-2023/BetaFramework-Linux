//------------------------------------------------------------------------------
//
// File Name:	Utility.cpp
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

#include "Shapes2D.h"

//------------------------------------------------------------------------------

namespace Beta
{
	namespace Interpolation
	{
		//------------------------------------------------------------------------------
		// Private Function Declarations:
		//------------------------------------------------------------------------------

		template <typename T>
		T InterpolateLinear(const T& start, const T& end, float percent);

		template <typename T>
		T InterpolateSpherical(const T& start, const T& end, float percent);

		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		float Lerp(float start, float end, float percent)
		{
			return InterpolateLinear(start, end, percent);
		}

		Color Lerp(const Color& start, const Color& end, float percent)
		{
			return InterpolateLinear(start, end, percent);
		}

		Vector2D Interpolate(const Vector2D& start, const Vector2D& end, float percent, InterpolationMode mode)
		{
			Vector2D result = start;

			switch (mode)
			{
			case IM_Linear:
				result = InterpolateLinear(start, end, percent);
				break;
			case IM_LinearNormalized:
				result = InterpolateLinear(start, end, percent);
				result = result.Normalized();
				break;
			case IM_Spherical:
				result = InterpolateSpherical(start, end, percent);
				break;
			default:
				break;
			}

			return result;
		}

		//------------------------------------------------------------------------------
		// Private Function Definitions:
		//------------------------------------------------------------------------------

		template <typename T>
		T InterpolateLinear(const T& start, const T& end, float percent)
		{
			return (start + percent * (end - start));
		}

		template<typename T>
		T InterpolateSpherical(const T & start, const T & end, float percent)
		{
			// Dot product - the cosine of the angle between 2 vectors.
			float dot = start.DotProduct(end);

			// Clamp it to be in the range of Acos()
			// This may be unnecessary, but floating point
			// precision can be a fickle mistress.
			dot = std::clamp(dot, -1.0f, 1.0f);

			// Acos(dot) returns the angle between start and end,
			// And multiplying that by percent returns the angle between
			// start and the final result.
			float theta = acosf(dot) * percent;
			T relativeVec = end - start * dot;

			// Orthonormal basis
			relativeVec = relativeVec.Normalized();
			// The final result.
			return start * cosf(theta) + relativeVec * sinf(theta);
		}
	}
}

//------------------------------------------------------------------------------
