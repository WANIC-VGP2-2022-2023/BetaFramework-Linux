//------------------------------------------------------------------------------
//
// File Name:	SoundManager.cpp
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
#include "SoundManager.h"
#include <fmod_errors.h>
#include <fmod_studio.hpp>
#include <fmod.hpp>
#include <sstream>

#include "EngineCore.h"	// EngineGetModule
#include "SoundEvent.h"	// SoundEvent

//------------------------------------------------------------------------------

using FMOD::Studio::EventInstance;

namespace Beta
{
	//------------------------------------------------------------------------------
	// Private Consts:
	//------------------------------------------------------------------------------

	static const unsigned maxNameLength = 100;
	static const unsigned maxEventsInBank = 100;
	static const std::string bankFilePath = "Banks/";

	// All events start with "event:/"
	static const std::string eventPrefix = "event:/";

	//------------------------------------------------------------------------------
	// Private Function Declarations:
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

	// Helper function for printing parameter info
	std::ostream& operator<<(std::ostream& os, const FMOD_STUDIO_PARAMETER_DESCRIPTION& parameter);

	//------------------------------------------------------------------------------
	// Private Structures:
	//------------------------------------------------------------------------------

	class SoundManager::Implementation
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor/Destructor
		Implementation()
			: effectsVolume(1.0f), musicVolume(1.0f), system(nullptr), musicChannel(nullptr), audioFilePath("Audio/")
		{
			FMOD_Assert(FMOD::Debug_Initialize(FMOD_DEBUG_LEVEL_ERROR)); // Only show errors in log
			FMOD_Assert(FMOD::Studio::System::create(&studioSystem)); // Create the main system object.
			FMOD_Assert(studioSystem->initialize(64, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0));
			FMOD_Assert(studioSystem->getLowLevelSystem(&system));
			FMOD_Assert(system->createChannelGroup("SoundEffects", &effectsChannelGroup));
		}

		~Implementation()
		{
			FMOD_Assert(studioSystem->release());
		}

		// Play a sound effect
		void PlayEffect(FMOD::Sound* sound) const
		{
			FMOD_Assert(system->playSound(sound, effectsChannelGroup, false, nullptr));
		}

		// Play background music
		void PlayMusic(FMOD::Sound* sound)
		{
			if (musicChannel != nullptr)
				FMOD_Assert(musicChannel->stop());

			FMOD_Assert(system->playSound(sound, nullptr, false, &musicChannel));
		}

		// Add FX/BGM helper
		void AddSound(const std::string & filename, FMOD_MODE mode)
		{
			const std::string& enginePath = EngineCore::GetInstance().GetFilePath();
			std::string fullPath = enginePath + audioFilePath + filename;
			int fullPathSize = static_cast<int>(fullPath.size());

			// Make sure we haven't already added this by checking the name of the sound against sounds in our list.
			char* nameBuffer = new char[fullPath.size()];
			bool found = false;
			for (auto it = soundList.Begin(); it != soundList.End(); ++it)
			{
				(*it)->getName(nameBuffer, fullPathSize);
				if (strcmp(nameBuffer, fullPath.c_str()) == 0)
				{
					found = true;
					break;
				}
			}
			delete[] nameBuffer;
			if (found) return;

			// Okay to add one object
			FMOD::Sound* sound;
			FMOD_Assert(system->createSound(fullPath.c_str(), mode, nullptr, &sound));
			soundList.PushBack(sound);
		}

		//------------------------------------------------------------------------------
		// Public Variables:
		//------------------------------------------------------------------------------

		Array<FMOD::Sound*> soundList;			// List of all loaded sounds
		Array<FMOD::Studio::Bank*> bankList;	// List of all loaded sound banks.
		FMOD::Channel* musicChannel;			 // The channel most recently used to play music
		FMOD::ChannelGroup* effectsChannelGroup; // The channel group used for SFX.
		FMOD::System* system;					 // The internal FMOD low-level system
		FMOD::Studio::System* studioSystem;		 // The internal FMOD studio system.
		float musicVolume;	 // The current volume of the music channel (0.0 to 1.0).
		float effectsVolume; // The current volume of the sound effects channel group (0.0 to 1.0).

		std::string audioFilePath;
	};

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor - initializes FMOD.
	SoundManager::SoundManager()
		: BetaObject("Module:SoundManager"), pimpl(new Implementation())
	{
	}

	// Destructor
	SoundManager::~SoundManager()
	{
		delete pimpl;
	}

	// Update the FMOD system.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void SoundManager::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);
		FMOD_Assert(pimpl->studioSystem->update());
	}

	// Shutdown the sound manager.
	void SoundManager::Shutdown(void)
	{
		size_t numSounds = pimpl->soundList.Size();
		for (size_t i = 0; i < numSounds; ++i)
		{
			pimpl->soundList[i]->release();
		}
		pimpl->soundList.Clear();

		size_t numBanks = pimpl->bankList.Size();
		for (size_t i = 0; i < numBanks; ++i)
		{
			pimpl->bankList[i]->unload();
		}
		pimpl->bankList.Clear();
	}

	// Creates a non-looping FMOD sound.
	// Params:
	//	 filename = Name of the sound file (WAV).
	void SoundManager::AddEffect(const std::string& filename)
	{
		pimpl->AddSound(filename, FMOD_DEFAULT | FMOD_LOOP_OFF);
	}

	// Creates an FMOD stream for a music file.
	// Params:
	//	 filename = Name of the music file (MP3).
	void SoundManager::AddMusic(const std::string& filename)
	{
		pimpl->AddSound(filename, FMOD_DEFAULT | FMOD_LOOP_NORMAL);
	}

	// Creates an FMOD sound bank
	// Params:
	//   filename = Name of the FMOD bank file.
	void SoundManager::AddBank(const std::string & filename)
	{
		// Load the FMOD bank file.
		const std::string& enginePath = EngineCore::GetInstance().GetFilePath();
		std::string fullPath = enginePath + pimpl->audioFilePath + bankFilePath + filename;
		FMOD::Studio::Bank* bank;
		FMOD_Assert(pimpl->studioSystem->loadBankFile(fullPath.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank));
		pimpl->bankList.PushBack(bank);

#if _DEBUG
		std::cout << "Loaded FMOD Bank \"" << filename << "\" successfully." << std::endl;

		// Determine number of events in bank
		FMOD::Studio::EventDescription* events[maxEventsInBank];
		int eventCount;
		bank->getEventList(events, maxEventsInBank, &eventCount);
		char eventPath[maxNameLength];
		int nameSize;

		// No events - no need to print any more info
		if (eventCount == 0)
		{
			std::cout << std::endl;
			return;
		}

		std::cout << "Events in bank:" << std::endl;
		for (int i = 0; i < eventCount; ++i)
		{
			// Display each event
			events[i]->getPath(eventPath, maxNameLength, &nameSize);
			std::string eventName(eventPath);
			eventName = eventName.substr(eventName.find("/") + 1, eventName.size());
			std::cout << i << ": " << "\"" << eventName << "\"" << std::endl;

			// Determine number of parameters in event
			int paramCount;
			events[i]->getParameterCount(&paramCount);
			std::cout << "Parameters for event:" << std::endl;

			for (int j = 0; j < paramCount; ++j)
			{
				// Display each event parameter
				FMOD_STUDIO_PARAMETER_DESCRIPTION param;
				events[i]->getParameterByIndex(j, &param);

				std::cout << param << std::endl;
			}
		}

		std::cout << std::endl;
#endif
	}

	// Unloads a sound file from memory.
	void SoundManager::RemoveSound(const std::string & filename)
	{
		std::string fullPath = pimpl->audioFilePath + filename;
		char* nameBuffer = new char[fullPath.size()];

		// For each sound object
		for (auto it = pimpl->soundList.Begin(); it != pimpl->soundList.End(); )
		{
			// If its name matches the one we're looking for
			(*it)->getName(nameBuffer, (int)fullPath.size());
			if (strcmp(nameBuffer, fullPath.c_str()) == 0)
			{
				// Release it
				(*it)->release();
				it = pimpl->soundList.Erase(it);
				break;
			}
			else
			{
				++it;
			}
		}
		delete[] nameBuffer;
	}

	// Plays a sound with the specified name.
	// Params:
	//	 name = The name of the sound to be played.
	void SoundManager::PlaySound(const std::string & soundName)
	{
		char soundString[maxNameLength];
		FMOD_MODE mode;

		size_t numSounds = pimpl->soundList.Size();
		for (size_t i = 0; i < numSounds; ++i)
		{
			FMOD_Assert(pimpl->soundList[i]->getName(soundString, maxNameLength));

			if (strcmp(soundName.c_str(), soundString) == 0)
			{
				// If the sound is a stream, record the channel being used for future use
				FMOD_Assert(pimpl->soundList[i]->getMode(&mode));
				if (mode & FMOD_CREATESTREAM)
					pimpl->PlayMusic(pimpl->soundList[i]);
				else
					pimpl->PlayEffect(pimpl->soundList[i]);
			}
		}
	}

	// Starts an audio event with the given name.
	// The event must be in one of the loaded banks.
	// Params:
	//	 eventName = The name of the event that will be started.
	SoundEventPtr SoundManager::PlayEvent(const std::string & eventName)
	{
		// Construct full event path
		std::string eventPath = eventPrefix + eventName;

		// Find an event with the given name from the loaded banks.
		FMOD::Studio::EventDescription* description;
		FMOD_Assert(pimpl->studioSystem->getEvent(eventPath.c_str(), &description));

		// Create an instance of the event to be played.
		EventInstance* instance;
		FMOD_Assert(description->createInstance(&instance));

		// NOTE: You don't actually want to start the instance here.
		// The event should be started AFTER its properties have been set.

		// Return the event instance so the caller can use it to modify event parameters.
		return SoundEventPtr(new SoundEvent(instance));
	}

	// Set the volume of the music channel.
	// Params:
	//	 volume = New value for the music volume (0.0 to 1.0).
	void SoundManager::SetMusicVolume(float volume)
	{
		pimpl->musicVolume = volume;

		if (pimpl->musicChannel != nullptr)
			FMOD_Assert(pimpl->musicChannel->setVolume(pimpl->musicVolume));
	}

	// Set the volume of the sound effects channel group.
	// Params:
	//	 volume = New value for the FX volume (0.0 to 1.0).
	void SoundManager::SetEffectsVolume(float volume)
	{
		pimpl->effectsVolume = volume;
		FMOD_Assert(pimpl->effectsChannelGroup->setVolume(pimpl->effectsVolume));
	}

	// Get the volume of the music channel.
	// Return:
	//	 volume = Current value for the music volume.
	float SoundManager::GetMusicVolume() const
	{
		return pimpl->musicVolume;
	}

	// Get the volume of the sound effects channel group.
	// Return:
	//	 volume = Current value for the FX volume.
	float SoundManager::GetEffectsVolume() const
	{
		return pimpl->effectsVolume;
	}

	// Helper function for printing information about parameters associated with events
	std::ostream& operator<<(std::ostream & os, const FMOD_STUDIO_PARAMETER_DESCRIPTION & parameter)
	{
		os << "  " << parameter.name << " (" << parameter.defaultvalue << " by default, range of "
			<< parameter.minimum << " to " << parameter.maximum << ")" << std::endl;

		std::string typeInfo;

		switch (parameter.type)
		{
		case FMOD_STUDIO_PARAMETER_GAME_CONTROLLED:
			typeInfo = "Controlled via the API using Studio::EventInstance::setParameterValue.";
			break;
		case FMOD_STUDIO_PARAMETER_AUTOMATIC_DISTANCE:
			typeInfo = "Distance between the event and the listener.";
			break;
		case FMOD_STUDIO_PARAMETER_AUTOMATIC_EVENT_CONE_ANGLE:
			typeInfo = "Angle between the event's forward vector and the vector pointing from the event to the listener (0 to 180 degrees).";
			break;
		case FMOD_STUDIO_PARAMETER_AUTOMATIC_EVENT_ORIENTATION:
			typeInfo = "Horizontal angle between the event's forward vector and listener's forward vector (-180 to 180 degrees).";
			break;
		case FMOD_STUDIO_PARAMETER_AUTOMATIC_DIRECTION:
			typeInfo = "Horizontal angle between the listener's forward vector and the vector pointing from the listener to the event (-180 to 180 degrees).";
			break;
		case FMOD_STUDIO_PARAMETER_AUTOMATIC_ELEVATION:
			typeInfo = "Angle between the listener's XZ plane and the vector pointing from the listener to the event (-90 to 90 degrees).";
			break;
		case FMOD_STUDIO_PARAMETER_AUTOMATIC_LISTENER_ORIENTATION:
			typeInfo = "Horizontal angle between the listener's forward vector and the global positive Z axis (-180 to 180 degrees).";
			break;
		}

		os << "  " << typeInfo << std::endl;

		return os;
	}
}
