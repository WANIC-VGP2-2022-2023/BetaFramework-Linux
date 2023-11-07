//------------------------------------------------------------------------------
//
// File Name:	GridDraw.h
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

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

namespace Beta
{
	class Transform;
	class Tilemap;
	typedef std::shared_ptr<Tilemap> TilemapPtr;
	class DebugDraw;

	namespace Editor
	{
		//------------------------------------------------------------------------------
		// Public Structures:
		//------------------------------------------------------------------------------

		class GridDraw : public Component
		{
		public:
			//------------------------------------------------------------------------------
			// Public Functions:
			//------------------------------------------------------------------------------

			// Constructor
			GridDraw();

			// Initialize this component (happens at object creation).
			void Initialize() override;

			// Update logic for this component.
			void Update(float) override;

			void Draw() override;

		private:
			//------------------------------------------------------------------------------
			// Private Functions:
			//------------------------------------------------------------------------------

			void OnMapLoad(const Event& event);

			//------------------------------------------------------------------------------
			// Private Variables:
			//------------------------------------------------------------------------------

			// Systems
			EventManager* events;
			DebugDraw& debugDraw;

			// Components
			Transform* transform;

			// Resources
			TilemapPtr map;

			// Other
			int columns;
			int rows;

			COMPONENT_SUBCLASS_DECLARATION(GridDraw)
		};
	}
}

//------------------------------------------------------------------------------
