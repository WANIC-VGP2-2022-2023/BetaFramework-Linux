//------------------------------------------------------------------------------
//
// File Name:	Color.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		Beta Engine
// Course:		WANIC VGP2
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Color.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Return the product of two colors (multiplied component-wise).
Color ColorMultiply(const Color* c0, const Color* c1)
{
	Color product;
	product.r = c0->r * c1->r;
	product.g = c0->g * c1->g;
	product.b = c0->b * c1->b;
	product.a = c0->a * c1->a;
	return product;
}

// Return the sum of two colors.
Color ColorAdd(const Color* c0, const Color* c1)
{
	Color sum;
	sum.r = c0->r + c1->r;
	sum.g = c0->g + c1->g;
	sum.b = c0->b + c1->b;
	sum.a = c0->a + c1->a;
	return sum;
}

// Return the difference of two colors.
Color ColorSubtract(const Color* c0, const Color* c1)
{
	Color difference;
	difference.r = c0->r - c1->r;
	difference.g = c0->g - c1->g;
	difference.b = c0->b - c1->b;
	difference.a = c0->a - c1->a;
	return difference;
}

// Return the product of a color and a scalar.
Color ColorScale(const Color* color, float scalar)
{
	Color sum;
	sum.r = color->r * scalar;
	sum.g = color->g * scalar;
	sum.b = color->b * scalar;
	sum.a = color->a * scalar;
	return sum;
}

// Return the given color with RGBA values adjusted so they lie between 0 and 1.
Color ColorClamp(const Color* color)
{
	Color temp;
	temp.r = std::clamp(color->r, 0.0f, 1.0f);
	temp.g = std::clamp(color->g, 0.0f, 1.0f);
	temp.b = std::clamp(color->b, 0.0f, 1.0f);
	temp.a = std::clamp(color->a, 0.0f, 1.0f);
	return temp;
}

//------------------------------------------------------------------------------
