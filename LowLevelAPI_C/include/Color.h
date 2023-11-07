//------------------------------------------------------------------------------
//
// File Name:	Color.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		Beta Engine
// Course:		WANIC VGP2
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

typedef struct Color
{
	float r;
	float g;
	float b;
	float a;
}Color;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

const Color ColorWhite = { 1,1,1,1 };
const Color ColorBlack = { 0,0,0,1 };
const Color ColorGrey = { 0.3f, 0.3f, 0.3f, 1.0f };

const Color ColorRed = { 1,0,0,1 };
const Color ColorGreen = { 0,1,0,1 };
const Color ColorBlue = { 0,0,1,1 };

const Color ColorYellow = { 1,1,0,1 };
const Color ColorViolet = { 1,0,1,1 };
const Color ColorAqua = { 0,1,1,1 };

const Color ColorOrange = { 1,0.5f,0,1 };
const Color ColorLightBlue = { 0,0.5f,1,1 };

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Return the product of two colors (multiplied component-wise).
BE_C_API Color ColorMultiply(const Color* c0, const Color* c1);

// Return the sum of two colors.
BE_C_API Color ColorAdd(const Color* c0, const Color* c1);

// Return the difference of two colors.
BE_C_API Color ColorSubtract(const Color* c0, const Color* c1);

// Return the product of a color and a scalar.
BE_C_API Color ColorScale(const Color* color, float scalar);

// Return the given color with RGBA values adjusted so they lie between 0 and 1.
BE_C_API Color ColorClamp(const Color* color);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif
