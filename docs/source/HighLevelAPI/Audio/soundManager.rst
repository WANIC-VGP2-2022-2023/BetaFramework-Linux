Sound Manager
=============

.. cpp:namespace:: Beta

.. cpp:class:: SoundManager : public BetaObject

Methods
-------

.. cpp:function:: SoundManager::SoundManager()
   
   Constructor initializes FMOD.

.. cpp:function:: SoundManager::~SoundManager()
   
   Destructor cleans up resources and uninitializes FMOD.

.. cpp:function:: void SoundManager::Update(float dt)
   
   Update the FMOD system.
   
   :param dt: Change in time (in seconds) since the last game loop.

.. cpp:function:: void SoundManager::Shutdown(void)
   
   Shutdown the sound manager. Frees any sounds or banks that have been loaded.

.. cpp:function:: void SoundManager::AddEffect(const std::string& filename)
   
   Creates a non-looping FMOD sound.
   
   :param filename: Name of the sound file (WAV or MP3). This path is relative to the Assets\Audio directory.

.. cpp:function:: void SoundManager::AddMusic(const std::string& filename)
   
   Creates a looping FMOD sound.
   
   :param filename: Name of the sound file (WAV or MP3). This path is relative to the Assets\Audio directory.

.. cpp:function:: void SoundManager::AddBank(const std::string& filename)
   
   Loads an FMOD sound bank.
   
   :param filename: Name of the bank file (BANK). This path is relative to the Assets\Audio\Banks directory.

.. cpp:function:: void SoundManager::RemoveSound(const std::string& filename)
   
   Unloads a sound file from memory. This function is essesntially the opposite of the AddEffect and AddMusic functions.
   
   :param filename: The name of the file that should be unloaded.

.. cpp:function:: void SoundManager::PlaySound(const std::string& name)
   
   Plays a sound with the specified name.
   
   :param name: The name of the sound to be played.

.. cpp:function:: SoundEventPtr SoundManager::PlayEvent(const std::string& eventName)
   
   Starts an audio event with the given name. The event must be in one of the loaded banks.
   
   :param eventName: The name of the event that will be started.
   
   :return: A pointer to the sound event so that it can still be controlled by changing settings of the event or stop it prematurely.

.. cpp:function:: void SoundManager::SetMusicVolume(float volume)
   
   Set the volume of the music channel.
   
   :param volume: New value for the music volume in the range [0.0f, 1.0f].

.. cpp:function:: void SoundManager::SetEffectsVolume(float volume)
   
   Set the volume of the sound effects channel group.
   
   :param volume: New value for the effect volume in the range [0.0f, 1.0f].

.. cpp:function:: float SoundManager::GetMusicVolume() const
   
   Get the volume of the music channel.
   
   :return: The volume as a float in the range [0.0f, 1.0f].

.. cpp:function:: float SoundManager::GetEffectsVolume() const
   
   Get the volume of the sound effects channel.
   
   :return: The volume as a float in the range [0.0f, 1.0f].