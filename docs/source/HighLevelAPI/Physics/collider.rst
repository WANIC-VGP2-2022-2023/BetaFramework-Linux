Collider
========

.. cpp:namespace:: Beta

.. cpp:class:: Collider : public Component

Methods
-------

.. cpp:function:: Collider::Collider(ColliderType type)
   
   Constructor allocates a new collider component.
   
   :param type: The type of collision shape represented by this collider. Can be one of ColliderTypeCircle, ColliderTypeLine, ColliderTypeRectangle, or ColliderTypeTilemap.

.. cpp:function:: void Collider::Initialize() override
   
   Set component pointers and intialize the component.

.. cpp:function:: void Collider::FixedUpdate(float dt) override
   
   Logic update for this component with fixed timestep.
   
   :param dt: The amount of time in seconds since the last frame. Since this is a fixed timestep the value will be 1/60th of a second by default.

.. cpp:function:: void Collider::CheckCollision(const Collider& other)
   
   Check if two objects are colliding and send collision events.
   
   :param other: A reference to the second collider component.

.. cpp:function:: virtual bool Collider::IsCollidingWith(const Collider& other) const = 0
   
   Perform intersection test between two arbitrary colliders. Note this is pure virtual because the Collider component is only an interface for what
   you need to implement when making a collider that has a specific shape.
   
   :param other: A reference to the second collider component.

.. cpp:function:: virtual bool Collider::IsIntersectingWith(const LineSegment& ray, float& t) const = 0
   
   Perform intersection test with ray to allow for raycasting.
   
   :param ray: The ray with which to test intersection.
   
   :param t: The t value for the intersection.
   
   :return: True if the ray is intersecting the collider at some point, false if no collision.

.. cpp:function:: ColliderType Collider::GetType() const
   
   Get the type of this component.
   
   :return: The type of the collider.

.. cpp:function:: bool Collider::WasProcesed() const
   
   Has this component been checked for collisons?
   
   :return: True if this collider has already been processed and false if it has not.

.. cpp:function:: void Collider::SetProcessed(bool value)
   
   Set this collider's processed bool.
   
   :param value: The value that the processed bool will be set to.