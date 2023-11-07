Game Object
===========

.. cpp:namespace:: Beta

.. cpp:class:: GameObject : public BetaObject, Serializable

Methods
-------

.. cpp:function GameObject::GameObject(const std::string& name)
   
   Constructor creates a new game object.
   
   :param name: The name of the game object being created.

.. cpp:function:: explicit GameObject::GameObject(const GameObject& other)
   
   Copy a game object from another object. Explicit to prevent accidental copies.
   
   :param other: A reference to the object being cloned.

.. cpp:function:: explicit GameObject::GameObject(Archetype archetype)
   
   Create a game object based on an archetype. Explicit to prevent accidental copies.
   
   :param archetype: The Archetype that will be used to create the object.

.. cpp:function:: GameObject::~GameObject()
   
   The destructor frees the memory associated with the game object.

.. cpp:function:: void GameObject::AddComponent(Component* component)
   
   Adds an existing component to the object.
   
   :param component: A pointer to the component that will have its owner changed to this object.

.. cpp:function:: template<typename ComponentType> ComponentType* GameObject::GetComponent()
   
   Retrieves the component with the given type if it exists. You can call the function like this::
   
      ComponentType* component = objectInstance.GetComponent<ComponentType>();
      if (component)
      {
         // Do stuff with the component here.
      }
   
   :type ComponentType: The class type of the component being retrieved from the object.
   
   :return: Returns a pointer to the component on the object if it exists and is null if the component was not present on the object.

.. cpp:function:: void GameObject::Initialize() override
   
   Initialize this object's components and set it to active.

.. cpp:function:: bool GameObject::IsDestroyed() const
   
   Whether the object has been marked for destruction.
   
   :return: True if the object has been marked for destruction and false otherwise.

.. cpp:function:: void GameObject::Update(float dt) override
   
   Update any components attached to the game object.
   
   :param dt: Change in time (in seconds) since the last game loop.

.. cpp:function:: void GameObject::FixedUpdate(float dt) override
   
   Updates components (usually just physics) using a fixed dt value. This is done to keep the physics simulation deterministic.
   
   :param dt: A fixed change in time, usually 1/60th of a second.

.. cpp:function:: void GameObject::Draw() override
   
   Draw any visible components attached to the game object.

.. cpp:function:: void GameObject::Destroy()
   
   Mark an object for destruction.
   
.. cpp:function:: bool GameObject::IsActive() const
   
   Tells whether an object will be updated.
   
   :return: True if the object is active and will be updated or false if inactive.

.. cpp:function:: void GameObject::SetActive(bool value)
   
   Sets whether an object will be updated.
   
   :param value: If set to true then the object will become active, false inactive.

.. cpp:function:: Space* GameObject::GetSpace() const
   
   Get the space that contains this object.
   
   :return: A pointer to the space that contains this object.

.. cpp:function:: void GameObject::Serialize(FileStream& stream) const
   
   Save object data to file.
   
   :param stream: The stream object used to save the object's data.

.. cpp:function:: void GameObject::Deserialize(FileStream& stream)
   
   Load object data from file
   
   :param stream: The stream object used to load the object's data.

.. cpp:function:: static ArchetypeManager& GameObject::GetArchetypeManager()

	Returns the archetype manager, which can be used to load new game object archetypes
	or obtain existing archetype resources.
	
	.. note:: To simplify the process of finding archetype resources, use the *ResourceGetArchetype* macro. This macro expands to *GameObject::GetArchetypeManager().GetResource(a)*, where *a* is the name of the object archetype.
	
	:return: A reference to the archetype manager.
	