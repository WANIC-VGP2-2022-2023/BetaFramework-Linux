//------------------------------------------------------------------------------
//
// File Name:	Animation.cpp
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
#include "Animation.h"

// Resources
#include <SpriteSource.h>

// Systems
#include <FileStream.h>
#include <EngineCore.h>	// GetFilePath

//------------------------------------------------------------------------------

namespace Beta
{
	AnimationManager Animation::animationManager;

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	AnimationManager::AnimationManager()
		: ResourceManager("Animations/")
	{
	}

	AnimationPtr AnimationManager::Create(const std::string& name)
	{
		// Try to load from file
		AnimationPtr animation = std::make_shared<Animation>();
		std::string filename = GetFilePath() + name + GetFileExtension();
		FileStream stream(filename, OM_Read);
		try
		{
			// Attempt to load animation
			animation->Deserialize(stream);
			
			// Get sprite source if it exists
			animation->spriteSource = ResourceGetSpriteSource(animation->spriteSourceName);
		}
		catch (const FileStreamException & e)
		{
			std::cout << "ERROR in AnimationManager: " << e.what() << std::endl;
			// Destroy the object
			animation.reset();
		}

		return animation;
	}

	Animation::Animation(const std::string& name, ConstSpriteSourcePtr spriteSource,
		unsigned frameCount, unsigned frameStart, float frameDuration)
		: name(name), spriteSource(spriteSource), frameCount(frameCount), frameStart(frameStart),
		frameDuration(frameDuration)
	{
	}
	
	Animation::~Animation()
	{
	}

	unsigned Animation::GetFrameCount() const
	{
		return frameCount;
	}

	unsigned Animation::GetActualFrameIndex(unsigned frameIndex) const
	{
		return frameStart + frameIndex % frameCount;
	}

	float Animation::GetActualFrameDuration(unsigned frameIndex) const
	{
		UNREFERENCED_PARAMETER(frameIndex);
		return frameDuration;
	}

	const std::string& Animation::GetName() const
	{
		return name;
	}

	const ConstSpriteSourcePtr Animation::GetSpriteSource() const
	{
		return spriteSource;
	}

	void Animation::Serialize(FileStream & stream) const
	{
		stream.WriteValue(name);
		stream.BeginScope();

		stream.WriteVariable("frameStart", frameStart);
		stream.WriteVariable("frameCount", frameCount);
		stream.WriteVariable("frameDuration", frameDuration);
		if(spriteSource != nullptr)
			stream.WriteVariable("spriteSource", spriteSource->GetName());
		else
			stream.WriteVariable("spriteSource", "null");

		stream.EndScope();
	}

	void Animation::Deserialize(FileStream & stream)
	{
		stream.ReadValue(name);
		stream.BeginScope();

		stream.ReadVariable("frameStart", frameStart);
		stream.ReadVariable("frameCount", frameCount);
		stream.ReadVariable("frameDuration", frameDuration);
		stream.ReadVariable("spriteSource", spriteSourceName);

		stream.EndScope();
	}

	AnimationManager& Animation::GetAnimationManager()
	{
		return animationManager;
	}
}
