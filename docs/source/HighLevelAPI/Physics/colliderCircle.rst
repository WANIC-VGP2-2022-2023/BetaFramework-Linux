Collider Circle
===============

.. cpp:namespace:: Beta

.. cpp:class:: ColliderCircle : public Collider

Methods
-------

.. cpp:function:: ColliderCircle::ColliderCircle(float radius = 0.5f)
   
   Constructor for circle collider.
   
   :param radius: Radius of the circle collider. The default should be 0.5f and is measured in world units.

.. cpp:function:: void ColliderCircle::Draw() override
   
   Debug drawing for colliders.

.. cpp:function:: float ColliderCircle::GetRadius() const
   
   Get the circle collider's radius.
   
   :return: The radius of the circle collider.

.. cpp:function:: void ColliderCircle::SetRadius(float radius)
   
   Set the circle collider's radius.
   
   :param radius: The circle collider's new radius.

.. cpp:function:: bool ColliderCircle::IsCollidingWith(const Collider& other) const override
   
   Check for collision between this circle and another arbitrary collider.
   
   :param other: A reference to the second collider component.
   
   :return: The results of the collision check, true if there is a collision.

.. cpp:function:: bool ColliderCircle::IsIntersectingWith(const LineSegment& ray, float& t) const override
   
   Perform intersection test with ray.
   
   :param ray: The ray with which to test intersection.
   
   :param t: The t value for the intersection.
   
   :return: True if the ray intersected with this collider, false otherwise.

.. cpp:function:: void ColliderCircle::Serialize(FileStream& stream) const
   
   Save collider data to file.
   
   :param stream: The stream object used to save the collider's data.

.. cpp:function:: void ColliderCircle::Deserialize(FileStream& stream)
   
   Load the component data from file.
   
   :param stream: The stream object used to load the collider's data.