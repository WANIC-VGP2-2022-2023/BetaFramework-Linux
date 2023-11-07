Event Manager
=============

.. cpp:namespace:: Beta

.. cpp:class:: EventManager : public BetaObject

Methods
-------

.. cpp:function:: EventManager::EventManager()
   
   EventManager constructor.

.. cpp:function:: EventManager::~EventManager()
   
   EventManager destructor.

.. cpp:function:: void EventManager::Update(float dt) override
   
   Decreases timers on all delayed events, sends events that are ready.
   
   :param dt: The change in time since the previous frame.

.. cpp:function:: void EventManager::Shutdown() override
   
   Destroys all listeners and delayed events.

.. cpp:function:: void EventManager::SendEvent(const Event* event, const BetaObject* source, float delay = 0.0f)
   
   Sends an event to all registered listeners.
   
   :param event: A pointer to the event being sent.
   :param source: The source of the event, typically a game object.
   :param delay: How long to wait in seconds before sending the event.

.. cpp:function:: template <class ObjectType> void EventManager::RegisterEventHandler(const BetaObject* source, const std::string& type, ObjectType& destination, void (ObjectType::*handler)(const Event&))
   
   Registers an event handler with the event manager. The handler will be called when an event matching the given description is received.
   
   .. warning:: Each component should remember which events it registered with so the component can automatically unregister when it is destroyed. You should use the RegisterEventHandler function of the component class instead.
   
   :type ObjectType: The class type of the object registering to the event. For example if I am the Damage component on an object and I want to listen for the CollisionStartedEvent, then ObjectType would be Damage.
   
   :param source: The source of the event, typically a game object.
   
   :param type: The type of event, e.g. "CollisionEvent".
   
   :param destination: The object that contains the event handler function.
   
   :param handler: The function that will be called when the event is received.

.. cpp:function:: template <class ObjectType> void EventManager::RegisterEventHandler(Listener<ObjectType>& listener)
   
   Registers an event handler with the event manager. The handler will be called when an event matching the given description is received. This function is just a convenient overload of the previous function.
   
   :type ObjectType: The class type of the object registering to the event. For example if I am the Damage component on an object and I want to listen for the CollisionStartedEvent, then ObjectType would be Damage.
   
   :param listener: The listener containing information about the source, type, destination, and event handler function.

.. cpp:function:: template <class ObjectType> void EventManager::UnregisterEventHandler(const BetaObject* source, const std::string& type, ObjectType& destination, void (ObjectType::*handler)(const Event&))
   
   Removes a registered event handler from the list of listeners in the event manager.
   
   .. warning:: Each component should remember which events it registered with so the component can automatically unregister when it is destroyed. You should use the RegisterEventHandler function of the component class instead.
   
   :type ObjectType: The class type of the object registering to the event. For example if I am the Damage component on an object and I want to listen for the CollisionStartedEvent, then ObjectType would be Damage.
   
   :param source: The source of the event, typically a game object component.
   
   :param type: The type of event, e.g. "CollisionEvent".
   
   :param destination: The object that contains the event handler function.
   
   :param handler: The function that will be called when the event is received.

.. cpp:function:: template <class ObjectType> void EventManager::UnregisterEventHandler(Listener<ObjectType>& listener)
   
   Removes a registered event handler from the list of listeners in the event manager.
   
   :type ObjectType: The class type of the object registering to the event. For example if I am the Damage component on an object and I want to listen for the CollisionStartedEvent, then ObjectType would be Damage.
   
   :param listener: The listener containing information about the source, type, destination, and event handler function.