Collider Tilemap
================

.. cpp:namespace:: Beta

.. cpp:class:: ColliderTilemap : public Collider

Methods
-------

.. cpp:function:: ColliderTilemap::ColliderTilemap()
   
   Constructor for a Tilemap collider.

.. cpp:function:: void ColliderTilemap::Initialize() override
   
   Gets a pointer to the event manager.

.. cpp:function:: void ColliderTilemap::Draw() override
   
   Debug drawing for colliders.

.. cpp:function:: bool ColliderTilemap::IsCollidingWith(const Collider& other) const override
   
   Check for collision between a tilemap and another arbitrary collider.
   
   :param other: Reference to the second collider component.
   
   :return: The results of the collision check. True means a collision happened.

.. cpp:function:: bool ColliderTilemap::IsIntersectingWith(const LineSegment& ray, float& t) const override
   
   .. warning: [NOT CURRENTLY IMPLEMENTED]
   
   Perform intersection test with ray.
   
   :param ray: The ray with which to test intersection.
   
   :param t: The t value for the intersection.
   
   :return: True if a collision happened. Due to this not being implemented it will always return false.

.. cpp:function:: void ColliderTilemap::SetTilemap(Levels::ConstTilemapPtr map)
   
   Sets the tilemap to use for this collider.
   
   :param map: A pointer to the tilemap resource.

.. cpp:function:: void ColliderTilemap::Serialize(FileStream& stream) const
   
   Save tilemap collider data to file.
   
   :param stream: The stream object used to save the tilemap collider's data.

.. cpp:function:: void ColliderTilemap::Deserialize(FileStream& stream)
   
   Load tilemap collider data from file.
   
   :param stream: The stream object used to load the tilemap collider's data.