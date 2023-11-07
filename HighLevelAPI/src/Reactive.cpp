//------------------------------------------------------------------------------
//
// File Name:	Reactive.cpp
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
#include "Reactive.h"

// Components
#include "GameObject.h"
#include "Transform.h"

// Math
#include "Vector2D.h"
#include "Intersection2D.h"	// PointRectangleIntersection

// Systems
#include "Input.h"	// CheckTriggered, CheckCurr
#include "Camera.h"	// CameraGetPosition
#include "Space.h"	// GetCamera
#include <EngineCore.h>	// GetModule
#include <GraphicsEngine.h>	// ScreenToWorldPosition
#include "EventManager.h"	// SendEvent

//------------------------------------------------------------------------------

namespace Beta
{
	

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Allocate a new Reactive component.
	Reactive::Reactive()
		: Component("Reactive"), mouseIsDown(false), mouseIsOver(false)
	{
	}

	// Check if the mouse is currently intersecting with this object.
	void Reactive::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		bool colliding = false;

		// Grab necessary information from object and mouse
		Transform* transform = GetOwner()->GetComponent<Transform>();

		// Check whether object is using camera
		Vector2D translation = transform->GetTranslation();

		// Perform the actual collision math for collisions between points and rectangles
		colliding = Intersection2D::PointRectangleIntersection(GetMouseWorldPosition(),
			transform->GetBounds());

		// Event manager
		EventManager* eventManager = EngineGetModule(EventManager);
		Input* input = EngineGetModule(Input);

		// If they collide, call respective handlers
		if (colliding)
		{
			// MOUSE BEGINS COLLIDING WITH OBJECT
			if (!mouseIsOver)
			{
				eventManager->SendEvent(new Event("MouseEnter"), GetOwner());
				mouseIsOver = true;
			}
			// MOUSE CONTINUES COLLIDING
			else
			{
				eventManager->SendEvent(new Event("MouseOver"), GetOwner());
			}

			// MOUSE CLICK BEGINS
			if (!mouseIsDown)
			{
				if (input->CheckTriggered(VK_LBUTTON) || input->CheckTriggered(VK_RBUTTON) || input->CheckTriggered(VK_MBUTTON))
				{
					eventManager->SendEvent(new Event("MouseClick"), GetOwner());
					mouseIsDown = true;
				}
			}
		}
		// MOUSE STOPS COLLIDING WITH OBJECT
		else if (mouseIsOver)
		{
			eventManager->SendEvent(new Event("MouseLeave"), GetOwner());
			mouseIsOver = false;
		}

		if (mouseIsDown)
		{
			// MOUSE CLICK CONTINUES
			if (input->IsKeyDown(VK_LBUTTON) || input->IsKeyDown(VK_RBUTTON) || input->IsKeyDown(VK_MBUTTON))
			{
				eventManager->SendEvent(new Event("MouseDown"), GetOwner());
			}
			// MOUSE CLICK ENDS
			else
			{
				eventManager->SendEvent(new Event("MouseUp"), GetOwner());
				mouseIsDown = false;
			}
		}
	}

	// Obtains the mouse's position in world coordinates.
	// Returns:
	//   The position of the mouse as a vector.
	Vector2D Reactive::GetMouseWorldPosition() const
	{
		Vector2D mouseScreenPosition = EngineGetModule(Input)->GetCursorPosition();
		return GetOwner()->GetSpace()->GetCamera().ScreenToWorldPosition(mouseScreenPosition);
	}

	// Save object data to file.
	// Params:
	//   stream = The stream object used to save the object's data.
	void Reactive::Serialize(FileStream& stream) const
	{
		UNREFERENCED_PARAMETER(stream);
	}

	// Load object data from file
	// Params:
	//   stream = The stream object used to load the object's data.
	void Reactive::Deserialize(FileStream& stream)
	{
		UNREFERENCED_PARAMETER(stream);
	}

	// RTTI
	COMPONENT_SUBCLASS_DEFINITION(Reactive)
}
