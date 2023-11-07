Component
=========

.. cpp:namespace:: Beta

.. cpp:class:: Component : public BetaObject, public Serializable

Methods
-------

.. cpp:function:: Component::Component(const std::string& name)
   
   Constructor for the component.
   
   :param name: A name that will be given to the component to identify it.
   
   .. note:: The name does not need to be the same as the type of the component but probably should be so you do not confuse yourself when debugging.

.. cpp:function:: GameObject* Component::GetOwner() const
   
   Retrieve component's owner object (if any).
   
   :return: A pointer to the owner game object or null if an object does not own the component.

.. cpp:function:: virtual Component* Component::Clone() const = 0
   
   Clones the component, returning a dynamically allocated copy.
   
   :return: A pointer to the newly allocated component clone.

.. cpp:function:: virtual void Component::UnregisterEventHandlers() = 0
   
   Unregisters any events that were registered during intialization.

.. cpp:function:: virtual bool Component::IsOfType(size_t classType) const = 0
   
   Checks whether a component is of the given classType.
   
   .. note:: This is primarily used by the GameObject GetComponent function, and you should generally not have to call this function. This function should be private, but in order for the GameObject to access it we would need to make it a friend and to help keep things simple we just made it public.
   
   :param classType: An unsigned int representing the type of the other object.
   
   :return: True if the component is of the same classType as the given type, and false if the types do not match.