//------------------------------------------------------------------------------
//
// File Name:	Monkey.cpp
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

#include "stdafx.h"				// Precompiled header
#include "MonkeyAnimation.h"	// Class file

// Systems
#include <Input.h>		// CheckHeld, CheckTriggered
#include <FileStream.h>	// WriteVariable, ReadVariable
#include <GameObject.h>	// GetOwner
#include <Space.h>
#include <ResourceManager.h>

// Components
#include <RigidBody.h>				// GetVelocity, SetVelocity, AddForce
#include <Animator.h>				// Play
#include <Transform.h>				// SetScale
#include "MonkeyMovement.h"			// IsOnGround

//------------------------------------------------------------------------------

namespace Beta
{
	// Constructor
	MonkeyAnimation::MonkeyAnimation()
		: Component("MonkeyAnimation"), walkIndex(0), jumpIndex(0), idleIndex(0),
		currentState(StateIdle), nextState(StateIdle),
		animator(nullptr), physics(nullptr), transform(nullptr),
		monkeyMovement(nullptr)
	{
	}

	// Initialize this component (happens at object creation).
	void MonkeyAnimation::Initialize()
	{
		// Store pointer to physics component
		physics = GetOwner()->GetComponent<RigidBody>();
		transform = GetOwner()->GetComponent<Transform>();
		monkeyMovement = GetOwner()->GetComponent<MonkeyMovement>();
		animator = GetOwner()->GetComponent<Animator>();
		sprite = GetOwner()->GetComponent<Sprite>();

		// Determine indices of animations
		walkIndex = animator->GetAnimationIndex("MonkeyWalk");
		jumpIndex = animator->GetAnimationIndex("MonkeyJump");
		idleIndex = animator->GetAnimationIndex("MonkeyIdle");

		// Start by playing the idle animation
		animator->Play(idleIndex);
	}

	// Update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void MonkeyAnimation::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		ChooseNextState();
		ChangeCurrentState();
		FlipSprite();
	}

	// Choose the correct state based on velocity.
	void MonkeyAnimation::ChooseNextState()
	{
		// Store current velocity from RigidBody component
		const Vector2D& velocity = physics->GetVelocity();

		// Determine which state we should be in based on velocity
		if (!monkeyMovement->IsOnGround())
			nextState = StateJump;
		else if (!AlmostEqual(velocity.x, 0.0f))
			nextState = StateWalk;
		else
			nextState = StateIdle;
	}

	// Change states and start the appropriate animation.
	void MonkeyAnimation::ChangeCurrentState()
	{
		// If we are changing states
		if (currentState != nextState)
		{
			// Switch to next state
			currentState = nextState;
			//std::cout << "State change: " << nextState << std::endl;

			// Switch animations
			switch (currentState)
			{
			case StateIdle:
				animator->Play(idleIndex);
				break;

			case StateWalk:
				animator->Play(walkIndex);
				break;

			case StateJump:
				animator->Play(jumpIndex);
				break;
			}
		}
	}

	// Flip the sprite based on velocity and current state.
	void MonkeyAnimation::FlipSprite() const
	{
		// Store current velocity from RigidBody component
		const Vector2D& velocity = physics->GetVelocity();

		// Flip based on velocity
		if (currentState != StateIdle)
		{
			if (velocity.x < 0.0f)
				sprite->SetFlipX(true);
			else if (velocity.x > 0.0f)
				sprite->SetFlipX(false);
		}
	}

	COMPONENT_SUBCLASS_DEFINITION(MonkeyAnimation)
}
