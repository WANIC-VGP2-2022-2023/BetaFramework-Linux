//------------------------------------------------------------------------------
//
// File Name:	FrameRateController.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
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
// Public Functions:
//------------------------------------------------------------------------------

// Get the change in time from the previous frame to the current
// Use this to make sure things such as movement and physics happen at a
// consistent speed across multiple devices or framerates. It should be 
// used to set a dt variable for use in physics and other time-sensitive 
// calculations and computations.
// Return:
//   The time between the start of the previous frame and the start of the current frame.
BE_C_API float FrameRateControllerGetFrameTime();

// Gets the number of frames that have elapsed since the engine started running.
// Returns:
//   The total frames that have elapsed.
BE_C_API unsigned long FrameRateControllerGetFrameCount();

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif
