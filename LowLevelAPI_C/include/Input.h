//------------------------------------------------------------------------------
//
// File Name:	Input.h
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

#include "Vector2D.h"

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Test if key was down before and is still down
BE_C_API bool InputCheckHeld(unsigned key);
// Test if key was not down before but is down now
BE_C_API bool InputCheckTriggered(unsigned key);
// Test if key was down before but is not down now
BE_C_API bool InputCheckReleased(unsigned key);

// Test if key is currently down 
BE_C_API bool InputIsKeyDown(unsigned key);
// Test if key was down a number of frames before the current frame
BE_C_API bool InputWasKeyDown(unsigned key, unsigned framesBefore);

// Get the current location of the mouse in screen coordinates
BE_C_API Vector2D InputGetCursorPosition();
// Sets the current location of the cursor this frame.
BE_C_API void InputSetCursorPosition(const Vector2D* position);

// Get the amount that the mouse has scrolled this frame
BE_C_API float InputGetMouseScrollAmount();
// Set the scroll amount that occurred this frame.
BE_C_API void InputSetMouseScrollAmount(float amount);

// Set the number of frames to store past the current frame.
// Params:
//   size = Number of previous frames to store (should be > 1).
BE_C_API void InputSetBufferSize(unsigned size);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif
