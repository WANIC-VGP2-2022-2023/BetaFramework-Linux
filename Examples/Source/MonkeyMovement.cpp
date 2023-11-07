//------------------------------------------------------------------------------
//
// File Name:	MonkeyMovement.cpp
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

#include "stdafx.h"			// Precompiled header
#include "MonkeyMovement.h"	// Class file

// Systems
#include <Input.h>		// CheckHeld, CheckTriggered
#include <FileStream.h>		// WriteVariable, ReadVariable
#include <Space.h>

// Components
#include <GameObject.h>	// GetOwner
#include <Transform.h>	// GetTranslation, SetTranslation
#include <RigidBody.h>	// GetVelocity, SetVelocity, AddForce
#include <Collider.h>	// CollisionEvent
#include <ColliderTilemap.h>

//------------------------------------------------------------------------------

namespace Beta
{
	// Constructor
	MonkeyMovement::MonkeyMovement()
		: Component("MonkeyMovement"), monkeyWalkSpeed(2.5f), monkeyJumpSpeed(7.0f), monkeyFallSpeedMax(-50.0f),
		gravity(0.0f, -10.0f), transform(nullptr), physics(nullptr),
		onGround(false), inLevel1(false)
	{
	}

	// Initialize this component (happens at object creation).
	void MonkeyMovement::Initialize()
	{
		// Store transform and physics
		transform = GetOwner()->GetComponent<Transform>();
		physics = GetOwner()->GetComponent<RigidBody>();

		// Set collision handlers
		RegisterEventHandler(
			GetOwner(), "CollisionStarted", &MonkeyMovement::OnCollisionStarted);
		RegisterEventHandler(
			GetOwner(), "MapCollision", &MonkeyMovement::OnMapCollision);

		if (GetOwner()->GetSpace()->GetLevelName() == "Level:Level1")
		{
			inLevel1 = true;
		}
	}

	// Update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void MonkeyMovement::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		// Fix for Level 1
		if (inLevel1)
		{
			const float groundHeight = -2.0f;
			const Vector2D& position = transform->GetTranslation();
			if (position.y <= groundHeight)
			{
				onGround = true;
				transform->SetTranslation(Vector2D(position.x, groundHeight));
				physics->SetVelocity(Vector2D(physics->GetVelocity().x, 0.0f));
			}
		}

		MoveHorizontal();
		MoveVertical();
	}

	// Write object data to file
	// Params:
	//   stream = The stream that is writing this object to a file.
	void MonkeyMovement::Serialize(FileStream& stream) const
	{
		stream.WriteVariable("monkeyWalkSpeed", monkeyWalkSpeed);
		stream.WriteVariable("monkeyJumpSpeed", monkeyJumpSpeed);
		stream.WriteVariable("gravity", gravity);
	}

	// Read object data from a file
	// Params:
	//   stream = The stream that is reading this object's data from a file.
	void MonkeyMovement::Deserialize(FileStream& stream)
	{
		stream.ReadVariable("monkeyWalkSpeed", monkeyWalkSpeed);
		stream.ReadVariable("monkeyJumpSpeed", monkeyJumpSpeed);
		stream.ReadVariable("gravity", gravity);
	}

	// Determine whether the monkey is on the ground
	bool MonkeyMovement::IsOnGround() const
	{
		return onGround;
	}

	// Moves horizontally based on input
	void MonkeyMovement::MoveHorizontal() const
	{
		//Get the Monkey’s current velocity.
		const Vector2D& velocityOld = physics->GetVelocity();

		// If neither is pressed, set X velocity = 0
		Vector2D velocityNew(0.0f, velocityOld.y);

		//If VK_LEFT is pressed, set X velocity = -monkeyWalkSpeed
		if (EngineGetModule(Input)->CheckHeld('A'))
			velocityNew.x -= monkeyWalkSpeed;

		//If VK_RIGHT is pressed, set X velocity = monkeyWalkSpeed
		if (EngineGetModule(Input)->CheckHeld('D'))
			velocityNew.x += monkeyWalkSpeed;

		// Set the Monkey’s updated velocity
		physics->SetVelocity(velocityNew);
	}

	// Moves vertically based on input
	void MonkeyMovement::MoveVertical()
	{
		const Vector2D& positionOld = transform->GetTranslation();
		Vector2D positionNew = positionOld;

		// Get the Monkey’s current velocity.
		Vector2D velocityNew = physics->GetVelocity();

		// If on ground
		if (onGround)
		{
			// If VK_Up is “triggered”
			if (EngineGetModule(Input)->CheckHeld(VK_SPACE))
			{
				// Jump
				velocityNew.y = monkeyJumpSpeed;
				onGround = false;
				//std::cout << "Jump" << std::endl;
			}

			// If falling (y velocity is less than -10.0f)
			if (velocityNew.y < -10.0f)
			{
				// Not on ground
				onGround = false;
				//std::cout << "Falling" << std::endl;
			}
		}

		// Cap falling speed
		if (velocityNew.y <= monkeyFallSpeedMax)
		{
			velocityNew.y = monkeyFallSpeedMax;
		}

		// Add gravity using physics->AddForce
		physics->AddForce(gravity);

		// Set the Monkey’s updated velocity
		physics->SetVelocity(velocityNew);
	}

	// The collision handling function for Spaceships.
	// Params:
	//	 other = The object the spaceship is colliding with.
	void MonkeyMovement::OnCollisionStarted(const Event& event)
	{
		CollisionEvent& collisionEvent = (CollisionEvent&)event;

		const std::string& objectName = collisionEvent.otherObject.GetName();

		if (objectName == "Coin")
		{
			collisionEvent.otherObject.Destroy();
		}

		if (objectName == "Spikes" || objectName == "Enemy")
		{
			GetOwner()->GetSpace()->RestartLevel();
		}
	}

	// Map collision event handler.
	// Params:
	//	 event = The collision event object.
	void MonkeyMovement::OnMapCollision(const Event& event)
	{
		const MapCollisionEvent& collisionEvent = static_cast<const MapCollisionEvent&>(event);

		// If colliding on bottom
		if (collisionEvent.collisions.bottom == true)
		{
			onGround = true;
		}
	}

	COMPONENT_SUBCLASS_DEFINITION(MonkeyMovement)
}
