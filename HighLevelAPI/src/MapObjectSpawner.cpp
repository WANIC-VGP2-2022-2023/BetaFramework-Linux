//------------------------------------------------------------------------------
//
// File Name:	MapObjectSpawner.cpp
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
#include "MapObjectSpawner.h"

// Systems
#include <EngineCore.h>		// EngineGetModule
#include <GraphicsEngine.h>	// ScreenToWorldPosition
#include <FileStream.h>		// Read/Write variables
#include <GameObject.h>	// GetSpace
#include <Space.h>		// GetObjectManager

// Components
#include <Transform.h>	// GetTranslation

// Resources
#include <Tilemap.h>	// GetObjects

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	MapObjectSpawner::MapObjectSpawner()
		: Component("MapObjectSpawner"), map(nullptr), transform(nullptr)
	{
	}

	void MapObjectSpawner::Initialize()
	{
		// Get components
		transform = GetOwner()->GetComponent<Transform>();

		// Place new objects
		const Array<ObjectInMap>& newObjects = map->GetObjects();
		for (auto it = newObjects.Begin(); it != newObjects.End(); ++it)
		{
			// Try to get existing archetype
			const std::string& archetypeName = (*it).name;
			GameObject* object = new GameObject(ResourceGetArchetype(archetypeName));
			(*it).object = object;
			PlaceObject(object, (*it).x, (*it).y);
		}
	}

	// Sets the tilemap data that will be used by this component.
	// Params:
	//   map = A pointer to the tilemap resource.
	void MapObjectSpawner::SetTilemap(ConstTilemapPtr _map)
	{
		map = _map;
	}

	// Loads object data from a file.
	// Params:
	//   stream = The stream for the file we want to read from.
	void MapObjectSpawner::Deserialize(FileStream& stream)
	{
		std::string tilemapName;
		stream.ReadVariable("tilemap", tilemapName);
		SetTilemap(ResourceGetTilemap(tilemapName));
	}

	// Saves object data to a file.
	// Params:
	//   stream = The stream for the file we want to write to.
	void MapObjectSpawner::Serialize(FileStream& stream) const
	{
		stream.WriteVariable("tilemap", map->GetName());
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	void MapObjectSpawner::PlaceObject(GameObject* object, int x, int y)
	{
		Transform* objectTransform = object->GetComponent<Transform>();

		// Set translation to specific cell on map
		objectTransform->SetTranslation(transform->GetMatrix() *
			Vector2D(static_cast<float>(x), static_cast<float>(-y)));

		// Add to object manager
		GetOwner()->GetSpace()->GetObjectManager().AddObject(*object);
	}

	// RTTI
	COMPONENT_SUBCLASS_DEFINITION(MapObjectSpawner)
}

//------------------------------------------------------------------------------
