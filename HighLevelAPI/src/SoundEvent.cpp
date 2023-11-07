//------------------------------------------------------------------------------
//
// File Name:	SoundEvent.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "SoundEvent.h"

#include <fmod_errors.h> // FMOD_ErrorString
#include <fmod_studio.hpp> // EventInstance

//------------------------------------------------------------------------------

using FMOD::Studio::EventInstance;

namespace Beta
{
	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	namespace
	{
		void FMOD_Assert(FMOD_RESULT result)
		{
			if (result != FMOD_OK)
			{
				std::cout << "FMOD error! (" << result << ") " << FMOD_ErrorString(result) << std::endl;
				//exit(-1);
			}
		}
	}

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	// Params:
	//   data = Internal event data.
	SoundEvent::SoundEvent(void* data)
		: data(data)
	{
	}

	// Destructor
	SoundEvent::~SoundEvent()
	{
		EventInstance* eventInstance = static_cast<EventInstance*>(data);
		FMOD_Assert(eventInstance->release());
	}

	// Start the event, causing sound to play.
	// Params:
	//   releaseOnStop = Whether the event should be released once it is stopped.
	void SoundEvent::Start(bool releaseOnStop)
	{
		EventInstance* eventInstance = static_cast<EventInstance*>(data);
		FMOD_Assert(eventInstance->start());
		if (releaseOnStop)
		{
			FMOD_Assert(eventInstance->release());
		}
	}

	// Stop the event, silencing all sounds.
	void SoundEvent::Stop(bool fadeOut)
	{
		EventInstance* eventInstance = static_cast<EventInstance*>(data);
		FMOD_STUDIO_STOP_MODE mode;
		mode = fadeOut ? FMOD_STUDIO_STOP_ALLOWFADEOUT : FMOD_STUDIO_STOP_IMMEDIATE;
		FMOD_Assert(eventInstance->stop(mode));
	}

	// Get the current value of the given parameter.
	float SoundEvent::GetParameter(std::string name) const
	{
		EventInstance* eventInstance = static_cast<EventInstance*>(data);
		float value;
		FMOD_Assert(eventInstance->getParameterValue(name.c_str(), &value));
		return value;
	}

	// Set the value of the given parameter.
	void SoundEvent::SetParameter(std::string name, float value)
	{
		EventInstance* eventInstance = static_cast<EventInstance*>(data);
		FMOD_Assert(eventInstance->setParameterValue(name.c_str(), value));
	}

	// Get the volume of this event.
	float SoundEvent::GetVolume() const
	{
		EventInstance* eventInstance = static_cast<EventInstance*>(data);
		float volume;
		FMOD_Assert(eventInstance->getVolume(&volume));
		return volume;
	}

	// Set the volume of this event.
	void SoundEvent::SetVolume(float volume)
	{
		EventInstance* eventInstance = static_cast<EventInstance*>(data);
		FMOD_Assert(eventInstance->setVolume(volume));
	}
}

//------------------------------------------------------------------------------
