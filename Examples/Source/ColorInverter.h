//------------------------------------------------------------------------------
//
// File Name:	ColorInverter.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		Beta Engine
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include <PostEffect.h>

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Structures:
	//------------------------------------------------------------------------------

	class ColorInverter : public PostEffect
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		ColorInverter();

		// Restart timer
		void Initialize() override;

		// Update the timer
		void Update(float dt) override;

		// Send data to OpenGL
		void Draw() override;

	private:
		//------------------------------------------------------------------------------
		// Private Data:
		//------------------------------------------------------------------------------

		// Time elapsed since effect was initialized.
		float timer;

		// Time it takes to change from unaltered to inverted (or vice versa).
		const float invertTime;
	};
}

//------------------------------------------------------------------------------
