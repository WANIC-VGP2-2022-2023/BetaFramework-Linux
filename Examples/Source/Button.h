//------------------------------------------------------------------------------
//
// File Name:	Button.h
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

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Forward Declarations:
	//------------------------------------------------------------------------------

	class GameObject;
	struct Event;

	//------------------------------------------------------------------------------
	// Public Function Declarations:
	//------------------------------------------------------------------------------

	class Button : public Component
	{
	public:

		// Constructor
		Button();

		// Initialize this component (happens at object creation).
		void Initialize() override;

	private:
		// Mouse over event handlers for buttons
		void OnMouseEnter(const Event& event);
		void OnMouseOver(const Event& event);
		void OnMouseLeave(const Event& event);

		// Mouse click event handlers for buttons
		void OnMouseClick(const Event& event);
		void OnMouseDown(const Event& event);
		void OnMouseUp(const Event& event);

		COMPONENT_SUBCLASS_DECLARATION(Button)
	};
}
