//------------------------------------------------------------------------------
//
// File Name:	ObjectPlacer.h
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

	class Transform;
	class GameObject;
	typedef std::shared_ptr<const GameObject> Archetype;
	class Tilemap;
	typedef std::shared_ptr<Tilemap> TilemapPtr;

	namespace Editor
	{
		//------------------------------------------------------------------------------
		// Public Structures:
		//------------------------------------------------------------------------------

		class ObjectPlacer : public Component
		{
		public:
			//------------------------------------------------------------------------------
			// Public Functions:
			//------------------------------------------------------------------------------

			// Constructor
			ObjectPlacer();

			// Initialize this component (happens at object creation).
			void Initialize() override;

			// Update logic for this component.
			void Update(float) override;

			void DelayNextEdit(float time);

		private:
			//------------------------------------------------------------------------------
			// Private Functions:
			//------------------------------------------------------------------------------

			// Handlers for object creation and deletion
			void OnObjectPlace(const Event& event);

			// Handler for GUI interaction
			void OnGuiClick(const Event& event);

			// Handlers for map save/load
			void OnMapLoad(const Event& event);

			// Handler for changing which object is being placed
			void OnArchetypeChange(const Event& event);

			// Actual object placement function
			void PlaceObject(GameObject* object, int x, int y);

			//------------------------------------------------------------------------------
			// Private Variables:
			//------------------------------------------------------------------------------

			// Systems
			EventManager* events;

			// Components
			Transform* transform;

			// Resources
			TilemapPtr map;
			Archetype archetype;

			// Misc
			bool delayEdit;
			float timer;

			COMPONENT_SUBCLASS_DECLARATION(ObjectPlacer)
		};
	}
}

//------------------------------------------------------------------------------
