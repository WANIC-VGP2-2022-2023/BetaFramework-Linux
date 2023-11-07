//------------------------------------------------------------------------------
//
// File Name:	Animation.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Serializable.h"

#include "ResourceManager.h"

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Forward References:
	//------------------------------------------------------------------------------

	class SpriteSource;
	typedef std::shared_ptr<const SpriteSource> ConstSpriteSourcePtr;

	class Animation;
	typedef std::shared_ptr<Animation> AnimationPtr;

	//------------------------------------------------------------------------------
	// Public Structures:
	//------------------------------------------------------------------------------

	class AnimationManager : public ResourceManager<Animation>
	{
	public:
		BE_HL_API AnimationManager();
	private:
		AnimationPtr Create(const std::string& name) override;
	};

	class Animation : public Serializable
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Construct a new animation resource.
		// Params:
		//   name = The name of the animation.
		//   spriteSource = The sprite source used by this animation.
		//   frameCount = The number of frames in this animation.
		//   frameStart = The starting frame in the sprite source for this animation.
		//   duration = The time that each frame will be displayed.
		BE_HL_API  Animation(const std::string& name = "", ConstSpriteSourcePtr spriteSource = nullptr,
			unsigned frameCount = 1, unsigned frameStart = 0, float frameDuration = 0.0f);

		// Destructor - defined to support inheritance.
		BE_HL_API virtual ~Animation();

		// Return the total number of frames in the animation
		BE_HL_API unsigned GetFrameCount() const;

		// Return the frame index that a sprite should use. This is based on the
		// sprite's current frame (given as a parameter), the animation's start frame, 
		// and the animation's frame count.
		// Params:
		//   frameIndex = The current frame in he animation, zero-based.
		// Returns:
		//   The frame in the sprite source for the current frame of the animation.
		BE_HL_API virtual unsigned GetActualFrameIndex(unsigned frameIndex) const;

		// Return the duration of the current frame
		// Params:
		//   frameIndex = The current frame in he animation, zero-based.
		// Returns:
		//   The duration of the current frame in the animation.
		BE_HL_API virtual float GetActualFrameDuration(unsigned frameIndex) const;

		// Get the name of the animation.
		BE_HL_API const std::string& GetName() const;

		// Get the sprite source used by this animation.
		BE_HL_API const ConstSpriteSourcePtr GetSpriteSource() const;

		// Save object data to a stream.
		// Params:
		//   stream = The stream object used to save the object's data.
		BE_HL_API void Serialize(FileStream& stream) const;

		// Load object data from a stream.
		// Params:
		//   stream = The stream object used to load the object's data.
		BE_HL_API void Deserialize(FileStream& stream);

		// Use this to manage sprite source resources!
		friend class AnimationManager;
		BE_HL_API static AnimationManager& GetAnimationManager();

	private:
		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// The amount of time to display each frame.
		float frameDuration;

		// Animation attributes
		unsigned frameCount;
		unsigned frameStart;

		// Name of animation
		std::string name;

		// Sprite source used for this animation.
		ConstSpriteSourcePtr spriteSource;
		// Name of the sprite source (for serialization)
		std::string spriteSourceName;

		static AnimationManager animationManager;
	};

	#define ResourceGetAnimation(a) Animation::GetAnimationManager().GetResource(a)
}

//----------------------------------------------------------------------------
