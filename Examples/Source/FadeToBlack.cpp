//------------------------------------------------------------------------------
//
// File Name:	FadeToBlack.h
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

#include <stdafx.h>
#include "FadeToBlack.h"

// Systems
#include <ShaderProgram.h>

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	FadeToBlack::FadeToBlack()
		: PostEffect("fadeToBlack.frag"), fadeTime(5.0f)
	{
		Initialize();
	}

	void FadeToBlack::Initialize()
	{
		timer = fadeTime;
	}

	void FadeToBlack::Update(float dt)
	{
		timer -= dt;
	}

	void FadeToBlack::Draw()
	{
		GetProgram().SetUniform("timer", timer);
		GetProgram().SetUniform("effectDuration", fadeTime);
	}
}

//------------------------------------------------------------------------------
