//------------------------------------------------------------------------------
//
// File Name:	ColorChange.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Component.h"

#include <Color.h>

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Consts:
	//------------------------------------------------------------------------------

	class GameObject;
	struct Event;
	class Sprite;

	//------------------------------------------------------------------------------
	// Public Function Declarations:
	//------------------------------------------------------------------------------

	class ColorChange : public Component
	{
	public:

		// Constructor
		ColorChange(const Color& color = Colors::Red);

		// Initialize this component (happens at object creation).
		void Initialize() override;

		// Write object data to file
		// Params:
		//   stream = The stream that is writing this object to a file.
		void Serialize(FileStream& stream) const override;

		// Read object data from a file
		// Params:
		//   stream = The stream that is reading this object's data from a file.
		void Deserialize(FileStream& stream) override;

	private:
		// Collision event handlers for ColorChanges
		void OnCollisionStarted(const Event& event);
		void OnCollisionEnded(const Event& event);

		Sprite* sprite;
		Color originalColor;
		Color newColor;

		COMPONENT_SUBCLASS_DECLARATION(ColorChange)
	};
}
