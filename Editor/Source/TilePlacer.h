//------------------------------------------------------------------------------
//
// File Name:	TilePlacer.h
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
#include <EventManager.h>

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Forward Declarations:
	//------------------------------------------------------------------------------

	class SpriteTilemap;
	class Tilemap;
	typedef std::shared_ptr<Tilemap> TilemapPtr;
	class Transform;
	class Input;

	namespace Editor
	{
		class TilePalette;

		//------------------------------------------------------------------------------
		// Public Structures:
		//------------------------------------------------------------------------------

		class TilePlacer : public Component
		{
		public:
			//------------------------------------------------------------------------------
			// Public Functions:
			//------------------------------------------------------------------------------

			// Constructor
			TilePlacer();

			// Initialize this component (happens at object creation).
			void Initialize() override;

			// Update logic for this component.
			void Update(float) override;

			void DelayNextEdit(float time);

		private:
			//------------------------------------------------------------------------------
			// Private Functions:
			//------------------------------------------------------------------------------

			// Event handlers for GUI stuff
			void OnTileChange(const Event& event);
			void OnGuiClick(const Event& event);

			// Handlers for saving/loading
			void OnMapSave(const Event& event);
			void OnMapLoad(const Event& event);
			void OnPaletteChange(const Event& event);

			//------------------------------------------------------------------------------
			// Private Variables:
			//------------------------------------------------------------------------------

			// Systems
			 EventManager* events;
			Input& input;

			// Components
			TilePalette* palette;
			SpriteTilemap* sprite;
			Transform* transform;

			// Resources
			TilemapPtr map;

			// Misc
			bool delayEdit;
			float timer;

			COMPONENT_SUBCLASS_DECLARATION(TilePlacer)
		};
	}
}

//------------------------------------------------------------------------------
