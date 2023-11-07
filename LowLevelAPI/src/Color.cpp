//------------------------------------------------------------------------------
//
// File Name:	Color.cpp
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
#include "Color.h"

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	Color::Color()
		: r(1.0f), g(1.0f), b(1.0f), a(1.0f)
	{
	}

	Color::Color(float r, float g, float b, float a)
		: r(r), g(g), b(b), a(a)
	{
	}

	Color Color::operator*(const Color& other) const
	{
		return Color(r * other.r, g * other.g, b * other.b, a * other.a);
	}

	Color Color::operator+(const Color& other) const
	{
		return Color(r + other.r, g + other.g, b + other.b, a + other.a);
	}

	Color Color::operator-(const Color & other) const
	{
		return Color(r - other.r, g - other.g, b - other.b, a - other.a);
	}

	Color Color::operator*(float scalar) const
	{
		return Color(r * scalar, g * scalar, b * scalar, a * scalar);
	}

	Color Color::Clamp() const
	{
		Color temp;
		temp.r = std::clamp(r, 0.0f, 1.0f);
		temp.g = std::clamp(g, 0.0f, 1.0f);
		temp.b = std::clamp(b, 0.0f, 1.0f);
		temp.a = std::clamp(a, 0.0f, 1.0f);
		return temp;
	}

	Color operator*(float scalar, const Color & color)
	{
		return color * scalar;
	}

	// Output a color to a stream
	std::ostream& operator<<(std::ostream & os, const Color & color)
	{
		os << "{ " << color.r << " , " << color.g << " , " << color.b << " , " << color.a << " }";
		return os;
	}

	// Read a color from a stream
	std::istream& operator>>(std::istream & is, Color & color)
	{
		is.ignore(10, '{'); is >> color.r; is.ignore(10, ',');
		is >> color.g; is.ignore(10, ',');
		is >> color.b; is.ignore(10, ',');
		is >> color.a; is.ignore(10, '}');
		return is;
	}
}

//------------------------------------------------------------------------------
