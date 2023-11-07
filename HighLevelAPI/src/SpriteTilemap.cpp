//------------------------------------------------------------------------------
//
// File Name:	SpriteTilemap.cpp
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
#include "SpriteTilemap.h"

// Components
#include "Transform.h"	// GetMatrix
#include "GameObject.h"	// GetComponent
#include "Area.h"		// SetSize, SetOffset

// Systems
#include <EngineCore.h>	// EngineGetModule
#include <FileStream.h> // Read/Write Variable
#include <Space.h>	// GetResourceManager

// Resources
#include "Tilemap.h"	// GetWidth, GetHeight, GetCellValue

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Create a new sprite object.
	SpriteTilemap::SpriteTilemap()
		: map(nullptr)
	{
		SetName("SpriteTilemap");
	}

	void SpriteTilemap::Initialize()
	{
		Sprite::Initialize();
		area = GetOwner()->GetComponent<Area>();
	}

	void SpriteTilemap::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		if (!area || !map)
			return;

		Vector2D size(static_cast<float>(map->GetWidth()),
			static_cast<float>(map->GetHeight()));

		Vector2D offset;
		offset.x += transform->GetScale().x * (size.x - 1) / 2.0f;
		offset.y -= transform->GetScale().y * (size.y - 1) / 2.0f;
		offset.x += transform->GetScale().x * map->GetMinIndexX();
		offset.y -= transform->GetScale().y * map->GetMinIndexY();

		area->SetSize(size);
		area->SetOffset(offset);
		area->SetZDepth(GetZDepth());
	}

	// Draw a sprite (Sprite can be textured or untextured).
	void SpriteTilemap::Draw()
	{
		if (map == nullptr)
			return;

		// Set up variables
		int minX = map->GetMinIndexX();
		int maxX = map->GetMaxIndexX();
		int minY = map->GetMinIndexY();
		int maxY = map->GetMaxIndexY();
		Vector2D offset;
		int cellValue = 0;
		const Vector2D & tileSize = transform->GetScale();

		// Loop through cells in map
		for (int r = minY; r < maxY; ++r)
		{
			for (int c = minX; c < maxX; ++c)
			{
				// Set frame index based on cell data
				cellValue = map->GetCellValue(c, r);

				// Skip empty cells
				if (cellValue < 1)
					continue;

				// Set frame index to cell value
				SetFrame(cellValue - 1);

				// Set offset based on cell location and scale
				offset.x = c * tileSize.x;
				offset.y = r * -tileSize.y;

				// Draw the sprite
				Sprite::Draw(offset);
			}
		}
	}

	// Sets the tilemap data that will be used by the sprite.
	// Params:
	//   map = A pointer to the tilemap resource.
	void SpriteTilemap::SetTilemap(ConstTilemapPtr map_)
	{
		map = map_;
	}

	// Loads object data from a file.
	// Params:
	//   stream = The stream for the file we want to read from.
	void SpriteTilemap::Deserialize(FileStream & stream)
	{
		Sprite::Deserialize(stream);
		std::string tilemapName;
		stream.ReadVariable("tilemap", tilemapName);
		SetTilemap(ResourceGetTilemap(tilemapName));
	}

	// Saves object data to a file.
	// Params:
	//   stream = The stream for the file we want to write to.
	void SpriteTilemap::Serialize(FileStream & stream) const
	{
		Sprite::Serialize(stream);
		stream.WriteVariable("tilemap", map->GetName());
	}

	COMPONENT_SUBCLASS_SUBCLASS_DEFINITION(Sprite, SpriteTilemap)
}
