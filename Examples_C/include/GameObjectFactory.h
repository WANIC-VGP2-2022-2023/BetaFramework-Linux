//------------------------------------------------------------------------------
//
// File Name:	GameObjectFactory.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 4
// Course:		CS230S19
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
// Forward References:
//------------------------------------------------------------------------------

typedef struct GameObject *GameObjectPtr;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Build a single instance of the specified game object.
// (NOTE: Use sprintf_s() to construct a path name using objectName.)
// (HINT: The correct path name should be constructed using "Data/&s.txt".)
// (HINT: See project instructions.)
// Params:
//	 objectName = The name of the object to be deserialized.
// Returns:
//	 If the objectName is valid,
//	   then return a pointer to a new instance of the specified game object,
//	   else NULL.
GameObjectPtr GameObjectFactoryBuild(const char * objectName);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif

