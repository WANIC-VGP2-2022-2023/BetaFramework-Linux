Area
====

.. cpp:namespace:: Beta

.. cpp:class:: Area : public Transform

Methods
-------

.. cpp:function:: Area::Area(float x, float y)
   
   Area constructor.
   
   :param x: Initial world position on the x-axis.
   
   :param y: Initial world position on the y-axis.

.. cpp:function:: Area::Area(Vector2D translation = Vector2D(), Vector2D scale = Vector2D(1, 1), float rotation = 0)
   
   Area constructor.
   
   :param translation: World position of the object.
   
   :param scale: Width and height of the object.
   
   :param rotation: Rotation of the object about the z-axis.

.. cpp:function:: void Area::Draw() override
   
   Draw the bounding rectangle of the area.

.. cpp:function:: void Area::SetSize(const Vector2D& size)
   
   Set the size, which is multiplied by the scale to find the bounding area.
   
   :param size: The size vector.

.. cpp:function:: void Area::SetOffset(const Vector2D& offset)
   
   Set the offset of the area center from the object's translation.
   
   :param offset: The offset vector.

.. cpp:function:: void Area::SetZDepth(float depth)
   
   Set the zDepth used for debug drawing.
   
   :param depth: The z depth that will be used.

.. cpp:function:: const BoundingRectangle Area::GetBounds() const override
   
   Returns a bounding rectangle using translation and (half) scale.
   
   :return: The BoundingRectangle of the area.