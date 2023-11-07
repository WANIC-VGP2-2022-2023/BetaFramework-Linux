//------------------------------------------------------------------------------
//
// File Name:	CameraController.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework Map Editor
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "CameraController.h"

// Components
#include <Transform.h>
#include <Sprite.h>

// Systems
#include <GameObject.h>
#include <Camera.h>
#include <Space.h>
#include <Input.h>

// Levels
#include "EditorLevel.h" // MapLoad event

// Resources
#include <Tilemap.h>

// Math
#include <Interpolation.h>

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Structures:
	//------------------------------------------------------------------------------

	namespace Editor
	{
		namespace
		{
			const float scrollZoomModifier = 15.0f;
		}

		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		CameraController::CameraController()
			: Component("CameraController"), moveSpeed(5.0f), zoomSpeed(20.0f),
			myTransform(nullptr), camera(nullptr), input(*EngineGetModule(Input)),
			mapTransform(nullptr), mapSprite(nullptr), mouseHeld(false)
		{
		}

		// Initialize this component (happens at object creation).
		void CameraController::Initialize()
		{
			myTransform = GetOwner()->GetComponent<Transform>();
			camera = &GetOwner()->GetSpace()->GetCamera();
			myTransform->SetTranslation(camera->GetTranslation());
			targetFov = camera->GetFOV();

			// Get map object from object manager
			GameObject* mapObject = GetOwner()->GetSpace()->GetObjectManager().GetObjectByName("Map");
			if (mapObject != nullptr)
			{
				mapTransform = mapObject->GetComponent<Transform>();
				mapSprite = mapObject->GetComponent<Sprite>();
			}

			// Register for camera event
			RegisterEventHandler(
				GetOwner()->GetSpace(),
				"MapLoad", &CameraController::OnMapLoad);
			RegisterEventHandler(
				GetOwner()->GetSpace(),
				"CameraReset", &CameraController::OnReset);

			RegisterEventHandler(
				mapObject, "MouseClick", &CameraController::OnMouseClick);

			RegisterEventHandler(
				mapObject, "MouseUp", &CameraController::OnMouseUp);
		}

		// Update data associated with this component.
		void CameraController::Update(float dt)
		{
			HandleInput(dt);

			// Interpolate position
			camera->SetTranslation(Interpolation::Interpolate(camera->GetTranslation(), myTransform->GetTranslation(), 4.0f * dt));

			// Interpolate FOV
			camera->SetFOV(Interpolation::Lerp(camera->GetFOV(), targetFov, 4.0f * dt));
		}

		void CameraController::SetPanSpeed(float value)
		{
			moveSpeed = value;
		}

		void CameraController::SetZoomSpeed(float value)
		{
			zoomSpeed = value;
		}

		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------

		void CameraController::OnMapLoad(const Event& event)
		{
			const MapLoad& loadEvent = static_cast<const MapLoad&>(event);
			map = loadEvent.map;
		}

		void CameraController::OnReset(const Event&)
		{
			// Force area set
			if(mapSprite != nullptr)
				mapSprite->Update(0.0f);

			if (mapTransform != nullptr)
			{
				Vector2D mapCenter = mapTransform->GetBounds().center;
				myTransform->SetTranslation(mapCenter);
			}
		}

		void CameraController::OnMouseClick(const Event& event)
		{
			UNREFERENCED_PARAMETER(event);

			if (!input.IsKeyDown(VK_MBUTTON))
				return;

			mouseStart = GetOwner()->GetSpace()->GetCamera().ScreenToWorldPosition(input.GetCursorPosition());
			mouseHeld = true;
		}

		void CameraController::OnMouseUp(const Event& event)
		{
			UNREFERENCED_PARAMETER(event);
			mouseHeld = false;
		}

		// Mouse click event handlers for buttons
		void CameraController::HandleInput(float dt)
		{
			Vector2D cameraPos = myTransform->GetTranslation();

			// Ignore inputs when control is held
			if (input.CheckHeld(VK_CONTROL))
				return;

			// Panning (middle mouse)
			if (mouseHeld)
			{
				mouseCurrent = GetOwner()->GetSpace()->GetCamera().ScreenToWorldPosition(input.GetCursorPosition());
				Vector2D mouseDelta = mouseStart - mouseCurrent;
				if (mouseDelta.MagnitudeSquared() > 3.0f)
					cameraPos += (mouseStart - mouseCurrent).Normalized() * moveSpeed * dt;
				else if (mouseDelta.MagnitudeSquared() == 0.0f)
					mouseStart = mouseCurrent;
			}
			else
			{
				// Panning (keyboard)
				if (input.CheckHeld('W'))
					cameraPos.y += moveSpeed * dt;

				if (input.CheckHeld('S'))
					cameraPos.y -= moveSpeed * dt;

				if (input.CheckHeld('D'))
					cameraPos.x += moveSpeed * dt;

				if (input.CheckHeld('A'))
					cameraPos.x -= moveSpeed * dt;
			}

			myTransform->SetTranslation(cameraPos);

			// Zoom out
			if (input.CheckHeld('E'))
				targetFov += zoomSpeed * dt;

			// Zoom in
			if (input.CheckHeld('Q'))
				targetFov -= zoomSpeed * dt;

			// Scroll
			targetFov -= input.GetMouseScrollAmount() * (zoomSpeed * scrollZoomModifier) * dt;
		}

		COMPONENT_SUBCLASS_DEFINITION(CameraController)
	}
}

//------------------------------------------------------------------------------
