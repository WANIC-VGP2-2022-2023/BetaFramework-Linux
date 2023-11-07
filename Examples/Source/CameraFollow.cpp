//------------------------------------------------------------------------------
//
// File Name:	CameraFollow.cpp
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

#include "stdafx.h"		// Precompiled header
#include "CameraFollow.h"	// Class file

// Components
#include <GameObject.h>
#include <Transform.h>

// Systems
#include <EngineCore.h>	// EngineGetModule
#include <Tilemap.h>
#include <Space.h>	// GetCamera
#include <FileStream.h>

// Math
#include <Interpolation.h>

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	// Params:
	//   tilemap = The tilemap that constrains the camera.
	CameraFollow::CameraFollow(ConstTilemapPtr tilemap)
		: Component("CameraFollow"), transform(nullptr),
		mapTransform(nullptr), tilemap(tilemap)
	{
	}

	// Initialize data for this object.
	void CameraFollow::Initialize()
	{
		transform = GetOwner()->GetComponent<Transform>();
	}

	// Update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void CameraFollow::Update(float dt)
	{
		if (mapTransform == nullptr)
		{
			GameObject* mapObject = GetOwner()->GetSpace()->
				GetObjectManager().GetObjectByName("Tilemap");
			if (mapObject != nullptr)
				mapTransform = mapObject->GetComponent<Transform>();
		}

		// Get target translation
		Vector2D targetTranslation = transform->GetTranslation();
		Camera& camera = GetOwner()->GetSpace()->GetCamera();

		// If map exists
		if (mapTransform != nullptr && tilemap != nullptr)
		{
			const BoundingRectangle& mapBounds = mapTransform->GetBounds();

			// Convert to world coordinates
			float top = mapBounds.top;
			float left = mapBounds.left;
			float bottom = mapBounds.bottom;
			float right = mapBounds.right;

			// Constrain camera edge to map
			const BoundingRectangle& screenBounds = camera.GetScreenWorldDimensions();
			left += screenBounds.extents.x;
			right -= screenBounds.extents.x;
			bottom += screenBounds.extents.y;
			top -= screenBounds.extents.y;

			// Change target
			targetTranslation.x = std::max(targetTranslation.x, left);
			targetTranslation.x = std::min(targetTranslation.x, right);
			targetTranslation.y = std::max(targetTranslation.y, bottom);
			targetTranslation.y = std::min(targetTranslation.y, top);
		}

		// Get current translation
		Vector2D currentTranslation = camera.GetTranslation();
		camera.SetTranslation(Interpolation::Interpolate(currentTranslation, targetTranslation, 3.0f * dt));
	}

	// Set the tilemap that will constrain the camera
	void CameraFollow::SetTilemap(ConstTilemapPtr tilemap_)
	{
		tilemap = tilemap_;
	}

	// Loads object data from a file.
	// Params:
	//   stream = The stream for the file we want to read from.
	void CameraFollow::Deserialize(FileStream & stream)
	{
		std::string tilemapName;
		stream.ReadVariable("tilemap", tilemapName);
		SetTilemap(ResourceGetTilemap(tilemapName));
	}

	// Saves object data to a file.
	// Params:
	//   stream = The stream for the file we want to write to.
	void CameraFollow::Serialize(FileStream & stream) const
	{
		stream.WriteVariable("tilemap", tilemap->GetName());
	}

	COMPONENT_SUBCLASS_DEFINITION(CameraFollow)
}
