//------------------------------------------------------------------------------
//
// File Name:	ColorChange.cpp
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
#include "ColorChange.h"

// Systems
#include <Space.h>		// GetCamera
#include <Camera.h>
#include <FileStream.h>

// Components
#include <GameObject.h>
#include <Sprite.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

namespace Beta
{
	ColorChange::ColorChange(const Color& color)
		: Component("ColorChange"), sprite(nullptr), newColor(color)
	{
	}

	void ColorChange::Initialize()
	{
		// Register event handlers
		RegisterEventHandler(
			GetOwner(), "CollisionStarted", &ColorChange::OnCollisionStarted);
		RegisterEventHandler(
			GetOwner(), "CollisionEnded", &ColorChange::OnCollisionEnded);

		// Store pointer to sprite
		sprite = GetOwner()->GetComponent<Sprite>();

		// Store original color
		originalColor = sprite->GetColor();
	}

	void ColorChange::Serialize(FileStream& stream) const
	{
		stream.WriteVariable("newColor", newColor);
	}

	void ColorChange::Deserialize(FileStream& stream)
	{
		stream.ReadVariable("newColor", newColor);
	}

	void ColorChange::OnCollisionStarted(const Event& event)
	{
		UNREFERENCED_PARAMETER(event);

		sprite->SetColor(newColor);
	}

	void ColorChange::OnCollisionEnded(const Event& event)
	{
		UNREFERENCED_PARAMETER(event);

		sprite->SetColor(originalColor);
	}

	COMPONENT_SUBCLASS_DEFINITION(ColorChange)
}

//------------------------------------------------------------------------------
