//------------------------------------------------------------------------------
//
// File Name:	BetaObjectC.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BetaObject.h"

#include <unordered_map>

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

// This struct provides a common interface for things like engine systems,
// levels/game states, game objects, and components.
typedef struct BetaObject
{
	// Name of the object.
	const char* name;

	// ID of the object
	unsigned id;

	// Object that contains this object.
	BetaObject* owner;

	// Game loop functions
	GameLoopFunctionTable functionTable;

	// Extra data
	void* data;
}BetaObject;

namespace
{
	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	// All systems, levels, and game objects that have been created
	std::unordered_map<IDType, BetaObject*> masterObjectList;

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	void BetaObjectGenerateID(BetaObject* object)
	{
		static IDType nextID = 0;
		object->id = nextID++;
		masterObjectList.emplace(object->id, object);
	}
}

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Create a dynamically allocated BetaObject.
// Params:
//   name = The name of this object.
//   owner = The owner object to this beta object (defaults to no owner).
//   table = The game loop functions used by this object.
//   data = Dynamically allocated data that can be accessed through this object.
// Returns:
//   A pointer to the newly created object.
BetaObject* BetaObjectCreate(const char* name, BetaObject* owner, GameLoopFunctionTable* table, void* data)
{
	BetaObject* object = new BetaObject();
	object->name = name;
	object->data = data;

	if (table != NULL)
	{
		object->functionTable = *table;
	}
	else
	{
		object->functionTable = { NULL, NULL, NULL, NULL, NULL, NULL, NULL };
	}

	BetaObjectSetOwner(object, owner);
	BetaObjectGenerateID(object);

	return object;
}

// Destroy a BetaObject and frees its dynamically allocated data.
// if any data was provided at construction.
// Params:
//   object - A pointer to a pointer to this object.
void BetaObjectDestroy(BetaObject** object)
{
	if (object == NULL || *object == NULL)
		return;

	if((*object)->data != NULL)
		free((*object)->data);

	delete* object;
	*object = NULL;
}

// Returns the name of the object.
// Params:
//   object - A pointer to this object.
const char* BetaObjectGetName(const BetaObject* object)
{
	if (object == NULL)
		return NULL;

	return object->name;
}

// Returns the id of the object.
// Params:
//   object - A pointer to this object.
IDType BetaObjectGetID(const BetaObject* object)
{
	if (object == NULL)
		return 0;

	return object->id;
}

// Retrieve the object that contains this object.
// Params:
//   object - A pointer to this object.
BetaObject* BetaObjectGetOwner(const BetaObject* object)
{
	if (object == NULL)
		return NULL;

	return object->owner;
}

// Set the object that owns or contains this object.
// Params:
//   object - A pointer to this object.
//   owner - A pointer to the object that owns this object.
void BetaObjectSetOwner(BetaObject* object, BetaObject* owner)
{
	if (object == NULL)
		return;

	object->owner = owner;
}

// Retrieves the game loop functions used by this object.
// Params:
//   object - A pointer to this object.
// Returns:
//   A pointer to the object's function table.
const GameLoopFunctionTable* BetaObjectGetFunctionTable(const BetaObject* object)
{
	if (object == NULL)
		return NULL;

	return &object->functionTable;
}

// Retrieve the data for this object.
// Params:
//   object - A pointer to this object.
// Returns:
//   A pointer to this object's data (or NULL if none was provided).
void* const BetaObjectGetData(BetaObject* object)
{
	if (object == NULL)
		return NULL;

	return object->data;
}

// Returns a pointer to an object with the given ID, if it exists.
// Returns NULL if the object does not exist (was destroyed).
// Params:
//   id - The ID of the object that should be retrieved.
BetaObject* GetObjectByID(IDType id)
{
	auto it = masterObjectList.find(id);

	// Object is not in list
	if (it == masterObjectList.end())
		return nullptr;
	// Object is in list
	else
		return it->second;
}

//------------------------------------------------------------------------------
