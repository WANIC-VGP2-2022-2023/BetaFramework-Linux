//------------------------------------------------------------------------------
//
// File Name:	Input.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		Beta Engine
// Course:		WANIC VGP2
//
// Copyright � 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "../include/stdafx.h"
#include "../include/Input.h"

#include "../include/Vector2D.h"
#include "WindowSystem.h"

#include <algorithm>
#include <cassert>
#include "../../glfw/src/glfw3.h"

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Private Function Declarations:
	//------------------------------------------------------------------------------

	void MouseScrollCallback(GLFWwindow * window, double xoffset, double yoffset);

	//------------------------------------------------------------------------------
	// Private Data:
	//------------------------------------------------------------------------------

	// Keys data from current and previous frames.
	const unsigned Input::inputKeyNum = 256;

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	Input::Input()
		: cursorDeltaX(0), cursorDeltaY(0), cursorPosX(0), cursorPosY(0)
	{
		// Default number of frames to store before the current is 1
		inputKeyBuffer.Resize(2);
		size_t bufferSize = inputKeyBuffer.Size();
		for (size_t i = 0; i < bufferSize; ++i)
		{
			inputKeyBuffer[i].Resize(inputKeyNum);
		}

		// Mouse
		cursorPosX = 0;
		cursorPosY = 0;
		cursorDeltaX = 0;
		cursorDeltaY = 0;

		scrollAmountPrevious = 0.0f;
		scrollAmountCurrent = 0.0f;

		// Initialize keys to false
		inputKeyBuffer[0].Fill(false);
	}

	// Update keyboard and cursor data
	void Input::Update()
	{
		UpdateKeyboard();
		UpdateMouse();
	}

	bool Input::CheckHeld(unsigned key) const
	{
		// True if key was down last frame and also in the current frame
		return WasKeyDown(key) && IsKeyDown(key);
	}

	bool Input::CheckTriggered(unsigned key) const
	{
		// True if key was not down last frame but is down in this one
		return IsKeyDown(key) && !WasKeyDown(key);
	}

	bool Input::CheckReleased(unsigned key) const
	{
		// True if key was down last frame but is not down in this one
		return WasKeyDown(key) && !IsKeyDown(key);
	}

	bool Input::IsKeyDown(unsigned key) const
	{
		return (inputKeyBuffer[0][key] & 0x80) != 0;
	}

	bool Input::WasKeyDown(unsigned key, unsigned framesBefore) const
	{
	#if _DEBUG
		assert(framesBefore < inputKeyBuffer.Size());
	#endif
		return (inputKeyBuffer[framesBefore][key] & 0x80) != 0;
	}

	Vector2D Input::GetCursorPosition() const
	{
		return Vector2D((float)cursorPosX, (float)cursorPosY);
	}

	void Input::SetCursorPosition(const Vector2D & position)
	{
		mousePosition = position;
	}

	float Input::GetMouseScrollAmount() const
	{
		return scrollAmountPrevious;
	}

	void Input::SetMouseScrollAmount(float amount)
	{
		scrollAmountCurrent = amount;
	}

	void Input::SetBufferSize(unsigned size)
	{
		assert(size >= 2);
		size_t previousSize = inputKeyBuffer.Size();
		inputKeyBuffer.Resize(size);
		if (size > previousSize)
		{
			// Make sure key state arrays are of appropriate size
			for (size_t i = previousSize; i < size; ++i)
				inputKeyBuffer[i].Resize(inputKeyNum);
		}
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Helper functions
	void Input::UpdateKeyboard()
	{
		// Shift input data
		int bufferSize = static_cast<int>(inputKeyBuffer.Size());
		for (int i = bufferSize - 2; i >= 0; --i)
		{
			auto source = inputKeyBuffer[i].Begin();
			auto end = inputKeyBuffer[i].End();
			auto destination = inputKeyBuffer[i + 1].Begin();
			while(source != end)
			{
				(*destination) = (*source);
				++source;
				++destination;
			}
		}

		// Get current keyboard state for this frame
        //TODO: REPLACE THIS WITH LINUX THING
		//GetKeyboardState(inputKeyBuffer[0].Data());
	}

	void Input::UpdateMouse()
	{
		long newXLong = static_cast<long>(mousePosition.x);
		long newYLong = static_cast<long>(mousePosition.y);

		cursorDeltaX = newXLong - cursorPosX;
		cursorDeltaY = newYLong - cursorPosY;
		cursorPosX = newXLong;
		cursorPosY = newYLong;

		scrollAmountPrevious = scrollAmountCurrent;
		scrollAmountCurrent = 0.0f;
	}
}

//------------------------------------------------------------------------------
