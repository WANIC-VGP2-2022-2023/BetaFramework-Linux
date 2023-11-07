//------------------------------------------------------------------------------
//
// File Name:	GameObjectManager.c
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 5
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
//#include "Collider.h"
#include "GameObject.h"
#include "GameObjectManager.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

#define objectListSize 100

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct GameObjectManager
{
	// The current number of game objects currently in the list.
	unsigned int objectCount;

	// The maximum number of game objects ever in the list, at any given moment.
	unsigned int objectMax;

	// This list can be a fixed-length array (minimum size of 100 entries)
	// or a dynamically sized array, such as a linked list.
	GameObjectPtr	objectList[objectListSize];

} GameObjectManager;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

static GameObjectManager gameObjectActiveList;
static GameObjectManager gameObjectArchetypes;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

void GameObjectManagerAddToList(GameObjectManager * manager, GameObjectPtr gameObject);
GameObjectPtr GameObjectManagerGetFromList(GameObjectManager * manager, const char * name);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Initialize the game object manager.
// (NOTE: This function should initialize the lists for both active game objects
//    and the archetype objects).
void GameObjectManagerInit(void)
{
	memset(&gameObjectActiveList, 0, sizeof(gameObjectActiveList));
	memset(&gameObjectArchetypes, 0, sizeof(gameObjectArchetypes));
}

// Update all objects in the active game objects list.
// (NOTE: After a game object has been updated, if it has been flagged as
//    destroyed, then it must be removed from the list and freed properly.
//    Additionally, the count of active objects must be reduced by 1.)
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameObjectManagerUpdate(float dt)
{
	for (unsigned int i = 0; i < objectListSize; ++i)
	{
		if (gameObjectActiveList.objectList[i] != NULL)
		{
			GameObjectUpdate(gameObjectActiveList.objectList[i], dt);

			if (GameObjectIsDestroyed(gameObjectActiveList.objectList[i]))
			{
				GameObjectFree(&(gameObjectActiveList.objectList[i]));
				--gameObjectActiveList.objectCount;
			}
		}
	}
}

// Check collisions between all objects held by the game object manager.
void GameObjectManagerCheckCollisions(void)
{
	/*for (unsigned int i = 0; i < objectListSize; ++i)
	{
		if (gameObjectActiveList.objectList[i] != NULL)
		{
			ColliderPtr collider1 = GameObjectGetCollider(gameObjectActiveList.objectList[i]);
			if (collider1)
			{
				for (unsigned int j = i + 1; j < objectListSize; ++j)
				{
					if (gameObjectActiveList.objectList[j] != NULL)
					{
						ColliderPtr collider2 = GameObjectGetCollider(gameObjectActiveList.objectList[j]);
						if (collider2)
						{
							ColliderCheck(collider1, collider2);
						}
					}
				}

			}
		}
	}*/
}

// Draw all game objects in the active game object list.
void GameObjectManagerDraw(void)
{
	for (unsigned int i = 0; i < objectListSize; ++i)
	{
		if (gameObjectActiveList.objectList[i] != NULL)
		{
			GameObjectDraw(gameObjectActiveList.objectList[i]);
		}
	}
}

// Shutdown the game object manager.
// (NOTE: This means removing all game objects from both the active and
//	  archetype game object lists.  Make sure that the object counts are
//	  properly updated in both situations.)
void GameObjectManagerShutdown(void)
{
	// Free all active game objects.
	for (unsigned int i = 0; i < objectListSize; ++i)
	{
		// Check for an active game object.
		if (gameObjectActiveList.objectList[i] != NULL)
		{
			// Free the current game object.
			GameObjectFree(&gameObjectActiveList.objectList[i]);
			--gameObjectActiveList.objectCount;
		}

		// Check for an active game object.
		if (gameObjectArchetypes.objectList[i] != NULL)
		{
			// Free the current game object.
			GameObjectFree(&gameObjectArchetypes.objectList[i]);
			--gameObjectArchetypes.objectCount;
		}
	}
}

// Add a game object to the active game object list.
// (Hint: This function and the GameObjectManagerAddArchetype functions require
//    the same code.  Consider creating a single function that adds an object
//	  to a list that is passed as a parameter.)
// Params:
//	 gameObject = Pointer to the game object to be added to the list.
void GameObjectManagerAdd(GameObjectPtr gameObject)
{
	// Add the game object to the managed lists of game objects.
	GameObjectManagerAddToList(&gameObjectActiveList, gameObject);
}

// Add a game object to the game object archetype list.
// (Hint: This function and the GameObjectManagerAdd functions require
//    the same code.  Consider creating a single, private function that adds
//	 an object to a list that is passed as a parameter.)
// Params:
//	 gameObject = Pointer to the game object to be added to the list.
void GameObjectManagerAddArchetype(GameObjectPtr gameObject)
{
	// Add the game object to the list of game object archetypes.
	GameObjectManagerAddToList(&gameObjectArchetypes, gameObject);
}

// Returns a pointer to the first active game object matching the specified name.
// (Hint: This function and the GameObjectManagerGetObjectByName functions require
//    the same code.  Consider creating a single, private function that searches
//	  for the named game object in a list that is passed as a parameter.)
// Params:
//	 name = The name of the archetype to be returned, if found.
// Returns:
//   If the named archetype is found,
//	   then return the pointer to the named game object archetype,
//	   else return NULL.
GameObjectPtr GameObjectManagerGetObjectByName(const char * name)
{
	return GameObjectManagerGetFromList(&gameObjectActiveList, name);
}

// Returns a pointer to the first game object archetype matching the specified name.
// Params:
//	 name = The name of the archetype to be returned, if found.
// Returns:
//   If the named archetype is found,
//	   then return the pointer to the named game object archetype,
//	   else return NULL.
GameObjectPtr GameObjectManagerGetArchetype(const char * name)
{
	return GameObjectManagerGetFromList(&gameObjectArchetypes, name);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

void GameObjectManagerAddToList(GameObjectManager * manager, GameObjectPtr gameObject)
{
	// Verify the pointers.
	if (manager && gameObject)
	{
		// Verify that the manager can hold more game objects.
		if (manager->objectCount < objectListSize)
		{
			for (unsigned int i = 0; i < objectListSize; ++i)
			{
				if (manager->objectList[i] == NULL)
				{
					manager->objectList[i] = gameObject;

					// Update the object count and object maximum values.
					++manager->objectCount;
					if (manager->objectMax < manager->objectCount)
					{
						manager->objectMax = manager->objectCount;
					}
					break;
				}
			}
		}
	}
}

// Returns a pointer to the first game object matching the specified name.
// Params:
//   manager = Pointer to the manager containing the list to be searched.
//	 name = The name of the archetype to be returned, if found.
// Returns:
//   If the named game object is found,
//	   then return the pointer to the game object,
//	   else return NULL.
GameObjectPtr GameObjectManagerGetFromList(GameObjectManager * manager, const char * name)
{
	for (unsigned int i = 0; i < objectListSize; ++i)
	{
		if (manager->objectList[i] != NULL)
		{
			if (strcmp(name, GameObjectGetName(manager->objectList[i])) == 0)
			{
				return manager->objectList[i];
			}
		}
	}
	return NULL;
}

