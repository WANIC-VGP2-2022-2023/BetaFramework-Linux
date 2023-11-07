//------------------------------------------------------------------------------
//
// File Name:	EngineModulesGlobal.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright � 2019 DigiPen (USA) Corporation.
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

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "EngineModulesGlobal.h"

#include "../../LowLevelAPI/include/EngineCore.h"

//------------------------------------------------------------------------------

namespace Beta
{
	// Standard modules - Declaration order here
	// in class specifies ideal order of initialization.
	EngineCore& engine = EngineCore::GetInstance();
	WindowSystem* windows = nullptr;
	Input* input = nullptr;
	GraphicsEngine* graphics = nullptr;
	MeshFactory* meshFactory = nullptr;
	DebugDraw* debugDraw = nullptr;
	FrameRateController* frameRateController = nullptr;
}

//------------------------------------------------------------------------------
