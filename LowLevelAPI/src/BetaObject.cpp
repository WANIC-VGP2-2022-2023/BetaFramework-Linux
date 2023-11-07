//------------------------------------------------------------------------------
//
// File Name:	BetaObject.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BetaObject.h"

#include <assert.h>

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Constructors and Destructors:
	//------------------------------------------------------------------------------

	// Constructor
	// Params:
	//   name = The name of this object.
	BetaObject::BetaObject(const std::string& name, BetaObject* owner)
		: name(name), owner(owner), id()
	{
		GenerateID();
	}

	// Copy constructor
	BetaObject::BetaObject(const BetaObject & other)
		: name(other.name), owner(other.owner), id()
	{
		GenerateID();
	}

	// Assignment operator
	BetaObject& BetaObject::operator=(const BetaObject & other)
	{
		name = other.name;
		owner = other.owner;
		return *this;
	}

	// Destructor
	BetaObject::~BetaObject()
	{
		auto it = GetObjectList().find(id);

		// Object is not in list
		if (it == GetObjectList().end())
			return;

		// Make sure everyone knows this thing is gone if they try to retrieve it.
		GetObjectList().erase(it);
	}

	//------------------------------------------------------------------------------
	// Game Loop Functions:
	//------------------------------------------------------------------------------

	// Loads assets for this object. Occurs when a level starts for the first time.
	void BetaObject::Load()
	{
	}

	// Resets instance data and reinstantiates child objects. Occurs when a
	// level is loaded or restarted.
	void BetaObject::Initialize()
	{
	}

	// Executes all operations that should occur every frame.
	// Params:
	//   dt = The change in time between last frame and the current frame.
	void BetaObject::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);
	}

	// Updates components using a fixed timestep (usually just for physics).
	// Params:
	//	 dt = A fixed change in time, usually 1/60th of a second.
	void BetaObject::FixedUpdate(float dt)
	{
		UNREFERENCED_PARAMETER(dt);
	}

	// Draw this object. Could be a sprite, text, or debug visuals.
	void BetaObject::Draw()
	{
	}

	// Removes any objects that will be recreated in Initialize.
	void BetaObject::Shutdown()
	{
	}

	// Unloads assets for this object. Occurs when changing levels.
	void BetaObject::Unload()
	{
	}

	//------------------------------------------------------------------------------
	// Other Public Functions:
	//------------------------------------------------------------------------------

	// Retrieves the name of the object.
	const std::string & BetaObject::GetName() const
	{
		return name;
	}

	// Give the object a new name - use with caution!
	void BetaObject::SetName(const std::string& name_)
	{
		name = name_;
	}

	// Returns the globally unique id of the object.
	const BetaObject::IDType BetaObject::GetID() const
	{
		return id;
	}

	// Retrieve the object that contains this object.
	BetaObject * BetaObject::GetOwner() const
	{
		return owner;
	}

	// Set the object that contains this object.
	void BetaObject::SetOwner(BetaObject * object)
	{
		owner = object;
	}

	// Find an object with the given GUID.
	BetaObject* BetaObject::GetObjectByID(IDType id)
	{
		auto it = GetObjectList().find(id);

		// Object is not in list
		if (it == GetObjectList().end())
			return nullptr;
		// Object is in list
		else
			return it->second;
	}

	// Generate an identifier for the object.
	void BetaObject::GenerateID()
	{
		static IDType nextID = 0;
		id = nextID++;
		GetObjectList().emplace(id, this);
	}

	// Retrieve the master object list
	BetaObject::ObjectList& BetaObject::GetObjectList()
	{
		// Create on first use (fix for static initialization order issues)
		static ObjectList masterObjectList;
		return masterObjectList;
	}
}

//------------------------------------------------------------------------------
