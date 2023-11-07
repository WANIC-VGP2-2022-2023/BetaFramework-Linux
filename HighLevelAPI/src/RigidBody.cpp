//------------------------------------------------------------------------------
//
// File Name:	RigidBody.cpp
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
#include "RigidBody.h"
#include "Vector2D.h" // Vector2DScaleAdd
#include "Transform.h" // TransformGetTranslation
#include "GameObject.h" // GetComponent

#include "FileStream.h"

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Dynamically allocate a new physics component.
	RigidBody::RigidBody()
		: Component("RigidBody"), angularVelocity(0.0), inverseMass(1.0f), transform(nullptr)
	{
	}

	// Get the acceleration of a physics component.
	// Returns:
	//		A pointer to the component's acceleration structure,
	const Vector2D& RigidBody::GetAcceleration() const
	{
		return acceleration;
	}

	// Get the velocity of a physics component.
	// Returns: 
	//		A pointer to the component's velocity structure,	
	const Vector2D& RigidBody::GetVelocity() const
	{
		return velocity;
	}

	// Get the old translation (position) of a physics component.
	// Returns:
	//		A pointer to the component's oldTranslation structure,
	const Vector2D& RigidBody::GetOldTranslation() const
	{
		return oldTranslation;
	}

	// Get the angular velocity of a physics component.
	// Returns:
	//	 A float representing the new angular velocity.
	float RigidBody::GetAngularVelocity() const
	{
		return angularVelocity;
	}

	// Add a force to the object.
	// Params:
	//	 force = A force vector with direction and magnitude.
	void RigidBody::AddForce(const Vector2D& force)
	{
		forcesSum += force;
	}

	// Set the velocity of a physics component.
	// Params:
	//	 
	//	 velocity = Pointer to a velocity vector.
	void RigidBody::SetVelocity(const Vector2D& velocity_)
	{
		velocity = velocity_;
	}

	void RigidBody::SetVelocityX(float x)
	{
		velocity.x = x;
	}

	void RigidBody::SetVelocityY(float y)
	{
		velocity.y = y;
	}

	// Set the angular velocity of a physics component.
	// Params:
	//	 velocity = New value for the angular velocity.
	void RigidBody::SetAngularVelocity(float velocity_)
	{
		angularVelocity = velocity_;
	}

	// Moves an object to the specified location, syncing
	// its old, current, and new translations to that position.
	void RigidBody::MovePosition(const Vector2D& translation)
	{
		oldTranslation = translation;
		transform->SetTranslation(translation);
	}

	void RigidBody::Initialize()
	{
		transform = GetOwner()->GetComponent<Transform>();
		oldTranslation = transform->GetTranslation();
	}

	void RigidBody::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		// Calculate acceleration
		acceleration = inverseMass * forcesSum;
		// Reset sum of forces
		forcesSum = Vector2D();
	}

	// Update the state of a physics component.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void RigidBody::FixedUpdate(float dt)
	{
		// Calculate new velocity from old velocity and acceleration
		velocity += acceleration * dt;

		// Save old translation
		oldTranslation = transform->GetTranslation();
		// Calculate new translation from old translation and velocity
		Vector2D endTranslation = oldTranslation + velocity * dt;
		// Calculate new rotation from old rotation and angular velocity
		float endRotation = transform->GetRotation() + angularVelocity * dt;

		// Publish results
		transform->SetRotation(endRotation);
		transform->SetTranslation(endTranslation);
	}

	// Save object data to file.
	// Params:
	//   stream = The stream object used to save the object's data.
	void RigidBody::Serialize(FileStream& stream) const
	{
		stream.WriteVariable("velocity", velocity);
		stream.WriteVariable("angularVelocity", angularVelocity);
		stream.WriteVariable("mass", 1.0f / inverseMass);
	}

	// Load object data from file
	// Params:
	//   stream = The stream object used to load the object's data.
	void RigidBody::Deserialize(FileStream & stream)
	{
		stream.ReadVariable("velocity", velocity);
		stream.ReadVariable("angularVelocity", angularVelocity);
		float mass;
		stream.ReadVariable("mass", mass);
		inverseMass = 1.0f / mass;
	}

	// RTTI
	COMPONENT_SUBCLASS_DEFINITION(RigidBody)
}
