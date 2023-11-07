//------------------------------------------------------------------------------
//
// File Name:	Animator.cpp
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
#include "Animator.h"

// Components
#include "Sprite.h" // SetFrame
#include "GameObject.h" // GetComponent

// Resources
#include "Animation.h"

// Systems
#include "FileStream.h"

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Allocate a new animation object.
	Animator::Animator()
		: Component("Animator"), animationIndex(0), playbackSpeed(1.0f),
		isRunning(false), isLooping(false), isDone(false), sprite(nullptr), 
		accumulator(0.0f), currentFrameIndex(0)
	{
	}

	// Initialize components.
	void Animator::Initialize()
	{
		sprite = GetOwner()->GetComponent<Sprite>();

		// Look for animation resources
		if (animationList.IsEmpty() && !animationNames.IsEmpty())
		{
			animationList.Reserve(animationNames.Size());
			for (auto it = animationNames.Begin(); it != animationNames.End(); ++it)
			{
				ConstAnimationPtr animation = ResourceGetAnimation(*it);

				if (animation != nullptr)
					animationList.PushBack(animation);
			}
		}
	}

	// Play an animation.
	// Params:
	//   animationIndex = The index of the animation to play.
	//	 playbackSpeed = The speed at which to play the animation.
	//	 loop = Whether the animation should go back to the beginning after it reaches the end.
	void Animator::Play(size_t animationIndex_, float playbackSpeed_, bool loop_)
	{
		// Animation does not exist - abort!
		if (animationIndex_ >= animationList.Size())
			return;

		playbackSpeed = playbackSpeed_;
		isLooping = loop_;
		isRunning = true;
		isDone = false;
		accumulator = 0.0f;
		currentFrameIndex = 0;

		animationIndex = animationIndex_;
		ConstAnimationPtr animation = animationList[animationIndex];
		
		ConstSpriteSourcePtr animationSource = animation->GetSpriteSource();
		if(animationSource != nullptr)
			sprite->SetSpriteSource(animationSource);
		
		// Set duration
		currentFrameDuration = animation->GetActualFrameDuration(currentFrameIndex);

		// Set frame
		sprite->SetFrame(animation->GetActualFrameIndex(currentFrameIndex));
	}

	// Update the animation.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Animator::Update(float dt)
	{
		if (isRunning == false) 
			return;

		// Not done yet!
		isDone = false;

		// Accumulate time
		if (currentFrameDuration != 0.0f)
			accumulator += dt * playbackSpeed;
		while (accumulator >= currentFrameDuration)
		{
			// Advance to next frame
			accumulator -= currentFrameDuration;
			++currentFrameIndex;

			// Are we at the last frame of animation of animation?
			ConstAnimationPtr animation = animationList[animationIndex];
			if (currentFrameIndex == animation->GetFrameCount() - 1)
			{
				isDone = true;
			}
			else if (currentFrameIndex >= animation->GetFrameCount())
			{
				if (isLooping)
					currentFrameIndex = 0;
				else
					isRunning = false;
			}
			
			// Update duration
			currentFrameDuration = animation->GetActualFrameDuration(currentFrameIndex);
			
			// Fix for instant frame changes
			if(currentFrameDuration == 0.0f)
				break;
		}

		// Set sprite frame
		sprite->SetFrame(animation->GetActualFrameIndex(currentFrameIndex));
	}

	size_t Animator::AddAnimation(ConstAnimationPtr animation)
	{
		animationList.PushBack(animation);
		return animationList.Size() - 1;
	}

	size_t Animator::GetCurrentAnimationIndex() const
	{
		return animationIndex;
	}

	size_t Animator::GetAnimationIndex(const std::string & name_) const
	{
		for (size_t i = 0; i < animationList.Size(); ++i)
		{
			if (animationList[i]->GetName() == name_)
				return i;
		}

		return 0;
	}

	// Determine if the animation has reached the end of its sequence.
	// Returns:
	//	 The value in isDone.
	bool Animator::IsDone() const
	{
		return isDone;
	}

	void Animator::SetPlaybackSpeed(float speed)
	{
		playbackSpeed = speed;
	}

	void Animator::Serialize(FileStream & stream) const
	{
		stream.WriteVariable("playbackSpeed", playbackSpeed);
		stream.WriteVariable("animationIndex", animationIndex);
		stream.WriteVariable("numAnimations", animationList.Size());

		animationNames.Reserve(animationList.Size());
		for (auto it = animationList.Begin(); it != animationList.End(); ++it)
		{
			animationNames.PushBack((*it)->GetName());
		}

		stream.WriteArrayVariable("animationList", animationNames.Data(), animationNames.Size(), true);
	}

	void Animator::Deserialize(FileStream & stream)
	{
		stream.ReadVariable("playbackSpeed", playbackSpeed);
		stream.ReadVariable("animationIndex", animationIndex);
		size_t numAnimations;
		stream.ReadVariable("numAnimations", numAnimations);
		animationNames.Resize(numAnimations);
		stream.ReadArrayVariable("animationList", animationNames.Data(), animationNames.Size());
	}

	// RTTI
	COMPONENT_SUBCLASS_DEFINITION(Animator)
}
