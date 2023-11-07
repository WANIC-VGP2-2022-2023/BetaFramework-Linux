Animator
========

.. cpp:namespace:: Beta

.. cpp:class:: Animator : public Component

Methods
-------

.. cpp:function:: Animator::Animator()
   
   Construct a new animator component.

.. cpp:function:: void Animator::Initialize() override
   
   Initialize related sprite components of the owning object.

.. cpp:function:: void Animator::FixedUpdate(float dt)
   
   Update the animation.
   
   :param dt: Change in time (in seconds) since the last game loop.

.. cpp:function:: void Animator::Play(size_t animationIndex = 0, float playbackSpeed = 1.0f, bool loop = true)
   
   Play an animation.
   
   :param animationIndex: The index of the animation to play. Default value is zero, or the first animation in the controller.
   
   :param playbackSpeed: The speed at which to play the animation.
   
   :param loop: Whether the animation should go back to the beginning after it reaches the end. By default this is true.

.. cpp:function:: size_t Animator::AddAnimation(ConstAnimationPtr animation)
   
   Add an animation to the controller.
   
   :param animation: The animation resource that will be added to the controller.
   
   :return: The index of the animation in the controller, for use with the Play function.

.. cpp:function:: size_t Animator::GetCurrentAnimationIndex() const
   
   Get the index of the currently playing animation, for use with the Play function.
   
   :return: The index of the currently playing animation.

.. cpp:function:: size_t Animator::GetAnimationIndex(const std::string& name) const
   
   Get the index of the animation with the specified name.
   
   :param name: The name of the animation for which the index was requested.
   
   :return: The index of the given animation if it was found, or 0 if it does not exist.

.. cpp:function:: bool Animator::IsDone() const
   
   Determine if the animation has reached the end of its sequence.
   
   :return: True if the animation is done, false otherwise.

.. cpp:function:: void Animator::SetPlaybackSpeed(float speed)
   
   Set the time to wait between frames for the currently playing animation.
   
   :param duration: The amount of time to wait between frames (in seconds).

.. cpp:function:: void Animator::Serialize(Serialization::FileStream& stream) const
   
   Save animation controller data to file.
   
   :param stream: The stream object used to save the animation controller's data.

.. cpp:function:: void Animator::Deserialize(Serialization::FileStream& stream)
   
   Load animation controller data from file.
   
   :param stream: The stream object used to load the animation controller's data.