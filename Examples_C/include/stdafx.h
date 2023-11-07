//------------------------------------------------------------------------------
//
// File Name:	stdafx.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		Beta Engine
// Course:		WANIC VGP2
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once

// Disable warnings for externals
#pragma warning(push, 0)

#define WIN32_LEAN_AND_MEAN // Exclude less common Windows headers
#include <Windows.h>		// Virtual key codes

#define _USE_MATH_DEFINES
#include <math.h>		// M_PI
#include <stdlib.h>
#include <stdio.h>		// printf
#include <stdbool.h>	// bool
#include <string.h>		// strncmp

// Used to track memory leaks to specific files and line numbers
#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
#endif

// Beta Engine
#include <BetaLow.h>

// Used to clean /W4 unused parameters for functions that must match a function-pointer type 
// NOTE: copied from winnt.h, but we don't want to include that here, otherwise 
#define UNREFERENCED_PARAMETER(P) (P)

// Re-enable warnings
#pragma warning(pop)
