Sound Event
===========

.. cpp:namespace:: Beta

.. cpp:class:: SoundEvent

.. note:: Due to the SoundEvent constructor being private, only the friend class SoundManager can create instances of the SoundEvent class.

Methods
-------

.. cpp:function:: SoundEvent::~SoundEvent()
   
   Destructor cleans up any allocated resources that the SoundEvent created.

.. cpp:function:: void SoundEvent::Start(bool releaseOnStop = true)
   
   Start the event, causing sound to play. Default is to release when the sound stops.
   
   :param releaseOnStop: If true the event should be destroyed once it is stopped.

.. cpp:function:: void SoundEvent::Stop(bool fadeOut = false)
   
   Stop the event, silencing all sounds.
   
   :param fadeOut: Whether the sound will fade out or end immediately. Default is to stop immediately.

.. cpp:function:: float SoundEvent::GetParameter(std::string name) const
   
   Get the current value of the given parameter.
   
   :param name: The name of the FMOD parameter of which the value will be retreived. If a parameter with the given name does not exist, check the console for FMOD errors.
   
   :return: The floating point value of the parameter.

.. cpp:function:: void SoundEvent::SetParameter(std::string name, float value)
   
   Set the value of the given parameter.
   
   :param name: The name of the parameter to be set.
   
   :param value: The value that the parameter will be set to.

.. cpp:function:: float SoundEvent::GetVolume() const
   
   Get the volume of this event.
   
   :return: The volume level in the range [0.0f, 1.0f].

.. cpp:function:: void SoundEvent::SetVolume(float volume)
   
   Set the volume of this event.
   
   :param volume: The volume that the event should be set to, this value should be in the range [0.0f, 1.0f].