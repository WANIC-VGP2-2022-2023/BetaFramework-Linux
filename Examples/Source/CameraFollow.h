//------------------------------------------------------------------------------
//
// File Name:	CameraFollow.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Component.h" // base class

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Forward Declarations
	//------------------------------------------------------------------------------

	class Transform;
	class Tilemap;
	typedef std::shared_ptr<const Tilemap> ConstTilemapPtr;

	//------------------------------------------------------------------------------
	// Public Structures:
	//------------------------------------------------------------------------------

	class CameraFollow : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		// Params:
		//   tilemap = The tilemap that constrains the camera.
		CameraFollow(ConstTilemapPtr tilemap = nullptr);

		// Initialize data for this object.
		void Initialize() override;

		// Update function for this component.
		// Params:
		//   dt = The (fixed) change in time since the last step.
		void Update(float dt) override;

		// Set the tilemap that will constrain the camera
		void SetTilemap(ConstTilemapPtr tilemap);

		// Loads object data from a file.
		// Params:
		//   stream = The stream for the file we want to read from.
		virtual void Deserialize(FileStream& stream);

		// Saves object data to a file.
		// Params:
		//   stream = The stream for the file we want to write to.
		virtual void Serialize(FileStream& stream) const;

	private:
		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Resources
		const Transform* transform;
		const Transform* mapTransform;
		ConstTilemapPtr tilemap;

		COMPONENT_SUBCLASS_DECLARATION(CameraFollow)
	};
}
