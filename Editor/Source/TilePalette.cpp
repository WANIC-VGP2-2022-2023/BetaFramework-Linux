//------------------------------------------------------------------------------
//
// File Name:	TilePalette.cpp
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
#include "TilePalette.h"

// Systems
#include <Space.h>			// GetCamera
#include <Camera.h>
#include <Input.h>			// CheckTriggered
#include <SpaceManager.h>	// GetSpaceByName
#include <MeshFactory.h>	// CreateQuadMesh
#include <GraphicsEngine.h>	// GetScreenWorldDimensions

// Components
#include <GameObject.h>
#include <Reactive.h>		// GetMouseWorldPosition
#include <Sprite.h>			// GetSpriteSource
#include <SpriteSource.h>	// GetTextureDimensions
#include <Transform.h>		// GetInverseMatrix

#include "EditorLevel.h"	// PaletteChange

//------------------------------------------------------------------------------

namespace Beta
{
	namespace Editor
	{
		const int TilePalette::invalidIndex = -1;
		const float TilePalette::paletteTileSize = 0.25f;

		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		TilePalette::TilePalette()
			: Component("TilePalette"), selectedTileIndex(invalidIndex), visible(true), 
			reactive(nullptr), transform(nullptr), sprite(nullptr), events(nullptr)
		{
		}

		void TilePalette::Initialize()
		{
			// Register event handlers
			events = EngineGetModule(EventManager);
			RegisterEventHandler(
				GetOwner(), "MouseClick", &TilePalette::OnMouseClick);
			RegisterEventHandler(
				EngineGetModule(SpaceManager)->GetSpaceByName("GameSpace"),
				"PaletteChange", &TilePalette::OnPaletteChange);

			// Get components
			sprite = GetOwner()->GetComponent<Sprite>();
			//sprite->SetZDepth(1.0f);
			reactive = GetOwner()->GetComponent<Reactive>();
			transform = GetOwner()->GetComponent<Transform>();
		}

		void TilePalette::Update(float)
		{
			float paletteYMove = 0.0f;

			// Show/hide tile palette
			Input& input = *EngineGetModule(Input);
			if (input.CheckTriggered('H'))
			{
				ToggleVisibility();
			}
			// Move palette
			else if (input.CheckTriggered(VK_DOWN))
			{
				paletteYMove = 1;
			}
			else if (input.CheckTriggered(VK_UP))
			{
				paletteYMove = -1;
			}

			// Nudge palette up/down by 1 tile
			Vector2D newTranslation = transform->GetTranslation();
			ConstSpriteSourcePtr source = sprite->GetSpriteSource();
			if(source != nullptr)
				newTranslation.y += paletteYMove * transform->GetScale().y / source->GetTextureDimensions().y;
			transform->SetTranslation(newTranslation);
		}

		int TilePalette::GetSelectedIndex() const
		{
			return selectedTileIndex;
		}

		void TilePalette::ClearSelection()
		{
			selectedTileIndex = invalidIndex;
		}

		// Toggles the visibility/interactibility of the tile palette
		void TilePalette::ToggleVisibility()
		{
			float alpha = sprite->GetAlpha();
			sprite->SetAlpha(alpha == 1.0f ? 0.0f : 1.0f);
			visible = !visible;
		}

		// Handler for palette change
		void TilePalette::OnPaletteChange(const Event& event)
		{
			//UNREFERENCED_PARAMETER(event);
			const PaletteChange& paletteEvent = static_cast<const PaletteChange&>(event);
			sprite->SetSpriteSource(paletteEvent.source);

			// Scale palette to fit all frames in sprite sheet
			const Vector2D& textureDimensions = paletteEvent.source->GetTextureDimensions();
			transform->SetScale(paletteTileSize * textureDimensions);

			// Position palette so it hugs the top right of the window
			const BoundingRectangle& screenDimensions = GetOwner()->GetSpace()->GetCamera().GetScreenWorldDimensions();
			Vector2D translation(screenDimensions.right, screenDimensions.top);
			translation -= transform->GetScale() * 0.5f;
			transform->SetTranslation(translation);
		}

		void TilePalette::OnMouseClick(const Event& event)
		{
			UNREFERENCED_PARAMETER(event);

			// Ignore if not visible
			if (!visible)
				return;

			// Figure out where on object the click happened
			Vector2D framePosition = reactive->GetMouseWorldPosition(); // World Position
			framePosition = transform->GetInverseMatrix() * framePosition + Vector2D(0.5f, 0.5f); // Normalized, 0 to 1

			// Transform to texture indices (0 to columns, 0 to rows)
			const Vector2D& textureDimensions = sprite->GetSpriteSource()->GetTextureDimensions();
			int column = static_cast<int>(textureDimensions.x * framePosition.x);
			int row = static_cast<int>(textureDimensions.y * (1.0f - framePosition.y));

			// Select tile based on indices
			selectedTileIndex = column + row * static_cast<int>(textureDimensions.x) + 1;

			// Send GUI click event
			events->SendEvent(new Event("GUIClick"), GetOwner()->GetSpace());
		}

		COMPONENT_SUBCLASS_DEFINITION(TilePalette)
	}
}

//------------------------------------------------------------------------------
