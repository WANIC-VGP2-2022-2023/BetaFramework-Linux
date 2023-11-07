//------------------------------------------------------------------------------
//
// File Name:	Physics.c
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 4
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Physics.h"
#include "Stream.h"
#include "Transform.h"
#include "Vector2D.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct Physics
{
	// Previous position.  May be used for resolving collisions.
	Vector2D	oldTranslation;

	// Acceleration = inverseMass * (sum of forces)
	Vector2D	acceleration;

	// Velocity may be stored as a direction vector and speed scalar, instead.
	Vector2D	velocity;

	// Rotational velocity (in radians).
	float rotationalVelocity;

	// Drag to be applied to moving objects.
	float drag;

	// Used when calculating acceleration due to forces.
	// Used when resolving collision between two dynamic objects.
	//float		inverseMass;

} Physics;

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new Physics component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
PhysicsPtr PhysicsCreate(void)
{
	// Allocate the memory for the animation.
	PhysicsPtr physics = calloc(1, sizeof(Physics));

	if (physics)
	{
		// Initialize any non-zero member variables.
		//physics->inverseMass = 1.0f;
	}

	return physics;
}

// Dynamically allocate a clone of an existing physics component.
// (Hint: Perform a shallow copy of the member variables.)
// Params:
//	 other = Pointer to the component to be cloned.
// Returns:
//	 If 'other' is valid and the memory allocation was successful,
//	   then return a pointer to the cloned component,
//	   else return NULL.
PhysicsPtr PhysicsClone(const PhysicsPtr other)
{
	// Validate the pointer.
	if (other)
	{
		// Allocate the memory for the animation.
		PhysicsPtr physics = calloc(1, sizeof(Physics));

		if (physics)
		{
			*physics = *other;
		}
		return physics;
	}
	return NULL;
}

// Free the memory associated with a Physics component.
// (Also, set the physics pointer to NULL.)
// Params:
//	 physics = Pointer to the Physics component pointer.
void PhysicsFree(PhysicsPtr * physics)
{
	// Verify that a valid pointer was specified.
	if (physics)
	{
		// Free the allocated memory.
		free(*physics);

		// Set the pointer to NULL.
		*physics = NULL;
	}
}

// Read the properties of a Physics component from a file.
// [NOTE: Read the acceleration and velocity values using StreamReadVector2D.]
// Params:
//	 physics = Pointer to the Physics component.
//	 stream = Pointer to the data stream used for reading.
void PhysicsRead(PhysicsPtr physics, Stream stream)
{
	if (physics && stream)
	{
		StreamReadVector2D(stream, &(physics->acceleration));
		StreamReadVector2D(stream, &(physics->velocity));
		//physics->inverseMass = StreamReadFloat(stream);
	}
}

// Get the acceleration of a Physics component.
// Params:
//	 physics = Pointer to the Physics component.
// Returns:
//	 If the physics pointer is valid,
//		then return a pointer to the component's acceleration structure,
//		else return a NULL pointer.
const Vector2D * PhysicsGetAcceleration(const PhysicsPtr physics)
{
	// Validate the pointer.
	if (physics)
	{
		return &(physics->acceleration);
	}
	return NULL;
}

// Get the velocity of a Physics component.
// Params:
//	 physics = Pointer to the Physics component.
// Returns:
//	 If the physics pointer is valid,
//		then return a pointer to the component's velocity structure,
//		else return a NULL pointer.
const Vector2D * PhysicsGetVelocity(const PhysicsPtr physics)
{
	// Validate the pointer.
	if (physics)
	{
		return &(physics->velocity);
	}
	return NULL;
}

// Get the rotational velocity of a physics component.
// Params:
//	 physics = Pointer to the physics component.
// Returns:
//	 If the physics pointer is valid,
//		then return the component's rotational velocity value,
//		else return 0.0f.
float PhysicsGetRotationalVelocity(PhysicsPtr physics)
{
	// Validate the pointer.
	if (physics)
	{
		return physics->rotationalVelocity;
	}
	return 0.0f;
}

// Get the old translation (position) of a Physics component.
// Params:
//	 physics = Pointer to the Physics component.
// Returns:
//	 If the physics pointer is valid,
//		then return a pointer to the component's oldTranslation structure,
//		else return a NULL pointer.
const Vector2D * PhysicsGetOldTranslation(PhysicsPtr physics)
{
	// Validate the pointer.
	if (physics)
	{
		// Get the physics component's old translation.
		return &(physics->oldTranslation);
	}
	return NULL;
}

// Set the acceleration of a Physics component.
// Params:
//	 physics = Pointer to the Physics component.
//	 acceleration = Pointer to an acceleration vector.
void PhysicsSetAcceleration(PhysicsPtr physics, const Vector2D * acceleration)
{
	// Validate the pointers.
	if (physics && acceleration)
	{
		// Set the physics component's new acceleration.
		physics->acceleration = *acceleration;
	}
}

// Set the velocity of a Physics component.
// Params:
//	 physics = Pointer to the Physics component.
//	 velocity = Pointer to a velocity vector.
void PhysicsSetVelocity(PhysicsPtr physics, const Vector2D * velocity)
{
	// Validate the pointers.
	if (physics && velocity)
	{
		// Set the physics component's new velocity.
		physics->velocity = *velocity;
	}
}

// Set the rotational velocity of a physics component.
// Params:
//	 physics = Pointer to the physics component.
//	 rotationalVelocity = The new rotational velocity.
void PhysicsSetRotationalVelocity(PhysicsPtr physics, float rotationalVelocity)
{
	// Validate the pointers.
	if (physics)
	{
		// Set the physics component's new velocity.
		physics->rotationalVelocity = rotationalVelocity;
	}
}

// Set the "drag" value of a physics component.
// Params:
//	 physics = Pointer to the physics component.
//	 drag = The new drag value.
void PhysicsSetDrag(PhysicsPtr physics, float drag)
{
	// Validate the pointer.
	if (physics)
	{
		physics->drag = min(max(0.0f, drag), 1.0f);
	}
}

// Update the state of a Physics component using the Semi-Implicit Euler method,
//	 as outlined in the "Dynamics" lecture slides.
// (NOTE: This function must verify that the physics and transform pointers are valid.)
// Params:
//	 physics = Pointer to the Physics component.
//	 transform = Pointer to the associated transform component.
//	 dt = Change in time (in seconds) since the last game loop.
void PhysicsUpdate(PhysicsPtr physics, TransformPtr transform, float dt)
{
	// Validate the pointers.
	if (physics && transform)
	{
		// Set oldTranslation = translation.
		Vector2D translation = *TransformGetTranslation(transform);
		physics->oldTranslation = translation;

		float rotation = TransformGetRotation(transform);
		rotation += physics->rotationalVelocity * dt;
		TransformSetRotation(transform, rotation);

		// Set velocity = acceleration * dt
		physics->velocity = Vector2DScaleAdd(&physics->velocity, &physics->acceleration, dt);

		// Set position = velocity * dt
		physics->velocity = Vector2DScale(&physics->velocity, (1.0f - physics->drag));
		translation = Vector2DScaleAdd(&translation, &physics->velocity, dt);
		TransformSetTranslation(transform, &translation);
	}
}

//------------------------------------------------------------------------------
