Collider Rectangle
==================

.. cpp:namespace:: Beta

.. cpp:class:: ColliderRectangle : public Collider

Methods
-------

.. cpp:function:: ColliderRectangle::ColliderRectangle(const Vector2D& extents = Vector2D(0.5f, 1.0f))
   
   Constructor for rectangle collider.
   
   :param extents: The rectangle's extents (half-width, half-height). The default is 0.5f by 1.0f.

.. cpp:function:: void ColliderRectangle::Draw() override
   
   Debug draw the collider.

.. cpp:function:: const Vector2D& ColliderRectangle::GetExtents() const
   
   Get the rectangle collider's extents (half-width, half-height).
   
   :return: The extents of the rectangle.

.. cpp:function:: void ColliderRectangle::SetExtents(const Vector2D& extents)
   
   Set the rectangle collider's extents (half-width, half-height).
   
   :param extents: The new extents of the rectangle.

.. cpp:function:: bool ColliderRectangle::IsCollidingWith(const Collider& other) const override
   
   Check for collision between this rectangle and another arbitrary collider.
   
   :param other: A reference to the second collider component.
   
   :return: True if a collision happened and false if not.

.. cpp:function:: bool ColliderRectangle::IsIntersectingWith(const LineSegment& ray, float& t) const override
   
   Perform intersection test with ray.
   
   :param ray: The ray with which to test intersection.
   
   :param t: The t value for the intersection.
   
   :return: True if the ray is intersecting this collider at some point, false if there is no collision.

.. cpp:function:: void ColliderRectangle::Serialize(FileStream& stream) const
   
   Save rectangle collider data to file.
   
   :param stream: The stream object used to save the collider's data.

.. cpp:function:: void ColliderRectangle::Deserialize(FileStream& stream)
   
   Load rectangle collider data from file.
   
   :param stream: The stream object used to load the collider's data.