Space Manager
=============

.. cpp:namespace:: Beta

.. cpp:class:: SpaceManager : public BetaObject

Methods
-------

.. cpp:function:: SpaceManager::SpaceManager()
   
   Constructor creates the default game space.

.. cpp:function:: SpaceManager::~SpaceManager()
   
   Destructor deletes all spaces.

.. cpp:function:: void SpaceManager::Initialize() override
   
   Initialize the space manager.

.. cpp:function:: void SpaceManager::Update(float dt) override
   
   Updates all spaces and destroys those that are ready for destruction.
   
   :param dt: The change in time (in seconds) since the last game loop.

.. cpp:function:: void SpaceManager::Draw() override
   
   Draw objects in all spaces.

.. cpp:function:: void SpaceManager::Shutdown() override
   
   Shuts down the space manager, destroying all spaces.

.. cpp:function:: Space* SpaceManager::AddSpace(const std::string& name, bool useCustomCamera)
   
   Creates a new space with the given name.
   
   :param name: The name of the newly created space.
   
   :param useCustomCamera: Whether this space should have its own camera.

.. cpp:function:: Space* SpaceManager::GetSpaceByName(const std::string& name) const
   
   Returns a pointer to the space with the given name.
   
   :return: A nullptr if no such space exists.

.. cpp:function:: Space& SpaceManager::GetDefaultSpace()
   
   Returns the default space for game objects.