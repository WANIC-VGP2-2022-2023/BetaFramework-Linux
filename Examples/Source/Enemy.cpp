//------------------------------------------------------------------------------
//
// File Name:	Enemy.h
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
#include "Enemy.h"

// Systems
#include "Space.h"
#include <Random.h>
#include <FileStream.h>

// Components
#include "Transform.h"
#include "RigidBody.h"
#include "ColliderTilemap.h"

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Structures:
	//------------------------------------------------------------------------------

	enum EnemyStates
	{
		StateIdle,
		StateWander,
		StateChase,
	};

	// Map collision handler for Monkey objects.
	// Params:
	//   event = The map collision event.
	void Enemy::OnMapCollision(const Event& event)
	{
		const MapCollisionEvent& collisionEvent = static_cast<const MapCollisionEvent&>(event);

		// If not idling and colliding on left or right
		if (currentState != StateIdle && (collisionEvent.collisions.left
			|| collisionEvent.collisions.right))
		{
			float direction = -1.0f;
			float multiplier = 1.0f;
			if (collisionEvent.collisions.left)
				direction = 1.0f;
			if (currentState == StateChase)
				multiplier = chaseMultiplier;

			transform->SetRotation(static_cast<float>(M_PI) * direction);
			physics->SetVelocity(direction * Vector2D(1.0f, 0.0f) * multiplier * baseSpeed);
		}
	}

	// Constructor
	Enemy::Enemy()
		: Component("Enemy"), currentState(StateIdle),
		nextState(StateIdle), innerState(InnerStateEnter), timer(0.0f),
		stateChangeTime(2.0f), baseSpeed(0.5f), chaseMultiplier(2.0f),
		sightRange(2.0f), playerTransform(nullptr)
	{
	}

	// Initialize data for this object.
	void Enemy::Initialize()
	{
		transform = GetOwner()->GetComponent<Transform>();
		physics = GetOwner()->GetComponent<RigidBody>();

		RegisterEventHandler(
			GetOwner(), "MapCollision", &Enemy::OnMapCollision);
	}

	// Update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void Enemy::Update(float dt)
	{
		if (playerTransform == nullptr)
		{
			GameObject* player = GetOwner()->GetSpace()->GetObjectManager().GetObjectByName("Monkey");
			if (player)
				playerTransform = player->GetComponent<Transform>();
		}

		switch (currentState)
		{
		case StateIdle:
			Idle(dt);
			break;
		case StateWander:
			Wander(dt);
			break;
		case StateChase:
			Chase(dt);
			break;
		}
	}

	// Sets the next state of the enemy.
	// Params:
	//   nextState = The next state the enemy should be in.
	void Enemy::SetState(unsigned _nextState)
	{
		nextState = _nextState;
		innerState = InnerStateExit;
		timer = 0.0f;
	}

	void Enemy::Serialize(FileStream & stream) const
	{
		stream.WriteVariable("stateChangeTime", stateChangeTime);
		stream.WriteVariable("baseSpeed", baseSpeed);
		stream.WriteVariable("chaseMultiplier", chaseMultiplier);
		stream.WriteVariable("sightRange", sightRange);
	}

	void Enemy::Deserialize(FileStream & stream)
	{
		stream.ReadVariable("stateChangeTime", stateChangeTime);
		stream.ReadVariable("baseSpeed", baseSpeed);
		stream.ReadVariable("chaseMultiplier", chaseMultiplier);
		stream.ReadVariable("sightRange", sightRange);
	}

	// Idle State
	void Enemy::Idle(float dt)
	{
		switch (innerState)
		{
		case InnerStateEnter:
			// Reset velocity and timer
			physics->SetVelocity(Vector2D());
			timer = 0.0f;

			innerState = InnerStateUpdate;
			break;

		case InnerStateUpdate:
			timer += dt;
			if (timer > stateChangeTime)
				SetState(StateWander);
			break;

		case InnerStateExit:
			currentState = nextState;
			innerState = InnerStateEnter;
			break;
		}
	}

	// Wander State
	void Enemy::Wander(float dt)
	{
		switch (innerState)
		{
		case InnerStateEnter:
		{
			innerState = InnerStateUpdate;

			// Choose direction - 0 is right, 1 is left
			int random = Random::Range(0, 1);
			float direction = -1.0f;
			if (random == 0)
				direction = 1.0f;
			transform->SetRotation(static_cast<float>(M_PI) * random);
			physics->SetVelocity(direction * Vector2D(1.0f, 0.0f) * baseSpeed);
			break;
		}

		case InnerStateUpdate:
			// If player is seen
			if (transform->GetTranslation().Distance(playerTransform->GetTranslation()) < sightRange)
				SetState(StateChase);
			break;

		case InnerStateExit:
			timer += dt;
			if (timer > stateChangeTime)
			{
				currentState = nextState;
				innerState = InnerStateEnter;
			}
			break;
		}
	}

	// Chase State
	void Enemy::Chase(float dt)
	{
		switch (innerState)
		{
		case InnerStateEnter:
		{
			innerState = InnerStateUpdate;

			// Increase speed
			float direction = -1.0f;
			if (playerTransform->GetTranslation().x > transform->GetTranslation().x)
				direction = 1.0f;
			transform->SetRotation(static_cast<float>(M_PI) * (direction == 1.0f ? 0.0f : 1.0f));
			physics->SetVelocity(direction * Vector2D(1.0f, 0.0f) * baseSpeed * chaseMultiplier);
			break;
		}
		case InnerStateUpdate:
			// If player is not seen
			if (transform->GetTranslation().Distance(playerTransform->GetTranslation()) > sightRange)
				SetState(StateIdle);
			break;

		case InnerStateExit:
			timer += dt;
			if (timer > stateChangeTime)
			{
				currentState = nextState;
				innerState = InnerStateEnter;
			}
			break;
		}
	}

	COMPONENT_SUBCLASS_DEFINITION(Enemy)
}

//------------------------------------------------------------------------------
