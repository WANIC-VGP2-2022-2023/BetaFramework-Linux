//------------------------------------------------------------------------------
//
// File Name:	GameObject.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameObject.h"

#include <EngineCore.h>			// GetModule
#include "Component.h"			// Clone
#include "Space.h"				// static_cast to Space*
#include "GameObjectFactory.h"	// CreateComponent
#include "FileStream.h"			// WriteValue, ReadSkip, BeginScope, EndScope

//------------------------------------------------------------------------------

namespace Beta
{
	ArchetypeManager GameObject::archetypeManager;

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	ArchetypeManager::ArchetypeManager()
		: ResourceManager("Objects/")
	{
	}

	Archetype ArchetypeManager::Create(const std::string& name)
	{
		// Object factory will handle this.
		return Archetype(EngineGetModule(GameObjectFactory)->CreateObject(name));
	}

	//Create a new game object.
	// Params:
	//	 name = The name of the game object being created.   
	GameObject::GameObject(const std::string& name)
		: BetaObject(name), isDestroyed(false), active(true)
	{
	}

	// Copy a game object from another object. Explicit to prevent accidental copies.
	// Params:
	//	 other = A reference to the object being cloned.
	GameObject::GameObject(const GameObject& other)
		: BetaObject(other.GetName()), isDestroyed(false), active(true), baseArchetype(other.baseArchetype)
	{
		size_t numComponentsOther = other.components.Size();
		for (size_t i = 0; i < numComponentsOther; ++i)
			AddComponent(other.components[i]->Clone());
	}

	// Copy a game object from an archetype. Explicit to prevent accidental copies.
	// Params:
	//	 archetype = A pointer to the object being cloned.
	GameObject::GameObject(Archetype other)
		: BetaObject(other->GetName()), isDestroyed(false), active(true), baseArchetype(other)
	{
		size_t numComponentsOther = other->components.Size();
		for (size_t i = 0; i < numComponentsOther; ++i)
			AddComponent(other->components[i]->Clone());
	}

	// Free the memory associated with a game object.
	GameObject::~GameObject()
	{
		size_t numComponents = components.Size();
		for (size_t i = 0; i < numComponents; ++i)
		{
			components[i]->UnregisterEventHandlers();
			delete components[i];
		}
		components.Clear();
	}

	// Initialize this object's components and set it to active.
	void GameObject::Initialize()
	{
		// Initialize the components
		size_t numComponents = components.Size();
		for (size_t i = 0; i < numComponents; ++i)
			components[i]->Initialize();
	}

	// Update any components attached to the game object.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void GameObject::Update(float dt)
	{
		if (isDestroyed || !active) return;

		size_t numComponents = components.Size();
		for (size_t i = 0; i < numComponents; ++i)
			components[i]->Update(dt);
	}

	// Updates components using a fixed timestep (usually just physics)
	// Params:
	//	 dt = A fixed change in time, usually 1/60th of a second.
	void GameObject::FixedUpdate(float dt)
	{
		if (isDestroyed || !active) return;

		size_t numComponents = components.Size();
		for (size_t i = 0; i < numComponents; ++i)
			components[i]->FixedUpdate(dt);
	}

	// Draw any visible components attached to the game object.
	void GameObject::Draw()
	{
		size_t numComponents = components.Size();
		for (size_t i = 0; i < numComponents; ++i)
			components[i]->Draw();
	}

	// Adds a component to the object.
	void GameObject::AddComponent(Component* component)
	{
		component->SetOwner(this);
		components.PushBack(component);
	}

	// Whether the object has been marked for destruction.
	// Returns:
	//		True if the object will be destroyed, false otherwise.
	bool GameObject::IsDestroyed() const
	{
		return isDestroyed;
	}

	// Mark an object for destruction.
	void GameObject::Destroy()
	{
		isDestroyed = true;
	}

	// Tells whether an object will be updated.
	bool GameObject::IsActive() const
	{
		return active;
	}

	// Sets whether an object will be updated.
	void GameObject::SetActive(bool value)
	{
		active = value;
	}

	// Get the space that contains this object.
	Space* GameObject::GetSpace() const
	{
		return static_cast<Space*>(BetaObject::GetOwner());
	}

	// Save object data to file.
	// Params:
	//   stream = The stream object used to save the object's data.
	void GameObject::Serialize(FileStream& stream) const
	{
		stream.WriteValue(GetName());
		stream.BeginScope();
		size_t numComponents = components.Size();
		stream.WriteVariable("numComponents", numComponents);
		for (size_t i = 0; i < numComponents; ++i)
		{
			stream.WriteValue(components[i]->GetName());
			stream.BeginScope();
			components[i]->Serialize(stream);
			stream.EndScope();
		}
		stream.EndScope();
	}

	// Load object data from file
	// Params:
	//   stream = The stream object used to load the object's data.
	void GameObject::Deserialize(FileStream& stream)
	{
		stream.ReadSkip(GetName());
		stream.BeginScope();
		unsigned numToAdd = 0;
		stream.ReadVariable("numComponents", numToAdd);
		std::string componentName;
		for (unsigned i = 0; i < numToAdd; ++i)
		{
			stream.ReadValue(componentName);
			Component* component = EngineGetModule(GameObjectFactory)->CreateComponent(componentName);

			// Couldn't create component
			if (component == nullptr)
			{
				throw FileStreamException(GetName() + GameObject::archetypeManager.GetFileExtension(), 
					"Unrecognized component name: " + componentName);
			}

			AddComponent(component);
			stream.BeginScope();
			component->Deserialize(stream);
			stream.EndScope();
		}
		stream.EndScope();
	}

	ArchetypeManager& GameObject::GetArchetypeManager()
	{
		return archetypeManager;
	}
}
