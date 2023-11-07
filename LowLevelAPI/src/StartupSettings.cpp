//------------------------------------------------------------------------------
//
// File Name:	StartupSettings.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "StartupSettings.h"

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	StartupSettings::StartupSettings()
		: windowWidth(800), windowHeight(600), framerateCap(60),
		showWindow(true), debugConsole(true), fullscreen(false),
		allowMaximize(false), closeOnEscape(true), vSync(true)
	{
	}
}

//------------------------------------------------------------------------------
