//------------------------------------------------------------------------------
//
// File Name:	ChangeLevel.h
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
#include "ChangeLevel.h"

// Systems
#include <EngineCore.h> // GetModule
#include <Space.h>
#include <Level.h>
#include <Input.h>

// Levels
#include "Level1.h"
#include "Level2.h"
#include "Asteroids.h"
#include "Omega.h"
#include "Test.h"
#include "Platformer.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

namespace Beta
{
	// Change to a different level from the current one
	// based on which key is pressed.
	// Params:
	//   currentLevel = The currently loaded level.
	void ChangeLevel(Level* currentLevel)
	{
		Space* space = currentLevel->GetSpace();

		if (EngineGetModule(Input)->CheckTriggered('1'))
			space->SetLevel<Level1>();

		else if (EngineGetModule(Input)->CheckTriggered('2'))
			space->SetLevel<Level2>();

		else if (EngineGetModule(Input)->CheckTriggered('3'))
			space->SetLevel<Asteroids>();

		else if (EngineGetModule(Input)->CheckTriggered('4'))
			space->SetLevel<Platformer>();

		else if (EngineGetModule(Input)->CheckTriggered('5'))
			space->SetLevel<Omega>();

		else if (EngineGetModule(Input)->CheckTriggered('6'))
			space->SetLevel<Test>();
	}
}

//------------------------------------------------------------------------------
