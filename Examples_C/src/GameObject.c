//------------------------------------------------------------------------------
//
// File Name:	GameObject.c
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 2
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Animation.h"
/*#include "Behavior.h"
#include "BehaviorAsteroid.h"
#include "BehaviorBullet.h"
#include "BehaviorHudText.h"
#include "BehaviorSpaceship.h"
#include "Collider.h"
#include "ColliderCircle.h"
#include "ColliderLine.h"*/
#include "GameObject.h"
#include "Physics.h"
#include "Sprite.h"
#include "Stream.h"
#include "Transform.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct GameObject
{
	// The name of the game object.
	// A buffer is used to allow each game object to have a unique name.
	// The buffer is hardcoded to an arbitrary length that will be sufficient
	//	 for all CS230 assignments.  You may, instead, use dynamically-allocated
	//	 arrays for this purpose but the additional complexity is unnecessary
	//	 and it is your responsibility to ensure that the memory is successfully
	//	 allocated and deallocated in all possible situations.
	// [NOTE: When setting the name, use strncpy_s() to reduce the risk of
	//	 buffer overruns. Additionally, do NOT hardcode the number "32" when
	//	 calling this function!  Instead, use the _countof() macro to get the
	//	 size of the "name" array.]
	char name[32];

	// Flag to indicate that the game object is dead and should be destroyed.
	bool isDestroyed;

	// Pointer to an attached animation component.
	AnimationPtr animation;

	// Pointer to an attached behavior component.
	//BehaviorPtr behavior;

	// Pointer to an attached collider component.
	ColliderPtr collider;

	// Pointer to an attached physics component.
	PhysicsPtr physics;

	// Pointer to an attached sprite component.
	SpritePtr sprite;

	// Pointer to an attached transform component.
	TransformPtr transform;

} GameObject;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new game object.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
GameObjectPtr GameObjectCreate(void)
{
	GameObjectPtr gameObject = calloc(1, sizeof(GameObject));

	if (gameObject)
	{
		//gameObject->name = name;
	}
	return gameObject;
}

// Dynamically allocate a clone of an existing game object.
// (Hint: Make sure to perform a shallow copy or deep copy, as appropriate.)
// (WARNING: When you clone the behavior component you need to change the 'parent' variable.)
// Params:
//	 other = Pointer to the game object to be cloned.
// Returns:
//	 If 'other' is valid and the memory allocation was successful,
//	   then return a pointer to the cloned object,
//	   else return NULL.
GameObjectPtr GameObjectClone(const GameObjectPtr other)
{
	// Validate the pointer.
	if (other)
	{
		GameObjectPtr gameObject = calloc(1, sizeof(GameObject));
		if (gameObject)
		{
			*gameObject = *other;
			GameObjectAddSprite(gameObject, SpriteClone(other->sprite));
			GameObjectAddAnimation(gameObject, AnimationClone(other->animation));
			//GameObjectAddBehavior(gameObject, BehaviorClone(other->behavior));
			//GameObjectAddCollider(gameObject, ColliderClone(other->collider));
			GameObjectAddPhysics(gameObject, PhysicsClone(other->physics));
			GameObjectAddTransform(gameObject, TransformClone(other->transform));
		}
		return gameObject;
	}
	return NULL;
}

// Free the memory associated with a game object, including all components.
// (Also, set the game object pointer to NULL.)
// Params:
//	 gameObject = Pointer to the game object pointer.
void GameObjectFree(GameObjectPtr * gameObject)
{
	// Verify that a valid game object was specified.
	if (gameObject && *gameObject)
	{
		// Free all objects and components used for graphics.
		AnimationFree(&(*gameObject)->animation);
		//BehaviorFree(&(*gameObject)->behavior);
		//ColliderFree(&(*gameObject)->collider);
		PhysicsFree(&(*gameObject)->physics);
		SpriteFree(&(*gameObject)->sprite);
		TransformFree(&(*gameObject)->transform);

		// Free the allocated memory.
		free(*gameObject);

		// Set the game object pointer to NULL.
		*gameObject = NULL;
	}
}

// Read (and construct) the components associated with a game object.
// [NOTE: See project instructions for implementation instructions.]
// Params:
//	 gameObject = Pointer to the game object.
//	 stream = Pointer to the data stream used for reading.
void GameObjectRead(GameObjectPtr gameObject, Stream stream)
{
	if (gameObject && stream)
	{
		// Set the game object's name.
		const char * token = StreamReadToken(stream);
		GameObjectSetName(gameObject, token);

		// Build and add any necessary components.
		while (true)
		{
			token = StreamReadToken(stream);
			if (strncmp(token, "Transform", _countof("Transform")) == 0)
			{
				TransformPtr transform = TransformCreate();
				TransformRead(transform, stream);
				GameObjectAddTransform(gameObject, transform);
			}
			else if (strncmp(token, "Physics", _countof("Physics")) == 0)
			{
				PhysicsPtr physics = PhysicsCreate();
				PhysicsRead(physics, stream);
				GameObjectAddPhysics(gameObject, physics);
			}
			else if (strncmp(token, "Sprite", _countof("Sprite")) == 0)
			{
				SpritePtr sprite = SpriteCreate();
				SpriteRead(sprite, stream);
				GameObjectAddSprite(gameObject, sprite);
			}
			else if (strncmp(token, "Animation", _countof("Animation")) == 0)
			{
				AnimationPtr animation = AnimationCreate();
				AnimationRead(animation, stream);
				GameObjectAddAnimation(gameObject, animation);
			}
			/*else if (strncmp(token, "BehaviorAsteroid", _countof("BehaviorAsteroid")) == 0)
			{
				BehaviorPtr behavior = BehaviorAsteroidCreate();
				BehaviorRead(behavior, stream);
				GameObjectAddBehavior(gameObject, behavior);
			}
			else if (strncmp(token, "BehaviorBullet", _countof("BehaviorBullet")) == 0)
			{
				BehaviorPtr behavior = BehaviorBulletCreate();
				BehaviorRead(behavior, stream);
				GameObjectAddBehavior(gameObject, behavior);
			}
			else if (strncmp(token, "BehaviorHudText", _countof("BehaviorHudText")) == 0)
			{
				BehaviorPtr behavior = BehaviorHudTextCreate();
				BehaviorHudTextRead(behavior, stream);
				GameObjectAddBehavior(gameObject, behavior);
			}
			else if (strncmp(token, "BehaviorSpaceship", _countof("BehaviorSpaceship")) == 0)
			{
				BehaviorPtr behavior = BehaviorSpaceshipCreate();
				BehaviorRead(behavior, stream);
				GameObjectAddBehavior(gameObject, behavior);
			}
#if 0  // Project 5 Collider
			else if (strncmp(token, "Collider", _countof("Collider")) == 0)
			{
				ColliderPtr collider = ColliderCreate();
				ColliderRead(collider, stream);
				GameObjectAddCollider(gameObject, collider);
			}
#else	// Project 6 Collider
			else if (strncmp(token, "ColliderCircle", _countof("ColliderCircle")) == 0)
			{
				ColliderPtr collider = ColliderCircleCreate();
				ColliderCircleRead(collider, stream);
				GameObjectAddCollider(gameObject, collider);
			}
			else if (strncmp(token, "ColliderLine", _countof("ColliderLine")) == 0)
			{
				ColliderPtr collider = ColliderLineCreate();
				ColliderLineRead(collider, stream);
				GameObjectAddCollider(gameObject, collider);
			}
#endif*/
			else if (strncmp(token, "", _countof("")) == 0)
			{
				break;
			}
		}
	}
}

// Flag a game object for destruction.
// (Note: This is to avoid game objects being destroyed while they are being processed.)
// Params:
//	 gameObject = Pointer to the game object to be destroyed.
// Returns:
//	 If 'gameObject' is valid,
//	   then set the 'isDestroyed' boolean variable,
//	   else do nothing.
void GameObjectDestroy(GameObjectPtr gameObject)
{
	// Verify that a valid game object was specified.
	if (gameObject)
	{
		// Flag the game object for destruction.
		gameObject->isDestroyed = true;
	}
}

// Check whether a game object has been flagged for destruction.
// Params:
//	 gameObject = Pointer to the game object to be checked.
// Returns:
//	 If 'gameObject' is valid,
//	   then return the 'isDestroyed' boolean variable,
//	   else return false.
bool GameObjectIsDestroyed(GameObjectPtr gameObject)
{
	// Verify that a valid game object was specified.
	if (gameObject)
	{
		// Return true if the game object is flagged for destruction.
		return gameObject->isDestroyed;
	}
	return false;
}

// Attach an animation component to a game object.
// (NOTE: This function must set the animation component's parent pointer by
//	  calling the AnimationSetParent() function.)
// Params:
//	 gameObject = Pointer to the game object.
//   animation = Pointer to the animation component to be attached.
void GameObjectAddAnimation(GameObjectPtr gameObject, AnimationPtr animation)
{
	// Verify that a valid game object was specified.
	if (gameObject)
	{
		gameObject->animation = animation;
		AnimationSetParent(animation, gameObject);
	}
}

// Attach a behavior component to a game object.
// Params:
//	 gameObject = Pointer to the game object.
//   behavior = Pointer to the behavior component to be attached.
/*void GameObjectAddBehavior(GameObjectPtr gameObject, BehaviorPtr behavior)
{
	// Verify that a valid game object was specified.
	if (gameObject && behavior)
	{
		gameObject->behavior = behavior;
		BehaviorSetParent(behavior, gameObject);
	}
}*/

// Attach a Collider component to a game object.
// Params:
//	 gameObject = Pointer to the game object.
//   collider = Pointer to the collider component to be attached.
/*void GameObjectAddCollider(GameObjectPtr gameObject, ColliderPtr collider)
{
	// Verify that a valid game object was specified.
	if (gameObject)
	{
		gameObject->collider = collider;
		ColliderSetParent(collider, gameObject);
	}
}*/

// Attach a physics component to a game object.
// Params:
//	 gameObject = Pointer to the game object.
//   physics = Pointer to the physics component to be attached.
void GameObjectAddPhysics(GameObjectPtr gameObject, PhysicsPtr physics)
{
	// Verify that a valid game object was specified.
	if (gameObject)
	{
		gameObject->physics = physics;
	}
}

// Attach a sprite component to a game object.
// Params:
//	 gameObject = Pointer to the game object.
//   sprite = Pointer to the sprite component to be attached.
void GameObjectAddSprite(GameObjectPtr gameObject, SpritePtr sprite)
{
	// Verify that a valid game object was specified.
	if (gameObject)
	{
		gameObject->sprite = sprite;
	}
}

// Attach a transform component to a game object.
// Params:
//	 gameObject = Pointer to the game object.
//   transform = Pointer to the transform component to be attached.
void GameObjectAddTransform(GameObjectPtr gameObject, TransformPtr transform)
{
	// Verify that a valid game object was specified.
	if (gameObject)
	{
		gameObject->transform = transform;
	}
}

// Set the game object's name.
// [NOTE: Verify that both pointers are valid before setting the name.]
// [NOTE: When setting the name, use strncpy_s() to reduce the risk of
//	 buffer overruns. Additionally, do NOT hardcode the number "32" when
//	 calling this function!  Instead, use the _countof() macro to get the
//	 size of the "name" array.]
// Params:
//	 gameObject = Pointer to the game object.
//	 newName = Pointer to the game object's new name.
void GameObjectSetName(GameObjectPtr gameObject, const char * name)
{
	if (gameObject && name)
	{
		strncpy_s(gameObject->name, _countof(gameObject->name), name, strlen(name));
	}
}

// Get the game object's name.
// Params:
//	 gameObject = Pointer to the game object.
// Returns:
//	 If the game object pointer is valid,
//		then return a pointer to the game object's name,
//		else return NULL.
const char * GameObjectGetName(const GameObject * gameObject)
{
	// Verify that a valid game object was specified.
	if (gameObject)
	{
		return gameObject->name;
	}
	return NULL;
}

// Compare the game object's name with the specified name.
// Params:
//	 gameObject = Pointer to the game object.
//   name = Pointer to the name to be checked.
// Returns:
//	 If the game object pointer is valid,
//		then return true if the names match, false otherwise,
//		else return false.
bool GameObjectIsNamed(const GameObject * gameObject, const char * name)
{
	if (gameObject)
	{
		return (strcmp(gameObject->name, name) == 0);
	}
	return false;
}

// Get the animation component attached to a game object.
// Params:
//	 gameObject = Pointer to the game object.
// Returns:
//	 If the game object pointer is valid,
//		then return a pointer to the attached animation component,
//		else return NULL.
AnimationPtr GameObjectGetAnimation(const GameObjectPtr gameObject)
{
	// Verify that a valid game object was specified.
	if (gameObject)
	{
		return gameObject->animation;
	}
	return NULL;
}

/*BehaviorPtr GameObjectGetBehavior(const GameObjectPtr gameObject)
{
	// Verify that a valid game object was specified.
	if (gameObject)
	{
		return gameObject->behavior;
	}
	return NULL;
}*/

/*ColliderPtr GameObjectGetCollider(const GameObjectPtr gameObject)
{
	// Verify that a valid game object was specified.
	if (gameObject)
	{
		return gameObject->collider;
	}
	return NULL;
}*/

// Get the physics component attached to a game object.
// Params:
//	 gameObject = Pointer to the game object.
// Returns:
//	 If the game object pointer is valid,
//		then return a pointer to the attached physics component,
//		else return NULL.
PhysicsPtr GameObjectGetPhysics(const GameObjectPtr gameObject)
{
	// Verify that a valid game object was specified.
	if (gameObject)
	{
		return gameObject->physics;
	}
	return NULL;
}

// Get the sprite component attached to a game object.
// Params:
//	 gameObject = Pointer to the game object.
// Returns:
//	 If the game object pointer is valid,
//		then return a pointer to the attached sprite component,
//		else return NULL.
SpritePtr GameObjectGetSprite(const GameObjectPtr gameObject)
{
	// Verify that a valid game object was specified.
	if (gameObject)
	{
		return gameObject->sprite;
	}
	return NULL;
}

// Get the transform component attached to a game object.
// Params:
//	 gameObject = Pointer to the game object.
// Returns:
//	 If the game object pointer is valid,
//		then return a pointer to the attached transform component,
//		else return NULL.
TransformPtr GameObjectGetTransform(const GameObjectPtr gameObject)
{
	// Verify that a valid game object was specified.
	if (gameObject)
	{
		return gameObject->transform;
	}
	return NULL;
}

// Update any components attached to the game object.
// (Hint: You will need to call AnimationUpdate() and PhysicsUpdate().)
// (NOTE: You must first check for valid pointers before calling these functions.)
// Params:
//	 gameObject = Pointer to the game object.
//	 dt = Change in time (in seconds) since the last game loop.
void GameObjectUpdate(GameObjectPtr gameObject, float dt)
{
	// Verify that a valid game object was specified.
	if (gameObject)
	{
		/*if (gameObject->behavior)
		{
			BehaviorUpdate(gameObject->behavior, dt);
		}*/

		if (gameObject->animation)
		{
			AnimationUpdate(gameObject->animation, dt);
		}

		if (gameObject->physics && gameObject->transform)
		{
			PhysicsUpdate(gameObject->physics, gameObject->transform, dt);
		}
	}
}

// Draw any visible components attached to the game object.
// (Hint: You will need to call SpriteDraw(), passing the sprite and transform components.)
// (NOTE: You must first check for valid pointers before calling this function.)
// Params:
//	 gameObject = Pointer to the game object.
void GameObjectDraw(GameObjectPtr gameObject)
{
	// Verify that a valid game object was specified.
	if (gameObject)
	{
		// Draw any attached sprite components.
		if (gameObject->sprite && gameObject->transform)
		{
			SpriteDraw(gameObject->sprite, gameObject->transform);
		}
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

