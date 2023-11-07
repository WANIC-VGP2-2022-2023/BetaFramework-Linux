//------------------------------------------------------------------------------
//
// File Name:	Component.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		Project 7
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Component.h"

#include "GameObject.h" // dynamic cast to GameObject*

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor - needed to construct BetaObject.
	Component::Component(const std::string& name)
		: BetaObject(name)
	{
	}

	// Retrieve component's owner object (if any)
	GameObject* Component::GetOwner() const
	{
		return static_cast<GameObject*>(BetaObject::GetOwner());
	}

	// Get the space that contains this object.
	Space* Component::GetSpace() const
	{
		return GetOwner()->GetSpace();
	}
}
