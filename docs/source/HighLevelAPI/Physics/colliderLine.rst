Collider Line
=============

.. cpp:namespace:: Beta

.. cpp:class:: ColliderLine : public Collider

Methods
-------

.. cpp:function:: ColliderLine::ColliderLine(bool reflection = true)
   
   Create a new line collider component.
   
   :param reflection: Should the collider perform reflection. By default this is true.

.. cpp:function:: void ColliderLine::Draw() override
   
   Debug drawing for the line collider.

.. cpp:function:: void ColliderLine::AddLineSegment(const Vector2D& p0, const Vector2D& p1)
   
   Add a line segment to the line collider's line segment list.
   
   :param p0: The line segment's starting position in world coordinates.
   
   :param p1: The line segment's ending position in world coordinates.

.. cpp:function:: bool ColliderLine::IsCollidingWith(const Collider& other) const override
   
   Check for collision between this line collider and another arbitrary collider.
   
   :param other: A reference to the other collider component.
   
   :return: Rhe results of the collision check, true if a collision happened.

.. cpp:function:: bool ColliderLine::IsIntersectingWith(const LineSegment& ray, float& t) const override
   
   Perform intersection test with ray.
   
   :param ray: The ray with which to test intersection.
   
   :param t: The t value for the intersection.
   
   :return: True if the ray is intersecting any of the line segments in this collider at some point, false if there is no collision.

.. cpp:function:: void ColliderLine::Serialize(FileStream& stream) const
   
   Save collider data to file.
   
   :param stream: The stream object used to save the collider's data.

.. cpp:function:: void ColliderLine::Deserialize(FileStream& stream)
   
   Load collider data from file.
   
   :param stream: The stream object used to load the collider's data.

.. cpp:function:: LineSegment ColliderLine::GetLineWithTransform(unsigned index) const
   
   Gets a line segment that incorporates the transform of the object.
   
   :param index: The index of the line within the array of line segments.
   
   :return: The line segment transformed with respect to the owner object's transform data.