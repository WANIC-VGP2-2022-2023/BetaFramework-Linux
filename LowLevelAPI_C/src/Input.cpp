//------------------------------------------------------------------------------
//
// File Name:	Input.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		Beta Engine
// Course:		WANIC VGP2
//
// Copyright � 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Input.h"

#include "EngineModulesGlobal.h"
#include "../../LowLevelAPI/include/Input.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Test if key was down before and is still down
bool InputCheckHeld(unsigned key)
{
	return Beta::input->CheckHeld(key);
}

// Test if key was not down before but is down now
bool InputCheckTriggered(unsigned key)
{
	return Beta::input->CheckTriggered(key);
}

// Test if key was down before but is not down now
bool InputCheckReleased(unsigned key)
{
	return Beta::input->CheckReleased(key);
}

// Test if key is currently down
bool InputIsKeyDown(unsigned key)
{
	return Beta::input->IsKeyDown(key);
}

// Test if key was down a number of frames before the current frame
bool InputWasKeyDown(unsigned key, unsigned framesBefore)
{
	return Beta::input->WasKeyDown(key, framesBefore);
}

// Get the current location of the mouse in screen coordinates
Vector2D InputGetCursorPosition()
{
	Beta::Vector2D pos = Beta::input->GetCursorPosition();
	return { pos.x, pos.y };
}

// Sets the current location of the cursor this frame.
void InputSetCursorPosition(const Vector2D* position)
{
	Beta::input->SetCursorPosition(Beta::Vector2D(position->x, position->y));
}

// Get the amount that the mouse has scrolled this frame
float InputGetMouseScrollAmount()
{
	return Beta::input->GetMouseScrollAmount();
}

// Set the scroll amount that occurred this frame.
void InputSetMouseScrollAmount(float amount)
{
	Beta::input->SetMouseScrollAmount(amount);
}

// Set the number of frames to store past the current frame.
// Params:
//   size = Number of previous frames to store (should be > 1).
void InputSetBufferSize(unsigned size)
{
	Beta::input->SetBufferSize(size);
}

//------------------------------------------------------------------------------
