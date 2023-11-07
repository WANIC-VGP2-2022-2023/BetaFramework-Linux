//------------------------------------------------------------------------------
//
// File Name:	Animation.c
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 3
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Animation.h"
#include "GameObject.h"
//#include "AnimationSequence.h"
#include "Sprite.h"
#include "Stream.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct Animation
{
	// Pointer to the parent game object associated with the animation.
	GameObjectPtr parent;

	// The current frame being displayed.
	unsigned int frameIndex;

	// The maximum number of frames in the sequence.
	unsigned int frameCount;

	// The time remaining for the current frame.
	float frameDelay;

	// The amount of time to display each successive frame.
	float frameDuration;

	// True if the animation is running; false if the animation has stopped.
	bool isRunning;

	// True if the animation loops back to the beginning.
	bool isLooping;

	// True if the end of the animation has been reached, false otherwise.
	// (Hint: This should be true for only one game loop.)
	bool isDone;

	// The animation sequence currently being played, if any.
	//AnimationSequencePtr	sequence;

} Animation;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

void AnimationAdvanceFrame(AnimationPtr animation);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new animation component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// Params:
//	 sprite = Pointer to the sprite object associated with the animation.
AnimationPtr AnimationCreate(void)
{
	// Allocate the memory for the animation.
	AnimationPtr animation = calloc(1, sizeof(Animation));

	if (animation)
	{
		// Initialize any non-zero member variables.
	}

	// Return the resulting animation.
	return animation;
}

// Dynamically allocate a clone of an existing animation.
// (Hint: Perform a shallow copy of the member variables.)
// Params:
//	 other = Pointer to the component to be cloned.
// Returns:
//	 If 'other' is valid and the memory allocation was successful,
//	   then return a pointer to the cloned component,
//	   else return NULL.
AnimationPtr AnimationClone(const AnimationPtr other)
{
	// Validate the pointer.
	if (other)
	{
		// Allocate the memory for the animation.
		AnimationPtr animation = calloc(1, sizeof(Animation));

		if (animation)
		{
			// Initialize any non-zero member variables.
			//animation->frameIndex = other->frameIndex;
			*animation = *other;
		}

		// Return the resulting animation.
		return animation;
	}
	return NULL;
}

// Free the memory associated with an animation.
// (Also, set the animation pointer to NULL.)
// Params:
//	 animation = Pointer to the animation pointer.
void AnimationFree(AnimationPtr * animation)
{
	// Verify that a valid animation was specified.
	if (animation)
	{
		// Free the allocated memory.
		free(*animation);

		// Set the animation pointer to NULL.
		*animation = NULL;
	}
}

// Read the properties of an Animation component from a file.
// [NOTE: Read the integer values for frameIndex and frameCount.]
// [NOTE: Read the float values for frameDelay and frameDuration.]
// [NOTE: Read the boolean values for isRunning and isLooping.]
// Params:
//	 animation = Pointer to the Animation component.
//	 stream = Pointer to the data stream used for reading.
void AnimationRead(AnimationPtr animation, Stream stream)
{
	if (animation && stream)
	{
		// The current frame being displayed.
		animation->frameIndex = StreamReadInt(stream);

		// The maximum number of frames in the sequence.
		animation->frameCount = StreamReadInt(stream);

		// The time remaining for the current frame.
		animation->frameDelay = StreamReadFloat(stream);

		// The amount of time to display each successive frame.
		animation->frameDuration = StreamReadFloat(stream);

		// True if the animation is running; false if the animation has stopped.
		animation->isRunning = StreamReadBoolean(stream);

		// True if the animation loops back to the beginning.
		animation->isLooping = StreamReadBoolean(stream);
	}
	
	return;
}

// Set the parent game object for an animation component.
// Params:
//	 animation = Pointer to the animation pointer.
//	 gameObject = Pointer to the parent game object.
void AnimationSetParent(AnimationPtr animation, GameObjectPtr parent)
{
	if (animation)
	{
		animation->parent = parent;
	}
}


// Play a simple animation sequence (0 .. N).
// (Hint: Make sure to initialize all variables, except for "sprite".)
// Params:
//	 animation = Pointer to the animation object.
//	 frameCount = The number of frames in the sequence.
//	 frameDelay = The amount of time to wait between frames (in seconds).
//	 isLooping = True if the animation loops, false otherwise.
void AnimationPlay(AnimationPtr animation, int frameCount, float frameDuration, bool isLooping)
{
	// Verify that the animation pointer is not NULL.
	if (animation)
	{
		// Clear out any existing animation sequence.
		//animation->sequence = NULL;

		// Initiate a simple animation sequence (0 - N).
		animation->frameIndex = 0;
		animation->frameCount = frameCount;
		animation->frameDelay = frameDuration;
		animation->frameDuration = frameDuration;
		animation->isRunning = true;
		animation->isLooping = isLooping;
		animation->isDone = false;

		if (animation->parent)
		{
			SpritePtr sprite = GameObjectGetSprite(animation->parent);
			if (sprite)
			{
				// Set the sprite to the first frame of the animation.
				SpriteSetFrame(sprite, animation->frameIndex);
			}
		}
	}
}

// Play a complex animation sequence (any frame/any delay).
// (Hint: Make sure to initialize all variables, except for "sprite".)
// Params:
//	 animation = Pointer to the animation object.
//	 sequence = Pointer to the animation sequence to be played.
//void AnimationPlaySequence(AnimationPtr animation, AnimationSequencePtr sequence)
//{
//	// Verify that the animation pointer is not NULL.
//	if (animation && animation->sprite)
//	{
//		// Store a pointer to the new animation sequence.
//		animation->sequence = sequence;
//
//		// Initialize the animation data using information from the animation sequence.
//		animation->frameIndex = 0;
//		const AnimationFrame * frameInfo = AnimationSequenceGetFrame(sequence, animation->frameIndex);
//		animation->frameDelay = frameInfo->frameDuration;
//		animation->frameCount = AnimationSequenceGetFrameCount(sequence);
//		animation->isRunning = true;
//		animation->isLooping = AnimationSequenceIsLooping(sequence);
//		animation->isDone = false;
//
//		// Set the sprite's frame using the frame index from the animation sequence.
//		SpriteSetFrame(animation->sprite, frameInfo->frameIndex);
//	}
//}

// Update the animation.
// Params:
//	 animation = Pointer to the animation object.
//	 dt = Change in time (in seconds) since the last game loop.
void AnimationUpdate(AnimationPtr animation, float dt)
{
	// Verify that the animation pointer is valid.
	if (animation)
	{
		// Set the isDone flag to false.
		animation->isDone = false;

		// Check if the animation is running.
		if (animation->isRunning)
		{
			// Update the frameDelay (frameDelay -= dt).
			animation->frameDelay -= dt;

			// While frameDelay is less than or equal to zero, then advance the animation.
			// (A while-loop is used to skip frames when a very large 'dt' occurs.)
			if (animation->frameDelay <= 0)
			{
				// Advance the animation to the next frame, handling looping, if necessary.
				AnimationAdvanceFrame(animation);
			}
		}
	}
}

// Determine if the animation has reached the end of its sequence.
// Params:
//	 animation = Pointer to the animation object.
// Returns:
//	 If the animation pointer is valid,
//		then return the value in isDone,
//		else return false.
bool AnimationIsDone(AnimationPtr animation)
{
	// Verify that a valid animation was specified.
	if (animation)
	{
		return animation->isDone;
	}

	// Return false if no sequence was specified.
	return false;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Advance the animation to the next frame, handling looping, if necessary.
// Params:
//	 animation = Pointer to the animation object.
void AnimationAdvanceFrame(AnimationPtr animation)
{
	// Verify that the animation and sprite pointers are valid.
	if (animation)
	{
		// Update the animation's frame index.
		++animation->frameIndex;

		// Check if the end of the animation has been reached.
		if (animation->frameIndex >= animation->frameCount)
		{
			// If looping is enabled,
			//   then go to the first frame (0),
			//   else go back to the last frame.
			if (animation->isLooping)
			{
				animation->frameIndex = 0;
			}
			else
			{
				animation->frameIndex = animation->frameCount - 1;
				animation->isRunning = false;
			}

			// Set the isDone flag to true.
			animation->isDone = true;
		}

		if (animation->isRunning)
		{
			if (animation->parent)
			{
				SpritePtr sprite = GameObjectGetSprite(animation->parent);
				if (sprite)
				{
					// Set the sprite to the first frame of the animation.
					SpriteSetFrame(sprite, animation->frameIndex);
				}
			}

			// If an animation sequence is playing (AnimationPlaySequence).
			//if (animation->sequence)
			//{
			//	// Get the animation frame info associated with the current frameIndex.
			//	const AnimationFrame * frameInfo = AnimationSequenceGetFrame(animation->sequence, animation->frameIndex);
			//	animation->frameDuration = frameInfo->frameDuration;
			//	// Set the sprite's new frame index.
			//	SpriteSetFrame(animation->sprite, frameInfo->frameIndex);
			//}
			//// else - A simple animation is playing (AnimationPlay).
			//else
			//{
			//	// Set the sprite's new frame index.
			//	SpriteSetFrame(animation->sprite, animation->frameIndex);
			//}
		}

		// Update the animation's frame delay (frameDelay += frameDuration).
		animation->frameDelay += animation->frameDuration;
	}
}
