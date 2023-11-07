Game Object Factory
===================

.. cpp:namespace:: Beta

.. cpp:class:: GameObjectFactory : public BetaObject

Methods
-------

.. cpp:function:: GameObjectFactory::GameObjectFactory()
   
   Constructor registers core components.

.. cpp:function:: GameObjectFactory::~GameObjectFactory()
   
   Delete all registered components.

.. cpp:function:: GameObject* GameObjectFactory::CreateObject(const std::string& name, bool ignoreUnregisteredComponents = false)
   
   Create a single instance of the specified game object. Loads the object from a text file (if it exists).
   
   :param name: The name of the object.
   
   :param ignoreUnregisteredComponents: Whether to return the object even if it contains unregistered components. True means return it anyway, but it is default false.
   
   .. note:: The main use is in the editor where you would want to see the object even if custom components have not yet been registered.
   
   :return: If the object file exists and can be loaded, then return a pointer to a new instance of the specified game object type, else nullptr.

.. cpp:function:: Component* GameObjectFactory::CreateComponent(const std::string& componentName)
   
   Create a single instance of the specified component.
   
   :param name: The name of the component.
   
   :return: A pointer to a new instance of the specified component if that component exists, else nullptr.

.. cpp:function:: void GameObjectFactory::SaveObjectToFile(const GameObject* object)
   
   Saves an object to an archetype file so it can be loaded later.
   
   :param object: The object being saved to a file. Filename is based on object name.

.. cpp:function:: template <class ComponentType> void GameObjectFactory::RegisterComponent()
   
   Register a component so it can be created on the fly.
   
   :type ComponentType: The type of the component to be registered.