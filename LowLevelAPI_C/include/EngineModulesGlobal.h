//------------------------------------------------------------------------------
//
// File Name:	EngineModulesGlobal.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// WARNING: 
// This file exists only to facilitate implementation of wrapper.
// DO NOT DISTRIBUTE with LowLevelAPI_C lib. All other headers (apart
// from maybe stdafx.h) from this project should be fine.
//
// DO NOT INCLUDE in any headers when making changes to LowLevelAPI_C.
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Forward Declarations:
	//------------------------------------------------------------------------------

	// Windows
	class Input;
	class WindowSystem;

	// Graphics
	class DebugDraw;
	class GraphicsEngine;
	class MeshFactory;

	// Core
	class EngineCore;
	class FrameRateController;

	// Standard modules - Declaration order here
	// in class specifies ideal order of initialization.
	extern EngineCore& engine;
	extern WindowSystem* windows;
	extern Input* input;
	extern GraphicsEngine* graphics;
	extern MeshFactory* meshFactory;
	extern DebugDraw* debugDraw;
	extern FrameRateController* frameRateController;
}

//------------------------------------------------------------------------------
