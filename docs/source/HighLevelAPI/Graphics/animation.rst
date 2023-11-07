Animation
=========

.. cpp:namespace:: Beta

.. cpp:class:: Animation : public Serializable

Methods
-------

.. cpp:function:: Animation::Animation(const std::string& name = "", ConstSpriteSourcePtr spriteSource = nullptr, unsigned frameCount = 1, unsigned frameStart = 0, float frameDuration = 0.0f)
   
   Construct a new animation resource.
   
   :param name: The name of the animation.
   
   :param spriteSource: The sprite source used by this animation.
   
   :param frameCount: The number of frames in this animation.
   
   :param frameStart: The starting frame in the sprite source for this animation.
   
   :param duration: The time (in seconds) that each frame will be displayed.

.. cpp:function:: virtual Animation::~Animation()
   
   Destructor defined to support inheritance. Default implementation does not clean up anything.

.. cpp:function:: unsigned Animation::GetCurrentFrameIndex(float time, float playbackSpeed, bool looping, bool* isDone = nullptr) const
   
   Get the current frame of the animation based on time.
   
   :param time: The amount of time the animation has been playing.
   
   :param playbackSpeed: How fast the animation is playing (1.0f is normal speed).
   
   :param loop: Whether the animation should loop.
   
   :param isDone: A pointer to an output parameter. True if animation is on its last frame, false otherwise.

.. cpp:function:: const std::string& Animation::GetName() const
   
   Get the name of the animation.
   
   :return: A reference to the name of the animation.

.. cpp:function:: const ConstSpriteSourcePtr Animation::GetSpriteSource() const
   
   Get the sprite source used by this animation.
   
   :return: A pointer to the sprite source that is being used.

.. cpp:function:: void Animation::Serialize(FileStream& stream) const
   
   Save animation data to file.
   
   :param stream: The stream object used to save the animation's data.

.. cpp:function:: void Animation::Deserialize(FileStream& stream)
   
   Load animation data from file.
   
   :param stream: The stream object used to load the animation's data.
   
.. cpp:function:: static AnimationManager& Animation::GetAnimationManager()

	Returns the animation manager, which can be used to load new animations
	or obtain existing animation resources.
	
	.. note:: To simplify the process of finding animation resources, use the *ResourceGetAnimation* macro. This macro expands to *Animation::GetAnimationManager().GetResource(a)*, where *a* is the name of the animation.
	
	:return: A reference to the animation manager.
