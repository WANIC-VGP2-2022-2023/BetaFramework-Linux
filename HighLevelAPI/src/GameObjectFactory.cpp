//------------------------------------------------------------------------------
//
// File Name:	GameObjectFactory.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameObjectFactory.h"

// Systems
#include "GameObject.h" // GetComponent
#include "FileStream.h"
#include "ResourceManager.h"
#include <Space.h>

// COMPONENTS
#include "Sprite.h"
#include "SpriteText.h"
#include "SpriteTilemap.h"
#include "Transform.h"
#include "Area.h"
#include "RigidBody.h"
#include "ColliderCircle.h"
#include "ColliderRectangle.h"
#include "ColliderLine.h"
#include "ColliderTilemap.h"
#include "Animator.h"
#include "Reactive.h"
#include "MapObjectSpawner.h"

#include "EngineCore.h" // GetFilePath

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Private Variables
	//------------------------------------------------------------------------------

	const std::string GameObjectFactory::objectFilePath = "Objects/";

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor is private to prevent accidental instantiation
	GameObjectFactory::GameObjectFactory()
		: BetaObject("GameObjectFactory")
	{
		// Register all HighLevel API components
		RegisterComponent<Transform>();
		RegisterComponent<Area>();
		RegisterComponent<Sprite>();
		RegisterComponent<SpriteText>();
		RegisterComponent<SpriteTilemap>();
		RegisterComponent<RigidBody>();
		RegisterComponent<Animator>();
		RegisterComponent<Reactive>();
		RegisterComponent<ColliderCircle>();
		RegisterComponent<ColliderRectangle>();
		RegisterComponent<ColliderLine>();
		RegisterComponent<ColliderTilemap>();
		RegisterComponent<MapObjectSpawner>();
	}

	// Destructor is private to prevent accidental destruction
	GameObjectFactory::~GameObjectFactory()
	{
		// Delete all registered components
		for (auto it = registeredComponents.begin(); it != registeredComponents.end(); ++it)
			delete it->second;
		registeredComponents.clear();
	}

	// Create a single instance of the specified game object.
	// Loads the object from a text file (if it exists).
	// Params:
	//   name = The name of the object.
	//   ignoreUnregisteredComponents = Whether to return the object even if it contains unregistered components.
	// Returns:
	//	 If the object file exists and can be loaded, then return a pointer to a new instance 
	//    of the specified game object type, else nullptr.
	GameObject* GameObjectFactory::CreateObject(const std::string & objectName, bool ignoreUnregisteredComponents)
	{
		// Create game object
		GameObject* object = new GameObject(objectName);

		// Create filename
		std::string filename = GameObject::GetArchetypeManager().GetFilePath() + objectName
			+ GameObject::GetArchetypeManager().GetFileExtension();
		FileStream stream(filename, OM_Read);
		try
		{
			// Attempt to load object
			object->Deserialize(stream);
		}
		catch (const FileStreamException & e)
		{
			std::cout << "ERROR in GameObjectFactory: " << e.what() << std::endl;
			if (!ignoreUnregisteredComponents)
			{
				delete object;
				object = nullptr;
			}
		}

		return object;
	}

	// Create a single instance of the specified component.
	// Params:
	//   name = The name of the component.
	// Returns:
	//	 If the component exists, then return a pointer to a new instance 
	//    of the specified component, else nullptr.
	Component* GameObjectFactory::CreateComponent(const std::string & componentName)
	{
		auto it = registeredComponents.find(componentName);

		if (it != registeredComponents.end())
		{
			return it->second->Clone();
		}

		return nullptr;
	}

	// Saves an object to an archetype file so it can be loaded later.
	void GameObjectFactory::SaveObjectToFile(const GameObject * object)
	{
		// Create filename
		std::string filename = GameObject::GetArchetypeManager().GetFilePath()
			+ object->GetName() + GameObject::GetArchetypeManager().GetFileExtension();
		FileStream stream(filename, OM_Write);

		// Attempt to save object
		object->Serialize(stream);
		std::cout << "Wrote object " << object->GetName() << " to file " << filename << std::endl;
	}
}
