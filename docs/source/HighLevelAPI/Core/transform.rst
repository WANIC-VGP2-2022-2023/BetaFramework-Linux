Transform
=========

.. cpp:namespace:: Beta

.. cpp:class:: Transform : public Component

Methods
-------

.. cpp:function:: Transform::Transform(float x, float y)
   
   Transform constructor.
   
   :param x: Initial world position on the x-axis.
   
   :param y: Initial world position on the y-axis.

.. cpp:function:: Transform::Transform(Vector2D translation = Vector2D(), Vector2D scale = Vector2D(1,1), float rotation = 0)
   
   Transform constructor.
   
   :param translation: World position of the object.
   
   :param scale: Width and height of the object.
   
   :param rotation: Rotation of the object about the z-axis in radians.

.. cpp:function:: const Matrix2D& Transform::GetMatrix() const
   
   Get the transform matrix, based upon translation, rotation and scale settings. The result is a 3 by 3 homogeneous coordinate matrix which is
   the result of concatenating a scale with a rotation, followed by a translation: [tra] * [rot] * [scl].

.. cpp:function:: const Matrix2D& Transform::GetInverseMatrix() const
   
   Get the inverse of the transform matrix.
   
   :return: A reference to the transform's inverse matrix.

.. cpp:function:: void Transform::SetTranslation(const Vector2D& translation)
   
   Set the translation of a transform component.
   
   :param translation: A reference to a translation vector, or the XY coordinate of the transform.

.. cpp:function:: const Vector2D& Transform::GetTranslation() const
   
   Get the translation of a transform component.
   
   :return: A reference to the component's translation structure.

.. cpp:function:: void Transform::SetRotation(float rotation)
   
   Set the rotation of a transform component.
   
   :param rotation: The rotation value in radians.

.. cpp:function:: float Transform::GetRotation() const
   
   Get the rotation value of a transform component.
   
   :return: The rotation about the z-axis in radians.

.. cpp:function:: void Transform::SetScale(const Vector2D& scale)
   
   Set the scale of a transform component.
   
   :param scale: A reference to a scale vector.

.. cpp:function:: const Vector2D& Transform::GetScale() const
   
   Get the scale of a transform component.
   
   :return: A reference to the component's scale structure.

.. cpp:function:: virtual const BoundingRectangle Transform::GetBounds() const
   
   Gets the bounds of the translation.
   
   :return: A bounding rectangle using translation and half scale.

.. cpp:function:: bool Transform::IsOnScreen() const
   
   Tests whether the object is visible on screen.

.. cpp:function:: void Transform::Serialize(FileStream& stream) const
   
   Save the transform data to file.
   
   :param stream: The stream object used to save the transform's data.

.. cpp:function:: void Transform::Deserialize(FileStream& stream)
   
   Load the transform data from file.
   
   :param stream: The stream object used to load the transform's data.