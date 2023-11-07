Reactive
========

.. cpp:namespace:: Events

.. cpp:class:: Reactive : public Component

Methods
-------

.. cpp:function:: Reactive()
   
   Allocate a new Reactive component.

.. cpp:function:: void Update(float dt)
   
   Check if the mouse is currently intersecting with this object.
   
   :param dt: The amount of time in seconds since the start of the last frame.

.. cpp:function:: Vector2D GetMouseWorldPosition() const
   
   Obtains the mouse's position in world coordinates.
   
   :return: The position of the mouse as a vector.

.. cpp:function:: void Serialize(FileStream& stream) const
   
   Save reactive data to file.
   
   :param stream: The stream object used to save the reactive's data.

.. cpp:function:: void Deserialize(FileStream& stream)
   
   Load reactive data from file.
   
   :param stream: The stream object used to load the reactive's data.