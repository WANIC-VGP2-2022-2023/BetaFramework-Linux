//------------------------------------------------------------------------------
//
// File Name:	PlayerShip.cpp
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
#include "PlayerShip.h"

// Systems
#include <Input.h>		// CheckHeld
#include <Space.h>		// GetObjectManager
#include <GameObjectManager.h> // GetArchetype
#include <Random.h>		// Random::Range
#include <FileStream.h>

// Components
#include <GameObject.h> // GetComponent
#include <RigidBody.h>	// GetVelocity, SetVelocity
#include <Transform.h>	// GetRotation
#include <Sprite.h>		// SetColor
#include <Collider.h>	// CollisionEvent
#include "PlayerProjectile.h"	// SetSpawner

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	// Params:
	//   forwardThrust = Amount of force added to the ship when thrusters are activated.
	//   maximumSpeed  = Maximum attainable speed of the ship.
	//   rotationSpeed = Speed at which the ship rotates.
	//   bulletSpeed   = Speed at which bullets move when fired by ship.
	//   deathDuration = Length of death "animation" in seconds.
	PlayerShip::PlayerShip(float forwardThrust, float rotationSpeed,
		float bulletSpeed, float deathDuration, int spreadNumber, float spreadAngle)
		: Component("PlayerShip"), forwardThrust(forwardThrust),
		rotationSpeed(rotationSpeed), bulletSpeed(bulletSpeed), deathDuration(deathDuration),
		spreadNumber(spreadNumber), spreadAngle(spreadAngle), timer(0.0f), isDying(false),
		invincible(false)
	{
	}

	// Initialize this component (happens at object creation).
	void PlayerShip::Initialize()
	{
		// Get components
		transform = GetOwner()->GetComponent<Transform>();
		physics = GetOwner()->GetComponent<RigidBody>();

		// Get bullet archetype
		bulletArchetype = ResourceGetArchetype("Bullet");

		// Reset score
		score = 0;

		// Set collision handlers
		RegisterEventHandler(GetOwner(), "CollisionStarted", &PlayerShip::OnCollisionStarted);
	}

	// Fixed update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void PlayerShip::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		if (!isDying)
		{
			Move();
			Rotate();

			if (EngineGetModule(Input)->CheckTriggered(VK_SPACE))
				Shoot();
		}
		else
		{
			DeathSequence(dt);
		}

		// Cheat codes
		if (EngineGetModule(Input)->CheckTriggered('I'))
		{
			invincible = !invincible;
		}
	}

	// Write object data to file
	// Params:
	//   stream = The stream that is writing this object to a file.
	void PlayerShip::Serialize(FileStream& stream) const
	{
		stream.WriteVariable("forwardThrust", forwardThrust);
		stream.WriteVariable("rotationSpeed", rotationSpeed);
		stream.WriteVariable("bulletSpeed", bulletSpeed);
		stream.WriteVariable("deathDuration", deathDuration);
		stream.WriteVariable("spreadNumber", spreadNumber);
		stream.WriteVariable("spreadAngle", spreadAngle);
	}

	// Read object data from a file
	// Params:
	//   stream = The stream that is reading this object's data from a file.
	void PlayerShip::Deserialize(FileStream& stream)
	{
		stream.ReadVariable("forwardThrust", forwardThrust);
		stream.ReadVariable("rotationSpeed", rotationSpeed);
		stream.ReadVariable("bulletSpeed", bulletSpeed);
		stream.ReadVariable("deathDuration", deathDuration);
		stream.ReadVariable("spreadNumber", spreadNumber);
		stream.ReadVariable("spreadAngle", spreadAngle);
	}

	// Returns player's score
	unsigned PlayerShip::GetScore() const
	{
		return score;
	}

	void PlayerShip::IncreaseScore(unsigned amount)
	{
		score += amount;
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Move forward when up arrow is pressed
	void PlayerShip::Move() const
	{
		// Add forward thrust
		if (EngineGetModule(Input)->CheckHeld('W'))
		{
			Vector2D direction = Vector2D::FromAngleRadians(transform->GetRotation());
			physics->AddForce(direction * forwardThrust);
		}

		// Add drag
		float speed = physics->GetVelocity().Magnitude();
		if (speed != 0.0f)
		{
			Vector2D direction = physics->GetVelocity() / speed;
			physics->AddForce(-direction * speed);
		}
	}

	// Rotate when left or right arrow key is pressed
	void PlayerShip::Rotate() const
	{
		float rotationDirection = 0;

		// If left is held
		if (EngineGetModule(Input)->CheckHeld('A'))
			// Rotate clockwise
			rotationDirection += 1;

		// If right is held
		if (EngineGetModule(Input)->CheckHeld('D'))
			// Rotate counter-clockwise
			rotationDirection -= 1;

		//if(rotationDirection != 0)
		physics->SetAngularVelocity(rotationDirection * rotationSpeed);
		//else
			//physics->SetAngularVelocity(physics->GetAngularVelocity() * 0.98f);
	}

	// Shoot projectiles when space is pressed
	void PlayerShip::Shoot()
	{
		for (int i = 0; i < spreadNumber; ++i)
		{
			// Create a copy of the bullet archetype
			GameObject* bulletClone = new GameObject(bulletArchetype);

			// Create direction vector from ship's rotation
			int offsetIndex = -(spreadNumber / 2) + i;
			float bulletAngle = transform->GetRotation() + offsetIndex * spreadAngle;
			Vector2D direction = Vector2D::FromAngleRadians(bulletAngle);

			// Set bullet transform to position slightly in front of ship
			Transform * bulletTransform = bulletClone->GetComponent<Transform>();
			bulletTransform->SetTranslation(transform->GetTranslation() +
				direction * transform->GetScale().x / 2.0f);

			// Set direction and speed of bullet based on ship orientation
			bulletTransform->SetRotation(transform->GetRotation());
			RigidBody * bulletPhysics = bulletClone->GetComponent<RigidBody>();
			bulletPhysics->SetVelocity(direction * bulletSpeed + physics->GetVelocity() * 0.5f);

			// Set player projectile spawner variable to this object
			PlayerProjectile * projectile = bulletClone->GetComponent<PlayerProjectile>();
			projectile->SetSpawner(this);

			// Add bullet to object manager
			GetOwner()->GetSpace()->GetObjectManager().AddObject(*bulletClone);
		}
	}

	// Play death "animation"
	void PlayerShip::DeathSequence(float dt)
	{
		// Death Effect
		Sprite* sprite = GetOwner()->GetComponent<Sprite>();
		int time = (int)(timer * 3.0f) % 2;
		if (time == 0)
			sprite->SetAlpha(sprite->GetAlpha() - 8.0f * dt);
		else if (time == 1)
			sprite->SetAlpha(sprite->GetAlpha() + 8.0f * dt);

		// Restart when death effect is done
		timer -= dt;
		if (timer <= 0.0f)
			GetOwner()->GetSpace()->RestartLevel();
	}

	// The collision handling function for Spaceships.
	// Params:
	//	 other = The object the spaceship is colliding with.
	void PlayerShip::OnCollisionStarted(const Event & event)
	{
		CollisionEvent& collisionEvent = (CollisionEvent&)event;

		if (collisionEvent.otherObject.GetName() == "Asteroid")
		{
			if (isDying || invincible)
				return;

			isDying = true;
			timer = deathDuration;

			// Set random angular velocity
			float angularVelocity = physics->GetAngularVelocity();

			if (angularVelocity < 0.0f)
				angularVelocity = Random::Range(3.0f, 6.0f);
			else
				angularVelocity = -Random::Range(3.0f, 6.0f);

			physics->SetAngularVelocity(angularVelocity);
		}
	}

	COMPONENT_SUBCLASS_DEFINITION(PlayerShip)
}

//------------------------------------------------------------------------------
