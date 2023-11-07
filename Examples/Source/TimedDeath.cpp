//------------------------------------------------------------------------------
//
// File Name:	TimedDeath.cpp
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
#include "TimedDeath.h"

#include <GameObject.h> // Destroy
#include <FileStream.h>		// WriteVariable, ReadVariable

//---------------------------------------------------------------------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	// Params:
	//   timeUntilDeath = Amount of time until the object self-destructs.
	TimedDeath::TimedDeath(float timeUntilDeath)
		: Component("TimedDeath"), timeUntilDeath(timeUntilDeath)
	{
	}

	// Update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void TimedDeath::Update(float dt)
	{
		// Destroy object when timer reaches 0
		timeUntilDeath -= dt;
		if (timeUntilDeath <= 0.0f)
			GetOwner()->Destroy();
	}

	// Write object data to file
	// Params:
	//   stream = The stream that is writing this object to a file.
	void TimedDeath::Serialize(FileStream & stream) const
	{
		stream.WriteVariable("timeUntilDeath", timeUntilDeath);
	}

	// Read object data from a file
	// Params:
	//   stream = The stream that is reading this object's data from a file.
	void TimedDeath::Deserialize(FileStream & stream)
	{
		stream.ReadVariable("timeUntilDeath", timeUntilDeath);
	}

	COMPONENT_SUBCLASS_DEFINITION(TimedDeath)
}

//------------------------------------------------------------------------------
