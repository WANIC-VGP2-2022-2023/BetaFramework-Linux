//------------------------------------------------------------------------------
//
// File Name:	FrameRateController.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright � 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "FrameRateController.h"

#include "EngineCore.h" // GetModule
#include "GraphicsEngine.h" // GetUseVsync
#include "../../glfw/src/glfw3.h"
#include <unistd.h>

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Private Consts:
	//------------------------------------------------------------------------------

	const double averageFrameRate = 60.0;
	const double millisecondsPerSecond = 1000.0;

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	FrameRateController::FrameRateController()
		: currTime(0), prevTime(0), frameCount(0),
		minDeltaTime(1.0 / averageFrameRate), deltaTime(minDeltaTime), graphics(nullptr)
	{
	}

	// Sets the maximum frame rate for the frame rate controller.
	// Frame rate controller will wait until enough time has passed if an individual
	// frame does not take enough time.
	// Params:
	//   framerateCap = Maximum frames per second.
	void FrameRateController::Initialize(unsigned framerateCap)
	{
		// Set minimum dt based on provided max rate - at least 1 ms
		minDeltaTime = std::max(1.0 / static_cast<double>(framerateCap), 1.0 / millisecondsPerSecond);

		// First dt will be based on framerate cap
		deltaTime = 1.0 / framerateCap;

		// Store times
		currTime = prevTime = glfwGetTime();

		// Initialize ongoing counters
		frameCount = 0;

		graphics = EngineGetModule(GraphicsEngine);
	}

	// End the current frame, updating the delta time.
	void FrameRateController::FrameEnd()
	{
		// Frame rate limiting - wait if necessary
		currTime = glfwGetTime();
		if (!graphics->GetUseVsync() && currTime - prevTime < minDeltaTime)
		{
			usleep(((prevTime + minDeltaTime) - currTime) * millisecondsPerSecond * 10);
		}

		// Get the current system time
		currTime = glfwGetTime();

		// Calculate dt based on current and previous frame time
		deltaTime = currTime - prevTime;

		// Update ongoing counters
		frameCount++;

		// Store previous time
		prevTime = currTime;
	}

	// Gets the frame time from Beta Framework.
	// Use this to make sure things such as movement and physics happen at a
	// consistent speed across multiple devices or framerates. It should be used to set a
	// dt variable for use in physics and other time-sensitive calculations and computations.
	// Return:
	//   The time between the start of the previous frame and the start of the current frame.
	float FrameRateController::GetFrameTime() const
	{
		return static_cast<float>(deltaTime);
	}

	// Gets the number of frames that have elapsed since the engine started running.
	// Returns:
	//   The total frames that have elapsed.
	unsigned long FrameRateController::GetFrameCount() const
	{
		return frameCount;
	}
}

//------------------------------------------------------------------------------
