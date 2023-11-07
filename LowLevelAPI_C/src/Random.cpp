//------------------------------------------------------------------------------
//
// File Name:	Random.h
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
#include "Random.h"

#include <time.h>  // time

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Initialize the random number generator (RNG).
void RandomInit()
{
	srand((unsigned)time(nullptr));
}

// Generate a random number in the interval [rangeMin, rangeMax] (inclusive).
// Params:
//	 rangeMin = The lowest possible number generated.
//	 rangeMax = The highest possible number generated.
// Returns:
//   A random number in the interval [rangeMin, rangeMax].
int RandomRangeInt(int rangeMin, int rangeMax)
{
	return rand() % (rangeMax - rangeMin + 1) + rangeMin;
}

// Generate a random floating-point number in the interval [rangeMin, rangeMax] (inclusive).
// Params:
//	 rangeMin = The lowest possible number generated.
//	 rangeMax = The highest possible number generated.
float RandomRangeFloat(float rangeMin, float rangeMax)
{
	int randInt = rand();
	float percentage = (float)randInt / (float)RAND_MAX;
	return percentage * (rangeMax - rangeMin) + rangeMin;
}

//------------------------------------------------------------------------------
