//------------------------------------------------------------------------------
//
// File Name:	GameObjectManager.cpp
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
#include "GameObjectManager.h"

// STD
#include <limits>

// Systems
#include <EngineCore.h>			// GetModule
#include <GraphicsEngine.h>		// GetWinMinX
#include "Quadtree.h"			// Constructor, AddObject, Clear
#include "Space.h"				// GameObject->SetOwner
#include "GameObjectFactory.h"	// CreateObject

// Components
#include "Collider.h"		// CheckCollision
#include "Transform.h"		// IsOnScreen
#include "ColliderLine.h"	// for raycasts
#include "SpriteTilemap.h"

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Private Function Declarations:
	//------------------------------------------------------------------------------

	void SwapGameObjects(GameObject** first, GameObject** second);

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	GameObjectManager::GameObjectManager(Space* space)
		: BetaObject("Module:GameObjectManager", space),
		timeAccumulator(0.0f), fixedUpdateDt(1.0f / 120.0f), quadtree(nullptr), quadtreeEnabled(false)
	{
		objects.Reserve(128);
	}

	// Destructor
	GameObjectManager::~GameObjectManager()
	{
		delete quadtree;
	}

	// Update all objects in the active game objects list.
	// (NOTE: After a game object has been updated, if it has been flagged as
	//    destroyed, then it must be removed from the list and freed properly.
	//    Additionally, the count of active objects must be reduced by 1.)
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void GameObjectManager::Update(float dt)
	{
		Space* space = static_cast<Space*>(GetOwner());

		// Remake the quadtree - so that raycasting doesn't go weird places
		PopulateQuadtree();

		// Update objects if not paused
		if (space == nullptr || !space->IsPaused())
		{
			// Normal update for objects.
			VariableUpdate(dt);

			// Update physics/check for collisions.
			FixedUpdate(dt);
		}

		// Destroy objects marked for deletion.
		DestroyObjects();
	}

	// Draw all game objects in the active game object list.
	void GameObjectManager::Draw(void)
	{
		size_t numObjects = objects.Size();
		for (size_t i = 0; i < numObjects; ++i)
		{
			objects[i]->Draw();
		}

		// Draw quadtree
		if (quadtreeEnabled && quadtree) quadtree->Draw();
	}

	// Shutdown the game object manager, destroying all active objects.
	void GameObjectManager::Shutdown(void)
	{
		for (auto it = objects.Begin(); it != objects.End(); ++it)
		{
			delete* it;
			(*it) = nullptr;
		}
		objects.Clear();

		// Delete the quadtree to prevent strange raycast behavior
		delete quadtree;
		quadtree = nullptr;
	}

	// Add a game object to the active game object list.
	// (Hint: This function and the GameObjectManagerAddArchetype functions require
	//    the same code.  Consider creating a single function that adds an object
	//	  to a list that is passed as a parameter.)
	// Params:
	//	 gameObject = Reference to the game object to be added to the list.
	void GameObjectManager::AddObject(GameObject& gameObject)
	{
		// Okay to add one object
		gameObject.SetOwner(GetOwner());
		gameObject.Initialize();
		objects.PushBack(&gameObject);
	}

	// Returns a pointer to the first active game object matching the specified name.
	// (Hint: This function and the GameObjectManagerGetObjectByName functions require
	//    the same code.  Consider creating a single, private function that searches
	//	  for the named game object in a list that is passed as a parameter.)
	// Params:
	//	 name = The name of the object to be returned, if found.
	// Returns:
	//   If the named object is found,
	//	   then return the pointer to the named game object,
	//	   else return nullptr.
	GameObject* GameObjectManager::GetObjectByName(const std::string& _name) const
	{
		for (auto it = objects.Begin(); it != objects.End(); ++it)
		{
			if (_name == (*it)->GetName() && !(*it)->IsDestroyed())
				return *it;
		}
		return nullptr;
	}

	// Returns the number of active objects with the given name.
	// Params:
	//   objectName = The name of the objects that should be counted.
	unsigned GameObjectManager::GetObjectCount(const std::string& objectName) const
	{
		unsigned count = 0;

		// Check for objects with specific name
		if (objectName != "")
		{
			for (auto it = objects.Begin(); it != objects.End(); ++it)
			{
				if ((*it)->IsDestroyed() || (*it)->GetName() != objectName)
					continue;

				++count;
			}
		}
		// If name is empty string, just return number of (not destroyed) objects
		else
		{
			for (auto it = objects.Begin(); it != objects.End(); ++it)
			{
				if ((*it)->IsDestroyed())
					continue;

				++count;
			}
		}

		return count;
	}

	// Test whether the quadtree is currently enabled for this object manager.
	bool GameObjectManager::IsQuadtreeEnabled() const
	{
		return quadtreeEnabled;
	}

	// Enable or disable use of the quadtree data structure for collision detection.
	void GameObjectManager::SetQuadtreeEnabled(bool enabled)
	{
		quadtreeEnabled = enabled;
	}

	// Perform a raycast and return all objects that intersect with the ray.
	// Params:
	//   start     = The starting point of the ray in world coordinates.
	//   direction = The direction of the ray.
	//   distance  = How far to check in the given direction.
	//   results   = The vector in which the results will be stored.
	//   filter    = Objects with this name are excluded.
	void GameObjectManager::CastRay(const Vector2D& start, const Vector2D& direction,
		float distance, Array<CastResult>& results, const std::string& filter)
	{
		if (!quadtreeEnabled)
		{
			std::cout << "Call to CastRay ignored: please enable quadtree to allow raycasts. "
				<< "Raycasting without a spatial data structure is not recommended." << std::endl;
			return;
		}

		if (quadtree != nullptr)
		{
			LineSegment ray(start, start + direction.Normalized() * distance);
			quadtree->CastRay(ray, results, filter);
		}
	}

	// Perform a raycast and return the closest object the ray hits.
	// Params:
	//   start     = The starting point of the ray in world coordinates.
	//   direction = The direction of the ray.
	//   distance  = How far to check in the given direction.
	//   filter    = Objects with this name are excluded.
	// Returns:
	//   A pointer to the first object hit by the ray.
	GameObject* GameObjectManager::CastRayClosest(const Vector2D& start,
		const Vector2D& direction, float distance, const std::string& filter)
	{
		Array<CastResult> results;
		CastRay(start, direction, distance, results, filter);

		float minT = 2.0f;
		GameObject* closest = nullptr;

		size_t numResults = results.Size();
		for (size_t i = 0; i < numResults; ++i)
		{
			const CastResult& current = results[i];

			if (current.t < minT)
			{
				minT = current.t;
				closest = current.object;
			}
		}

		return closest;
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Update object logic using variable timestep.
	void GameObjectManager::VariableUpdate(float dt)
	{
		// Update active objects
		size_t numObjects = objects.Size();
		for (unsigned i = 0; i < numObjects; ++i)
		{
			objects[i]->Update(dt);
		}
	}

	// Update object physics using fixed timestep.
	void GameObjectManager::FixedUpdate(float dt)
	{
		timeAccumulator += dt;

		while (timeAccumulator >= fixedUpdateDt)
		{
			// Update active objects (physics) with fixed dt
			for (auto it = objects.Begin(); it != objects.End(); ++it)
			{
				(*it)->FixedUpdate(fixedUpdateDt);
			}

			if (!quadtreeEnabled)
			{
				// Check for collisions between objects
				CheckCollisions();
			}
			else
			{
				// Instantiate quadtree
				RemakeQuadtree();
				CheckCollisionsQuadtree();
			}

			// Decrease accumulator
			timeAccumulator -= fixedUpdateDt;
		}
	}

	// Check collisions between pairs of objects.
	void GameObjectManager::CheckCollisions()
	{
		// Make copy of active list to avoid iterator invalidation
		Array<GameObject*> currentObjects(objects);

		// Iterate through all objects
		for (auto it = currentObjects.Begin(); it != currentObjects.End(); ++it)
		{
			GameObject* first = *it;

			// Skip inactive/destroyed
			if (!first->IsActive() || first->IsDestroyed())
				continue;

			Collider* firstCollider = first->GetComponent<Collider>();

			// Skip objects without colliders
			if (!firstCollider) 
				continue;

			for (auto jt = it + 1; jt != currentObjects.End(); ++jt)
			{
				GameObject* second = *jt;

				// Skip inactive/destroyed
				if (!second->IsActive() || second->IsDestroyed())
					continue;

				Collider* secondCollider = second->GetComponent<Collider>();

				// Skip objects without colliders
				if (!secondCollider) 
					continue;

				// Perform collision check and use callbacks if there is a collision
				firstCollider->CheckCollision(*secondCollider);
			}
		}
	}

	// Check collisions using the quadtree.
	void GameObjectManager::CheckCollisionsQuadtree()
	{
		// Refill the quadtree
		PopulateQuadtree();
		size_t numObjects = collidableObjects.Size();

		// Check collision against valid pairs of objects
		for (unsigned i = 0; i < numObjects; ++i)
		{
			// Skip objects marked for destruction
			if (!collidableObjects[i]->IsActive())
				continue;

			Collider* firstCollider = collidableObjects[i]->GetComponent<Collider>();

			// Toggle this collider so we don't accidentally check it against
			// the same objects again.
			firstCollider->SetProcessed(true);

			// Retrieve nearby objects
			Array<GameObject*> nearbyObjects;
			quadtree->RetrieveNearbyObjects(collidableObjects[i], nearbyObjects);

			// For all nearby objects
			size_t numNearbyObjects = nearbyObjects.Size();
			for (unsigned j = 0; j < numNearbyObjects; ++j)
			{
				// Skip objects marked for destruction
				if (!nearbyObjects[j]->IsActive())
					continue;

				Collider* secondCollider = nearbyObjects[j]->GetComponent<Collider>();

				// Skip objects we have already checked
				if (secondCollider->WasProcesed())
					continue;

				// Perform collision check and use callbacks if there is a collision
				firstCollider->CheckCollision(*secondCollider);
			}
		}
	}

	// Destroy any objects marked for destruction.
	void GameObjectManager::DestroyObjects()
	{
		for (auto it = objects.Begin(); it != objects.End(); )
		{
			// Remove destroyed objects
			if ((*it)->IsDestroyed())
			{
				delete* it;
				it = objects.Erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	// Instantiate the quadtree using the current world size.
	void GameObjectManager::RemakeQuadtree()
	{
		Space* space = static_cast<Space*>(GetOwner());
		const BoundingRectangle& screenRect = space->GetCamera().GetScreenWorldDimensions();

		delete quadtree;

		// Create the quadtree
		const unsigned level = 4, objectsPerLevel = 2;
		quadtree = new Quadtree(BoundingRectangle(screenRect.center, screenRect.extents), level, objectsPerLevel);
	}

	// Inserts objects into the quadtree
	void GameObjectManager::PopulateQuadtree()
	{
		if (quadtree == nullptr)
			RemakeQuadtree();

		// Clear contents of quadtree
		quadtree->Clear();

		// Retrieve all objects with colliders
		collidableObjects.Clear();
		GetAllObjectsWithComponent<Collider>(collidableObjects);

		// Add objects with colliders to quadtree
		size_t numObjects = collidableObjects.Size();
		for (unsigned i = 0; i < numObjects; ++i)
		{
			quadtree->AddObject(collidableObjects[i]);
		}
	}

	void SwapGameObjects(GameObject** first, GameObject** second)
	{
		GameObject* temp = *first;
		*first = *second;
		*second = temp;
	}
}
