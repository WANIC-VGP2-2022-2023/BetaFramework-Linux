//------------------------------------------------------------------------------
//
// File Name:	WindowSystem.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		Beta Engine
// Course:		CS230
//
// Copyright � 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "WindowSystem.h"

#include "EngineModulesGlobal.h"
#include "../../LowLevelAPI/include/WindowSystem.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Set the title of the window being used by Beta Framework.
// Params:
//   text = The string that the window title will be set to.
void SystemSetWindowTitle(const char* text)
{
	Beta::windows->SetWindowTitle(text);
}

// Test whether the application is in fullscreen mode.
bool SystemIsFullScreen()
{
	return Beta::windows->IsFullScreen();
}

// Set whether the window is fullscreen.
void SystemSetFullScreen(bool fullscreen)
{
	Beta::windows->SetFullScreen(fullscreen);
}

// Set the resolution of the window.
// Params:
//   width = The new width of the window.
//   height = The new height of the window.
void SystemSetResolution(unsigned width, unsigned height)
{
	Beta::windows->SetResolution(width, height);
}

// Get the current resolution of the window.
Vector2D SystemGetResolution()
{
	Beta::Vector2D resolution = Beta::windows->GetResolution();
	return { resolution.x, resolution.y };
}

/*----------------------------------------------------------------------------*/
