//------------------------------------------------------------------------------
//
// File Name:	Button.cpp
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
#include "Button.h"

// Systems
#include <Space.h>			// GetCamera
#include <Camera.h>

// Components
#include <GameObject.h>
#include <Reactive.h>		// GetMouseWorldPosition
#include <Transform.h>		// SetTranslation

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

namespace Beta
{
	Button::Button()
		: Component("Button")
	{
	}

	void Button::Initialize()
	{
		// Register event handlers
		RegisterEventHandler(
			GetOwner(), "MouseEnter", &Button::OnMouseEnter);
		RegisterEventHandler(
			GetOwner(), "MouseOver", &Button::OnMouseOver);
		RegisterEventHandler(
			GetOwner(), "MouseLeave", &Button::OnMouseLeave);
		RegisterEventHandler(
			GetOwner(), "MouseClick", &Button::OnMouseClick);
		RegisterEventHandler(
			GetOwner(), "MouseDown", &Button::OnMouseDown);
		RegisterEventHandler(
			GetOwner(), "MouseUp", &Button::OnMouseUp);
	}

	void Button::OnMouseEnter(const Event& event)
	{
		UNREFERENCED_PARAMETER(event);

		std::cout << "Mouse entered object" << std::endl;
	}

	void Button::OnMouseOver(const Event& event)
	{
		UNREFERENCED_PARAMETER(event);
	}

	void Button::OnMouseLeave(const Event& event)
	{
		UNREFERENCED_PARAMETER(event);

		std::cout << "Mouse left object" << std::endl;
	}

	void Button::OnMouseClick(const Event& event)
	{
		UNREFERENCED_PARAMETER(event);

		std::cout << "Button clicked on object" << std::endl;
	}

	void Button::OnMouseDown(const Event& event)
	{
		UNREFERENCED_PARAMETER(event);

		// New translation will be mouse translation - click and drag to change position!
		Vector2D translation = GetOwner()->GetComponent<Reactive>()->GetMouseWorldPosition();

		// Check whether object is using camera, adjust position accordingly
		Transform* transform = GetOwner()->GetComponent<Transform>();

		// Set new position of object
		transform->SetTranslation(translation);
	}

	void Button::OnMouseUp(const Event& event)
	{
		UNREFERENCED_PARAMETER(event);

		std::cout << "Button up on object" << std::endl;
	}

	COMPONENT_SUBCLASS_DEFINITION(Button)
}

//------------------------------------------------------------------------------
