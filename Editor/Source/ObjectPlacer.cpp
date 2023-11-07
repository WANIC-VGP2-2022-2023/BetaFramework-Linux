//------------------------------------------------------------------------------
//
// File Name:	ObjectPlacer.cpp
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
#include "ObjectPlacer.h"

// Systems
#include <Input.h>
#include <EngineCore.h>
#include <SpaceManager.h>
#include <GraphicsEngine.h>	// ScreenToWorldPosition

// Components
#include <Transform.h>	// GetTranslation
#include <Sprite.h>		// SetZDepth

// Resources
#include <Tilemap.h>

// Levels
#include "EditorLevel.h"

//------------------------------------------------------------------------------

namespace Beta
{
	namespace Editor
	{
		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		namespace
		{
			const float guiClickDelay = 0.5f;
			const float objectChangeDelay = 2.0f;
			const float mapLoadDelay = 2.0f;
		}

		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		ObjectPlacer::ObjectPlacer()
			: Component("ObjectPlacer"), archetype(nullptr), delayEdit(false), map(nullptr)
		{
		}

		void ObjectPlacer::Initialize()
		{
			// Get the GUI space
			Space* guiSpace = EngineGetModule(SpaceManager)->GetSpaceByName("GuiSpace");

			// Register event handlers
			events = EngineGetModule(EventManager);
			events->RegisterEventHandler(
				guiSpace, "GUIClick", *this, &ObjectPlacer::OnGuiClick);
			events->RegisterEventHandler(
				GetOwner()->GetSpace(), "ObjectCreate", *this, &ObjectPlacer::OnObjectPlace);
			events->RegisterEventHandler(
				GetOwner()->GetSpace(), "ObjectDestroy", *this, &ObjectPlacer::OnObjectPlace);
			events->RegisterEventHandler(
				GetOwner()->GetSpace(), "MapLoad", *this, &ObjectPlacer::OnMapLoad);
			events->RegisterEventHandler(
				GetOwner()->GetSpace(), "ArchetypeChange", *this, &ObjectPlacer::OnArchetypeChange);

			// Get components
			transform = GetOwner()->GetComponent<Transform>();
		}

		void ObjectPlacer::Update(float dt)
		{
			// Hack to prevent accidental map edits while interacting with GUI elements
			// Will fix using proper raycasting later
			timer -= dt;
			if (timer <= 0)
				delayEdit = false;
		}

		void ObjectPlacer::DelayNextEdit(float time)
		{
			delayEdit = true;
			timer = time;
		}

		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------

		void ObjectPlacer::OnObjectPlace(const Event& event)
		{
			UNREFERENCED_PARAMETER(event);

			if (delayEdit)
				return;

			// CALCULATE MAP INDICES
			// Get mouse world position
			Input& input = *EngineGetModule(Input);
			Vector2D mousePosition = GetOwner()->GetSpace()->GetCamera().ScreenToWorldPosition(input.GetCursorPosition());
			// Apply inverse map transform
			mousePosition = transform->GetInverseMatrix() * mousePosition;
			// Normalize, invert y
			int x = static_cast<int>(floorf(mousePosition.x + 0.5f));
			int y = static_cast<int>(floorf(-mousePosition.y + 0.5f));

			// Remove object currently in map at location
			GameObject* current = map->RemoveObject(x, y);
			if (current)
				current->Destroy();

			// Create new object
			if (event.type == "ObjectCreate")
			{
				GameObject* object = new GameObject(archetype);

				// Place the object in the world
				PlaceObject(object, x, y);

				// Add to map
				map->AddObject(object, x, y);
			}

			DelayNextEdit(guiClickDelay);
		}

		void ObjectPlacer::OnGuiClick(const Event&)
		{
			DelayNextEdit(guiClickDelay);
		}

		void ObjectPlacer::OnMapLoad(const Event& event)
		{
			// Destroy existing objects
			if (map != nullptr)
			{
				const Array<ObjectInMap>& oldObjects = map->GetObjects();
				for (auto it = oldObjects.Begin(); it != oldObjects.End(); ++it)
				{
					GameObject* objectInMap = it->object;
					if (objectInMap != nullptr)
					{
						it->object->Destroy();
						it->object = nullptr;
					}
				}
			}

			// Set new map
			const MapLoad& loadEvent = static_cast<const MapLoad&>(event);
			map = loadEvent.map;

			if (map != nullptr)
			{
				// Place new objects
				const Array<ObjectInMap>& newObjects = map->GetObjects();
				for (auto it = newObjects.Begin(); it != newObjects.End(); ++it)
				{
					// Try to get existing archetype
					const std::string& archetypeName = (*it).name;
					GameObjectFactory* factory = EngineGetModule(GameObjectFactory);
					GameObject* object = factory->CreateObject(archetypeName, true);
					(*it).object = object;
					PlaceObject(object, (*it).x, (*it).y);
				}
			}

			// Delay next map edit
			DelayNextEdit(mapLoadDelay);
		}

		void ObjectPlacer::OnArchetypeChange(const Event& event)
		{
			const ArchetypeChange& archetypeEvent = static_cast<const ArchetypeChange&>(event);
			archetype = archetypeEvent.archetype;
			DelayNextEdit(objectChangeDelay);
		}

		void ObjectPlacer::PlaceObject(GameObject* object, int x, int y)
		{
			Transform* objectTransform = object->GetComponent<Transform>();
			// Scale object based on tile size
			//const Vector2D& tileScale = transform->GetScale();
			//Vector2D normalizedScale = objectTransform->GetScale().Normalized();
			//objectTransform->SetScale(Vector2D(normalizedScale.x * tileScale.x, normalizedScale.y * tileScale.y));
			// Set translation to specific cell on map
			objectTransform->SetTranslation(transform->GetMatrix() *
				Vector2D(static_cast<float>(x), static_cast<float>(-y)));
			// Make object inactive
			object->SetActive(false);

			// Set Z-depth so it appears above tiles
			Sprite* objectSprite = object->GetComponent<Sprite>();
			if (objectSprite)
				objectSprite->SetZDepth(0.01f);

			// Add to object manager
			GetOwner()->GetSpace()->GetObjectManager().AddObject(*object);
		}

		// RTTI
		COMPONENT_SUBCLASS_DEFINITION(ObjectPlacer)
	}
}

//------------------------------------------------------------------------------
