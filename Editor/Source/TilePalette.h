//------------------------------------------------------------------------------
//
// File Name:	TilePalette.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework Map Editor
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include <Component.h>

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Forward Declarations:
	//------------------------------------------------------------------------------

	struct Event;
	class EventManager;
	class Reactive;
	class Sprite;
	class Transform;

	//------------------------------------------------------------------------------
	// Public Structures:
	//------------------------------------------------------------------------------

	namespace Editor
	{
		class TilePalette : public Component
		{
		public:
			//------------------------------------------------------------------------------
			// Public Functions:
			//------------------------------------------------------------------------------

			// Constructor
			TilePalette();

			// Initialize this component (happens at object creation).
			void Initialize() override;

			// Update logic for this component.
			void Update(float) override;

			// Return the index of the currently selected tile.
			int GetSelectedIndex() const;

			// Resets the selection to invalid/none selected.
			void ClearSelection();

			// Toggles the visibility/interactibility of the tile palette
			void ToggleVisibility();

			// The value returned by GetSelectedIndex when no tile is selected.
			static const int invalidIndex;

			// The display size of individual tiles in the palette
			static const float paletteTileSize;

		private:
			//------------------------------------------------------------------------------
			// Private Functions:
			//------------------------------------------------------------------------------

			// Handler for palette change
			void OnPaletteChange(const Event& event);

			// Mouse click event handlers for buttons
			void OnMouseClick(const Event& event);

			//------------------------------------------------------------------------------
			// Private Variables:
			//------------------------------------------------------------------------------

			// Systems
			EventManager* events;

			// Components
			Sprite* sprite;
			Reactive* reactive;
			Transform* transform;

			// Other
			int selectedTileIndex;
			bool visible;

			COMPONENT_SUBCLASS_DECLARATION(TilePalette)
		};
	}
}

//------------------------------------------------------------------------------
