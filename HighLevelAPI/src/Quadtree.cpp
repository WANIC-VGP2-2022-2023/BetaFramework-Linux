//------------------------------------------------------------------------------
//
// File Name:	Quadtree.cpp
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
#include "Quadtree.h"

// STD
#include <algorithm>        // remove

// Math
#include "Intersection2D.h" // RectRectIntersection

// Systems
#include <EngineCore.h>		// GetModule
#include <DebugDraw.h>		// AddRectangle
#include <GraphicsEngine.h>	// GetDefaultCamera

// Components
#include "GameObject.h"
#include "Transform.h"		// GetBounds
#include "Collider.h"		// IsIntersectingWith

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Private Consts:
	//------------------------------------------------------------------------------

	enum Quadrants : short
	{
		NO_FIT = -2,
		PARTIAL_FIT = -1,
		TOP_RIGHT,
		TOP_LEFT,
		BOTTOM_RIGHT,
		BOTTOM_LEFT,
		NUM_CHILDREN
	};

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Cast result constructors
	CastResult::CastResult()
		: object(nullptr), t(0.0f)
	{
	}

	CastResult::CastResult(GameObject* object, float t)
		: object(object), t(t)
	{
	}

	// Default constructor. Initializes all variables to 0.
	Quadtree::Quadtree()
		: bounds(Vector2D(), Vector2D()), level(0), nodes()
	{
	}

	// Constructor for quadtree.
	// Params:
	//   bounds   = The area encompassed by this node.
	//   level    = The level of this node in the tree.
	//   maxObjectsPerNode = The maximum number of game objects in this node before it will be split.
	Quadtree::Quadtree(const BoundingRectangle& bounds, int level, int maxObjects)
		: bounds(bounds), level(level), nodes()
	{
		// Use objects.Reserve to set the capacity of the object list
		objects.Reserve(maxObjects);

		// Initialize all child node pointers to nullptr
		for (unsigned n = 0; n < NUM_CHILDREN; ++n)
			nodes[n] = nullptr;
	}

	// Destructor for quadtree. Calls Clear function.
	Quadtree::~Quadtree()
	{
		Clear();
	}

	// Draws each of the nodes in the tree (for debug purposes only).
	void Quadtree::Draw() const
	{
		// If we are at leaf node
		if (IsLeafNode())
		{
			DebugDraw& debugDraw = *EngineGetModule(DebugDraw);
			debugDraw.SetCamera(EngineGetModule(GraphicsEngine)->GetDefaultCamera());
			// Add bounds to debug draw usind EngineGetModule(DebugDraw).AddRectangle
			debugDraw.AddRectangle(bounds.center, bounds.extents);
		}
		// Else, for each child node
		else for (unsigned n = 0; n < NUM_CHILDREN; ++n)
		{
			// Call their draw functions
			nodes[n]->Draw();
		}
	}

	// Adds an object to the tree
	// Params:
	//   object = The game object being added to the tree.
	void Quadtree::AddObject(GameObject * object)
	{
		// Get the index of the object
		int index = GetIndex(object->GetComponent<Transform>()->GetBounds());

		// If object is within node bounds (index != NO_FIT)
		if (index != NO_FIT)
		{
			// If we are at a leaf node
			if (IsLeafNode())
			{
				// If we are not at max depth and there are too many objects
				if (objects.Size() == objects.Capacity() && level > 0)
				{
					// Split this node
					Split();
					// Try adding the object again
					AddObject(object);
				}
				// Else, number of objects is okay or we are at max depth, so push object onto list.
				else objects.PushBack(object);
			}
			// Else, we are not at a leaf node
			else
			{
				// If object has total fit, add to specific node.
				if (index != PARTIAL_FIT)
				{
					nodes[index]->AddObject(object);
				}
				// Else, object has partial fit, so add to all nodes.
				else for (unsigned n = 0; n < NUM_CHILDREN; ++n)
				{
					nodes[n]->AddObject(object);
				}
			}
		}
	}

	// Retrieve all objects from the tree that could collide with a given object.
	// Params:
	//   object = The object for which we want to retrieve nearby objects.
	//   results = The list that stores the nearby objects.
	void Quadtree::RetrieveNearbyObjects(GameObject * object, Array<GameObject*> & results)
	{
		// If we are at leaf node
		if (IsLeafNode())
		{
			// Push all objects onto results list
			size_t numObjects = objects.Size();
			for (size_t i = 0; i < numObjects; ++i)
			{
				results.PushBack(objects[i]);
			}
		}
		// Else, we are not at leaf node
		else
		{
			int index = GetIndex(object->GetComponent<Transform>()->GetBounds());

			// If object has total fit (index > PARTIAL_FIT)
			if (index > PARTIAL_FIT)
			{
				// Retrieve objects from corresponding node
				nodes[index]->RetrieveNearbyObjects(object, results);
			}
			// Else if object has partial fit
			else if (index == PARTIAL_FIT)
			{
				// Retrieve objects from all nodes
				nodes[TOP_RIGHT]->RetrieveNearbyObjects(object, results);
				nodes[TOP_LEFT]->RetrieveNearbyObjects(object, results);
				nodes[BOTTOM_RIGHT]->RetrieveNearbyObjects(object, results);
				nodes[BOTTOM_LEFT]->RetrieveNearbyObjects(object, results);
			}
		}
	}

	// Retrieve all objects from the tree that intersect with a given ray.
	// Params:
	//   object  = The object for which we want to retrieve nearby objects.
	//   results = Intersecting objects and their distance from the ray's origin.
	//   filter = Objects with this name are excluded.
	void Quadtree::CastRay(const LineSegment & ray, Array<CastResult> & results, const std::string & filter)
	{
		// If we are at leaf node
		if (IsLeafNode())
		{
			Collider* collider = nullptr;
			float t = 0.0f;

			// Push all objects onto results list
			size_t numObjects = objects.Size();
			for (size_t i = 0; i < numObjects; ++i)
			{
				if (typeid(*objects[i]) != typeid(GameObject))
					std::cout << "WAT" << std::endl;

				collider = objects[i]->GetComponent<Collider>();

				// Ray intersection test
				if (collider && objects[i]->GetName() != filter && collider->IsIntersectingWith(ray, t))
				{
					results.PushBack(CastResult(objects[i], t));
				}
			}
		}
		// Else, we are not at leaf node
		else
		{
			// Create ray bounds
			Vector2D center = ray.start.Midpoint(ray.end);
			Vector2D extents = (ray.end - ray.start) / 2.0f;
			extents.x = fabsf(extents.x); extents.y = fabsf(extents.y);
			BoundingRectangle rayBounds = BoundingRectangle(center, extents);

			int index = GetIndex(rayBounds);

			// If object has total fit (index > PARTIAL_FIT)
			if (index > PARTIAL_FIT)
			{
				// Retrieve objects from corresponding node
				nodes[index]->CastRay(ray, results, filter);
			}
			// Else if object has partial fit
			else if (index == PARTIAL_FIT)
			{
				// Retrieve objects from all nodes
				nodes[TOP_RIGHT]->CastRay(ray, results, filter);
				nodes[TOP_LEFT]->CastRay(ray, results, filter);
				nodes[BOTTOM_RIGHT]->CastRay(ray, results, filter);
				nodes[BOTTOM_LEFT]->CastRay(ray, results, filter);
			}
		}
	}

	// Remove all objects from the tree.
	void Quadtree::Clear()
	{
		// If we are at a leaf node
		if (IsLeafNode())
			// Clear objects list
			objects.Clear();
		else
		{
			// For each node
			for (int i = 0; i < NUM_CHILDREN; i++)
			{
				// Delete
				delete nodes[i];
				// Set to null
				nodes[i] = nullptr;
			}
		}
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Splits the contents of a node into four child nodes.
	void Quadtree::Split()
	{
		// Create variables for width and height of sub-node.
		// Width and height of sub nodes will be half of the extents of the current node.
		Vector2D subExtents = Vector2D(bounds.extents.x / 2.0f, bounds.extents.y / 2.0f);
		Vector2D subCenter;

		// Create the four new nodes with centers
		subCenter = Vector2D(bounds.center.x + subExtents.x, bounds.center.y + subExtents.y);
		nodes[TOP_RIGHT] = new Quadtree(BoundingRectangle(subCenter, subExtents), level - 1, (unsigned)objects.Capacity());

		subCenter = Vector2D(bounds.center.x - subExtents.x, bounds.center.y + subExtents.y);
		nodes[TOP_LEFT] = new Quadtree(BoundingRectangle(subCenter, subExtents), level - 1, (unsigned)objects.Capacity());

		subCenter = Vector2D(bounds.center.x + subExtents.x, bounds.center.y - subExtents.y);
		nodes[BOTTOM_RIGHT] = new Quadtree(BoundingRectangle(subCenter, subExtents), level - 1, (unsigned)objects.Capacity());

		subCenter = Vector2D(bounds.center.x - subExtents.x, bounds.center.y - subExtents.y);
		nodes[BOTTOM_LEFT] = new Quadtree(BoundingRectangle(subCenter, subExtents), level - 1, (unsigned)objects.Capacity());

		// For each object in this node
		for (auto it = objects.Begin(); it != objects.End(); ++it)
		{
			// Get the index of the object
			int index = GetIndex((*it)->GetComponent<Transform>()->GetBounds());

			if (typeid(**it) != typeid(GameObject))
				std::cout << "WAT" << std::endl;

			// Fits - add to specific node.
			if (index != PARTIAL_FIT)
				nodes[index]->AddObject(*it);
			// Partial fit - add to all nodes.
			else for (unsigned n = 0; n < NUM_CHILDREN; ++n)
				nodes[n]->AddObject(*it);
		}

		// Remove all objects so that objects only exist in leaf nodes.
		objects.Clear();
	}

	// Determines which of the four child nodes that an object belongs in.
	// Params:
	//   object = The object for which we need to find the index.
	Quadrants Quadtree::GetIndex(const BoundingRectangle & object)
	{
		// If object is not within bounds of this node
		if (Intersection2D::RectangleRectangleIntersection(bounds, object) != true)
			// Return NO_FIT
			return NO_FIT;

		// Now we know that fit is at least partial
		Quadrants index = PARTIAL_FIT;

		// Determine whether object can completely fit within the top quadrants
		bool topQuadrant = object.bottom > bounds.center.y;

		// Determine whether object can completely fit within the bottom quadrants
		bool bottomQuadrant = object.top < bounds.center.y;

		// If object can completely fit within the left quadrants
		if (object.right < bounds.center.x)
		{
			// and top quadrants
			if (topQuadrant)
				// Index is top left
				index = TOP_LEFT;
			// and bottom quadrants
			else if (bottomQuadrant)
				// Index is bottom left
				index = BOTTOM_LEFT;
		}
		// Else if object can completely fit within the right quadrants
		else if (object.left > bounds.center.x)
		{
			// and top quadrants
			if (topQuadrant)
				// Index is top right
				index = TOP_RIGHT;
			// and bottom quadrants
			else if (bottomQuadrant)
				// Index is bottom right
				index = BOTTOM_RIGHT;
		}

		return index;
	}

	// Determines whether this node can contain objects.
	bool Quadtree::IsLeafNode() const
	{
		return nodes[TOP_RIGHT] == nullptr;
	}
}
