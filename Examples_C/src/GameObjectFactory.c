//------------------------------------------------------------------------------
//
// File Name:	GameObjectFactory.c
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 4
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "GameObjectManager.h"
#include "Stream.h"

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Build a single instance of the specified game object.
// (NOTE: Use sprintf_s() to construct a path name using objectName.)
// (HINT: The correct path name should be constructed using "Data/&s.txt".)
// (HINT: See project instructions.)
// Params:
//	 objectName = The name of the object to be deserialized.
// Returns:
//	 If the objectName is valid,
//	   then return a pointer to a new instance of the specified game object,
//	   else NULL.
GameObjectPtr GameObjectFactoryBuild(const char * objectName)
{
	if (objectName)
	{
		// Search for the specified object in the archetype list.
		GameObjectPtr archetype = GameObjectManagerGetArchetype(objectName);

		// If the archetype does not yet exist.
		if (!archetype)
		{
			// Construct the file pathname (“Data/%s.txt”) using the object name.
			char pathname[FILENAME_MAX] = "";
			sprintf_s(pathname, _countof(pathname), "Data/%s.txt", objectName);

			// Open the file for streaming.
			Stream stream = StreamOpen(pathname);
			if (stream)
			{
				// Verify that a game object file has been opened.

				// If the first token is “GameObject”
				const char * token = StreamReadToken(stream);
				if (strncmp(token, "GameObject", _countof("GameObject")) == 0)
				{
					// Create a new game object.
					archetype = GameObjectCreate();
					// Read the contents of the game object from the data file.
					GameObjectRead(archetype, stream);
					// Add the new game object to the archetype list.
					GameObjectManagerAddArchetype(archetype);
				}
				// Close the stream.
				StreamClose(&stream);
			}
		}

		// If the archetype exists (either already existing or newly created).
		if (archetype)
		{
			// Clone the archetype.
			GameObjectPtr gameObject = GameObjectClone(archetype);

			// Return the pointer to the cloned game object.
			return gameObject;
		}
	}
	return NULL;
}
