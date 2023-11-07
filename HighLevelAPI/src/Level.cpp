//------------------------------------------------------------------------------
//
// File Name:	Level.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		Project 7
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Level.h"

//------------------------------------------------------------------------------

#include "Space.h" // dynamic cast to Space*

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

namespace Beta
{
	// Creates a game state.
	// Params:
	//   name = Name to use for this state.
	Level::Level(const std::string& name)
		: BetaObject("Level:" + name)
	{
	}

	// Retrieve the space in which the level is running.
	Space* Level::GetSpace() const
	{
		return static_cast<Space*>(GetOwner());
	}
}
