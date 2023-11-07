//------------------------------------------------------------------------------
//
// File Name:	ColorInverter.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		Beta Engine
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "ColorInverter.h"

#include <ShaderProgram.h>

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	ColorInverter::ColorInverter()
		: PostEffect("colorInverter.frag"), timer(0.0f), invertTime(2.0f)
	{
	}

	// Restart timer
	void ColorInverter::Initialize()
	{
		timer = 0.0f;
	}

	// Update the timer
	void ColorInverter::Update(float dt)
	{
		timer += dt;
	}

	// Send data to OpenGL
	void ColorInverter::Draw()
	{
		GetProgram().SetUniform("timer", timer);
		GetProgram().SetUniform("invertTime", invertTime);
	}
}

//------------------------------------------------------------------------------
