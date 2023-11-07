//------------------------------------------------------------------------------
//
// File Name:	CameraController.h
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
#include <Vector2D.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

namespace Beta
{
	class Camera; 
	class Sprite;
	class Transform;
	struct Event;
	class Input;
	class Tilemap;
	typedef std::shared_ptr<Tilemap> TilemapPtr;

	//------------------------------------------------------------------------------
	// Public Structures:
	//------------------------------------------------------------------------------

	namespace Editor
	{
		class CameraController : public Component
		{
		public:
			//------------------------------------------------------------------------------
			// Public Functions:
			//------------------------------------------------------------------------------

			// Constructor
			CameraController();

			// Initialize this component (happens at object creation).
			void Initialize() override;

			// Update data associated with this component.
			// Params:
			//   dt = The change in time since the previous frame.
			void Update(float dt) override;

			// Sets the pan speed of the camera
			void SetPanSpeed(float value);

			// Sets the zoom speed of the camera.
			void SetZoomSpeed(float value);

		private:
			//------------------------------------------------------------------------------
			// Private Functions:
			//------------------------------------------------------------------------------

			void OnMapLoad(const Event& event);

			void OnReset(const Event& event);

			void OnMouseClick(const Event& event);
			void OnMouseUp(const Event& event);

			// Mouse click event handlers for buttons
			void HandleInput(float dt);

			//------------------------------------------------------------------------------
			// Private Variables:
			//------------------------------------------------------------------------------

			// Properties
			float moveSpeed;
			float zoomSpeed;

			// Components
			Transform* myTransform;
			Transform* mapTransform;
			Sprite* mapSprite;

			// Resources
			Camera* camera;
			Input& input;
			TilemapPtr map;

			// Other variables
			float targetFov;
			Vector2D mouseStart;
			Vector2D mouseCurrent;
			bool mouseHeld;

			COMPONENT_SUBCLASS_DECLARATION(CameraController)
		};
	}
}

//------------------------------------------------------------------------------
