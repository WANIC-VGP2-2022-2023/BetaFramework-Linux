//------------------------------------------------------------------------------
//
// File Name:	BetaObject.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2
//
// Copyright � 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

#define BE_C_API
//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Typedefs:
//------------------------------------------------------------------------------

typedef struct BetaObject BetaObject;
typedef unsigned IDType;
typedef void (*GameLoopFunctionPointer)(BetaObject*);
typedef void (*GameLoopUpdateFunctionPointer)(BetaObject*, float);

//------------------------------------------------------------------------------
// Game Loop Functions:
//------------------------------------------------------------------------------

typedef struct GameLoopFunctionTable
{
	// Loads external resources associated with this object.
	// Occurs when a level starts for the first time.
	GameLoopFunctionPointer load;

	// Resets instance data and reinstantiates child objects.
	// Occurs when a level is loaded or restarted.
	GameLoopFunctionPointer initialize;

	// Executes all operations that should occur every frame.
	// Params:
	//   dt = The change in time between last frame and the current frame.
	GameLoopUpdateFunctionPointer update;

	// Updates components using a fixed timestep (usually just for physics).
	// Params:
	//	 dt = A fixed change in time, usually 1/60th of a second.
	GameLoopUpdateFunctionPointer fixedUpdate;

	// Draw this object. Could be a sprite, text, or debug visuals.
	GameLoopFunctionPointer draw;

	// Removes any objects that will be recreated in Initialize.
	GameLoopFunctionPointer shutdown;

	// Unloads external assets associated with this object.
	// Occurs when changing levels.
	GameLoopFunctionPointer unload;
}GameLoopFunctionTable;

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
BE_C_API BetaObject* BetaObjectCreate(const char* name, BetaObject* owner, GameLoopFunctionTable* table, void* data);

// Destroy a BetaObject and frees its dynamically allocated data.
// if any data was provided at construction.
// Params:
//   object - A pointer to a pointer to this object.
BE_C_API void BetaObjectDestroy(BetaObject** object);

// Returns the name of the object.
// Params:
//   object - A pointer to this object.
BE_C_API const char* BetaObjectGetName(const BetaObject* object);

// Returns the ID of the object.
// Params:
//   object - A pointer to this object.
BE_C_API IDType BetaObjectGetID(const BetaObject* object);

// Retrieve the object that contains this object.
// Params:
//   object - A pointer to this object.
BE_C_API BetaObject* BetaObjectGetOwner(const BetaObject* object);

// Set the object that owns or contains this object.
// Params:
//   object - A pointer to this object.
//   owner - A pointer to the object that owns this object.
BE_C_API void BetaObjectSetOwner(BetaObject* object, BetaObject* owner);

// Retrieves the game loop functions used by this object.
// Params:
//   object - A pointer to this object.
// Returns:
//   A pointer to the object's function table.
BE_C_API const GameLoopFunctionTable* BetaObjectGetFunctionTable(const BetaObject* object);

// Retrieve the data for this object.
// Params:
//   object - A pointer to this object.
// Returns:
//   A pointer to this object's data (or NULL if none was provided).
BE_C_API void* const BetaObjectGetData(BetaObject* object);

// Returns a pointer to an object with the given ID, if it exists.
// Returns NULL if the object does not exist (was destroyed).
// Params:
//   id - The ID of the object that should be retrieved.
BE_C_API BetaObject* GetObjectByID(IDType id);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif
