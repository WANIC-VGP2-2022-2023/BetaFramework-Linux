//------------------------------------------------------------------------------
//
// File Name:	TilePlacer.cpp
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
#include "TilePlacer.h"

// Systems
#include <Space.h>			// GetCamera
#include <Camera.h>
#include <EngineCore.h>		// GetModule
#include <EventManager.h>	// RegisterEventHandler, UnregisterEventHandler
#include <Input.h>			// CheckTriggered
#include <GraphicsEngine.h>	// ScreenToWorldPosition
#include <SpaceManager.h>	// GetSpaceByName
#include <MeshFactory.h>	// CreateQuadMesh

// Components
#include <Transform.h>
#include <SpriteTilemap.h>	// GetTilemap
#include "TilePalette.h"	// GetSelectedTile

// Resources
#include <GameObject.h>
#include <SpriteSource.h>	// GetTextureDimensions
#include <Tilemap.h>
#include "EditorLevel.h"	// MapLoad event

//------------------------------------------------------------------------------
namespace Beta
{
	namespace Editor
	{
		//------------------------------------------------------------------------------
		// Private Consts:
		//------------------------------------------------------------------------------

		namespace
		{
			const float guiClickDelay = 0.5f;
			const float mapSaveDelay = 1.0f;
			const float mapLoadDelay = 2.0f;
		}

		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		TilePlacer::TilePlacer()
			: Component("TilePlacer"), input(*EngineGetModule(Input)), delayEdit(false), map(nullptr)
		{
		}

		void TilePlacer::Initialize()
		{
			// Get the GUI space
			Space* guiSpace = EngineGetModule(SpaceManager)->GetSpaceByName("GuiSpace");

			// Register event handlers
			events = EngineGetModule(EventManager);
			events->RegisterEventHandler(
				guiSpace, "GUIClick", *this, &TilePlacer::OnGuiClick);
			events->RegisterEventHandler(
				GetOwner()->GetSpace(), "TileSet", *this, &TilePlacer::OnTileChange);
			events->RegisterEventHandler(
				GetOwner()->GetSpace(), "TileErase", *this, &TilePlacer::OnTileChange);
			events->RegisterEventHandler(
				GetOwner()->GetSpace(), "MapSave", *this, &TilePlacer::OnMapSave);
			events->RegisterEventHandler(
				GetOwner()->GetSpace(), "MapLoad", *this, &TilePlacer::OnMapLoad);
			events->RegisterEventHandler(
				GetOwner()->GetSpace(), "PaletteChange", *this, &TilePlacer::OnPaletteChange);

			// Get components
			transform = GetOwner()->GetComponent<Transform>();
			sprite = GetOwner()->GetComponent<SpriteTilemap>();
			palette = guiSpace->GetObjectManager().GetObjectByName("TilePalette")->GetComponent<TilePalette>();
		}

		void TilePlacer::Update(float dt)
		{
			// Hack to prevent accidental map edits while interacting with GUI elements
			// Will fix using proper raycasting later
			timer -= dt;
			if (timer <= 0)
				delayEdit = false;
		}

		void TilePlacer::DelayNextEdit(float time)
		{
			delayEdit = true;
			timer = time;
		}

		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------

		void TilePlacer::OnTileChange(const Event& event)
		{
			UNREFERENCED_PARAMETER(event);

			if (delayEdit)
				return;

			// CALCULATE MAP INDICES
			// Get mouse world position
			Vector2D mousePosition = GetOwner()->GetSpace()->GetCamera().ScreenToWorldPosition(input.GetCursorPosition());
			// Apply inverse map transform
			mousePosition = transform->GetInverseMatrix() * mousePosition;
			// Normalize, invert y
			int x = static_cast<int>(floorf(mousePosition.x + 0.5f));
			int y = static_cast<int>(floorf(-mousePosition.y + 0.5f));

			bool erase = event.type == "TileSet" ? false : true;

			// Tile placement
			if (palette->GetSelectedIndex() == TilePalette::invalidIndex && !erase)
				return;

			int newValue = erase ? 0 : palette->GetSelectedIndex();

			// If edit would incur resize, delay next edit to avoid continuous resize
			if (map->GetCellValue(x, y) == Tilemap::InvalidIndex())
				DelayNextEdit(0.5f);

			// TO DO: Remember previous value (for undo)

			// Don't edit if edit would result in no change
			const int& currentValue = map->GetCellValue(x, y);
			if (currentValue == newValue)
				return;

			// Don't resize if erasing tiles
			bool resize = (newValue > 0);
			map->SetCellValue(x, y, newValue, resize, true);
		}

		void TilePlacer::OnGuiClick(const Event&)
		{
			DelayNextEdit(guiClickDelay);
		}

		void TilePlacer::OnMapSave(const Event&)
		{
			map->ShrinkToFit(); // Shrinks the map to save space
			DelayNextEdit(mapSaveDelay);
		}

		void TilePlacer::OnMapLoad(const Event& event)
		{
			const MapLoad& loadEvent = static_cast<const MapLoad&>(event);
			map = loadEvent.map;
			sprite->SetTilemap(map);
			DelayNextEdit(mapLoadDelay);
		}

		// Handler for palette change
		void TilePlacer::OnPaletteChange(const Event& event)
		{
			// Set the sprite source
			const PaletteChange& paletteEvent = static_cast<const PaletteChange&>(event);
			sprite->SetSpriteSource(paletteEvent.source);

			// Create custom meshes for both objects
			const Vector2D& textureDimensions = paletteEvent.source->GetTextureDimensions();
			float u = 1.0f / textureDimensions.x;
			float v = 1.0f / textureDimensions.y;
			MeshPtr mapMesh = MeshPtr(EngineGetModule(MeshFactory)->CreateQuadMesh(Vector2D(u, v)));

			// Set meshes and add meshes to resource manager
			Sprite::GetMeshManager().Add("Map", mapMesh);
			sprite->SetMesh(mapMesh);
		}

		COMPONENT_SUBCLASS_DEFINITION(TilePlacer)
	}
}

//------------------------------------------------------------------------------
