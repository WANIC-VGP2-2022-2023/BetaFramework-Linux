//------------------------------------------------------------------------------
//
// File Name:	FrameRateController.cpp
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
#include "FrameRateController.h"

#include "EngineModulesGlobal.h"
#include "../../LowLevelAPI/include/FrameRateController.h"

//------------------------------------------------------------------------------

using namespace Beta;

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Get the change in time from the previous frame to the current
// Use this to make sure things such as movement and physics happen at a
// consistent speed across multiple devices or framerates. It should be used to set a
// dt variable for use in physics and other time-sensitive calculations and computations.
// Return:
//   The time between the start of the previous frame and the start of the current frame.
float FrameRateControllerGetFrameTime()
{
	return frameRateController->GetFrameTime();
}

// Gets the number of frames that have elapsed since the engine started running.
// Returns:
//   The total frames that have elapsed.
unsigned long FrameRateControllerGetFrameCount()
{
	return frameRateController->GetFrameCount();
}

//------------------------------------------------------------------------------
