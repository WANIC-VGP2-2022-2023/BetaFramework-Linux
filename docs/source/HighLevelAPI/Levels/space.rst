Space
=====

.. cpp:namespace:: Beta

.. cpp:class:: Space : public BetaObject

Methods
-------

.. cpp:function:: Space::Space(const std::string& name, bool customCamera = false)
   
   Creates a new space.
   
   :param name: The name of the space that is being created.
   
   :param customCamera: Default is false. Uses a custom camera implementation if set to true, otherwise it uses the default camera.

.. cpp:function:: Space::~Space()
   
   Shuts down all systems, deletes the camera and the current level if a level was loaded in the space.

.. cpp:function:: void Space::Update(float dt) override
   
   Updates the level and object manager.
   
   :param dt: Change in time (in seconds) since the last game loop.

.. cpp:function:: void Space::Draw() override
   
   Draw the level and object manager.

.. cpp:function:: void Space::Shutdown() override
   
   Shuts down the object manager.

.. cpp:function:: bool Space::IsPaused() const
   
   See if the space is paused.
   
   :return: A boolean indicating whether objects in this space are paused. True means they are paused.

.. cpp:function:: std::string Space::GetLevelName() const
   
   Get the name of the level that is loaded in this space.
   
   :return: The name of the level currently running in this space. If no level is loaded the string "no level currently loaded in space" will be returned.

.. cpp:function:: GameObjectManager& Space::GetObjectManager()
   
   Get a reference to the object manager of the space.
   
   :return: The object manager, which you can use to retrieve and add objects.

.. cpp:function:: void Space::SetPaused(bool value)
   
   Pauses the space, preventing objects from being updated, but objects are still drawn.
   
   :param value: True means paused, false unpaused.

.. cpp:function:: template <class LevelType> void Space::SetLevel()
   
   Sets the level that the space is using after unloading the current level.
   
   :type LevelType: The class type of the level that will be loaded.

.. cpp:function:: void Space::RestartLevel()
   
   Restarts the current level (next level = current)

.. cpp:function:: bool Space::IsDestroyed() const
   
   Checks whether space should be destroyed.
   
   :return: True if the space is marked for destruction and false if not.

.. cpp:function:: void Space::Destroy()
   
   Marks the space for destruction.

.. cpp:function:: Graphics::Camera& Space::GetCamera() const
   
   Get the camera used by this space.
   
   :return: A reference to the camera that is being used to view the space.