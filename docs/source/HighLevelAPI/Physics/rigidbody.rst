RigidBody
=========

.. cpp:namespace:: Beta

.. cpp:class:: RigidBody : public Component

Methods
-------

.. cpp:function:: RigidBody::RigidBody()
   
   Create a new physics component.

.. cpp:function:: const Vector2D& RigidBody::GetAcceleration() const
   
   Get the acceleration of a physics component. The acceleration is measured in Units per Second per Second.
   
   :return: A reference to the component's acceleration structure.

.. cpp:function:: const Vector2D& RigidBody::GetVelocity() const
   
   Get the velocity of a physics component. The velocity is measured in Units per Second.
   
   :return: A reference to the component's velocity structure.

.. cpp:function:: const Vector2D& RigidBody::GetOldTranslation() const
   
   Get the old translation (position) of a physics component.
   
   :return: A reference to the component's oldTranslation variable.

.. cpp:function:: float RigidBody::GetAngularVelocity() const
   
   Get the angular velocity of a physics component. The angular velocity is measured in Radians per Second.
   
   .. note:: A positive value means counter-clockwise rotation when viewed from the positive z-axis.
   
   :return: A float representing the new angular velocity.

.. cpp:function:: void RigidBody::AddForce(const Vector2D& force)
   
   Add a force to the object.
   
   :param force: A force vector with direction and magnitude.

.. cpp:function:: void RigidBody::SetVelocity(const Vector2D& velocity)
   
   Set the velocity of a physics component.
   
   :param velocity: Pointer to a velocity vector.

.. cpp:function:: void RigidBody::SetAngularVelocity(float velocity)
   
   Set the angular velocity of a physics component. The angular velocity is measured in Radians per Second.
   
   :param velocity: New value for the rotational velocity.

.. cpp:function:: void RigidBody::MovePosition(const Vector2D& position)
   
   Moves an object to the specified location, syncing its old, current, and new translations to that position.
   
   :param position: The position that the object should be in after this function call.

.. cpp:function:: void RigidBody::Initialize() override
   
   Initialize data and aquire component dependencies.

.. cpp:function:: void RigidBody::Update(float dt) override
   
   Updates the acceleration based on the forces acting upon the object.
   
   :param dt: Change in time (in seconds) since the last game loop.

.. cpp:function:: void RigidBody::FixedUpdate(float dt) override
   
   Update velocity and translation.
   
   :param dt: Change in time (in seconds) since the last fixed update.

.. cpp:function:: void RigidBody::Serialize(FileStream& stream) const
   
   Save rigidbody data to file.
   
   :param stream: The stream object used to save the rigidbody's data.

.. cpp:function:: void RigidBody::Deserialize(FileStream& stream)
   
   Load rigidbody data from file.
   
   :param stream: The stream object used to load the rigidbody's data.