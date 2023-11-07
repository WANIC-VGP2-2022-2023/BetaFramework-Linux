//------------------------------------------------------------------------------
//
// File Name:	GridDraw.cpp
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
#include "GridDraw.h"

// Systems
#include <GraphicsEngine.h>	// GetScreenWorldDimensions
#include <DebugDraw.h>		// AddLineStrip
#include <EngineCore.h>		// GetModule
#include <Space.h>			// GetCamera

// Components
#include <GameObject.h>
#include <Transform.h>		// GetInverseMatrix

// Resources
#include <Tilemap.h>

// Levels
#include "EditorLevel.h"	// MapLoad event

//------------------------------------------------------------------------------

namespace Beta
{
	namespace Editor
	{
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		GridDraw::GridDraw()
			: Component("GridDraw"), debugDraw(*EngineGetModule(DebugDraw)), events(nullptr)
		{
		}

		void GridDraw::Initialize()
		{
			transform = GetOwner()->GetComponent<Transform>();

			// Register event handlers
			events = EngineGetModule(EventManager);
			events->RegisterEventHandler(
				GetOwner()->GetSpace(), "MapLoad", *this, &GridDraw::OnMapLoad);

			debugDraw.SetEnabled(true);
		}

		void GridDraw::Update(float)
		{
			if (map == nullptr)
				return;

			columns = map->GetWidth();
			rows = map->GetHeight();
		}

		void GridDraw::Draw()
		{
			if (map == nullptr)
				return;

			const Vector2D& position = transform->GetTranslation();
			const Color lineColor = Color(1, 1, 1, 0.5f);

			// Set up variables
			int minX = map->GetMinIndexX();
			int maxX = map->GetMaxIndexX();
			int minY = map->GetMinIndexY();
			int maxY = map->GetMaxIndexY();
			Vector2D offset;
			const Vector2D& tileSize = transform->GetScale();
			debugDraw.SetCamera(GetOwner()->GetSpace()->GetCamera());

			// Vertical lines
			float yBoundMin = -minY * tileSize.y + position.y + tileSize.y * 0.5f;
			float yBoundMax = -maxY * tileSize.y + position.y + tileSize.y * 0.5f;
			for (int c = minX; c < maxX - 1; ++c)
			{
				// Set offset based on cell location and scale
				offset.x = c * tileSize.x + position.x + tileSize.x * 0.5f;
				debugDraw.AddLineToList(Vector2D(offset.x, yBoundMin), Vector2D(offset.x, yBoundMax), lineColor);
			}
			debugDraw.EndLineList();

			// Horizontal lines
			float xBoundMin = minX * tileSize.x + position.x - tileSize.x * 0.5f;
			float xBoundMax = maxX * tileSize.x + position.x - tileSize.x * 0.5f;
			for (int r = minY + 1; r < maxY; ++r)
			{
				// Set offset based on cell location and scale
				offset.y = -r * tileSize.y + position.y + tileSize.y * 0.5f;
				debugDraw.AddLineToList(Vector2D(xBoundMin, offset.y), Vector2D(xBoundMax, offset.y), lineColor);
			}
			debugDraw.EndLineList();
		}

		void GridDraw::OnMapLoad(const Event& event)
		{
			const MapLoad& loadEvent = static_cast<const MapLoad&>(event);
			map = loadEvent.map;
		}

		COMPONENT_SUBCLASS_DEFINITION(GridDraw)
	}
}

//------------------------------------------------------------------------------
