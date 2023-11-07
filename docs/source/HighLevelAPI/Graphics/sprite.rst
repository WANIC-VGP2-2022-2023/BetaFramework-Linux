Sprite
======

.. cpp:namespace:: Beta

.. cpp:class:: Sprite : public Component

Methods
-------

.. cpp:function:: Sprite::Sprite()
   
   Create a new sprite component.

.. cpp:function:: void Sprite::Initialize() override
   
   Obtains mesh and sprite source data from resource manager and initializes other necessary components.

.. cpp:function:: void Sprite::Draw() override
   
   Draw a sprite.
   
   .. note:: The sprite can be textured or untextured. If the sprite has no sprite source, the default (white) texture will be used, so color will be determined primarily by the sprite's *color* variable and the vertex colors of the sprite's mesh.

.. cpp:function:: void Sprite::Draw(const Vector2D& offset)
   
   Draw a sprite at an offset from the object's translation.
   
   :param offset: The offset that will be added to the translation when drawing.

.. cpp:function:: void Sprite::SetAlpha(float alpha)
   
   Set a sprite's transparency. Transparency is in the range [0.0f, 1.0f]. Zero means invisible.
   
   :param alpha: New value for the sprite's alpha value.

.. cpp:function:: float Sprite::GetAlpha() const
   
   Get the current value for a sprite's transparency.
   
   :return: The alpha value in the range [0.0f, 1.0f].

.. cpp:function:: void Sprite::SetFrame(unsigned int frameIndex)
   
   Set the sprite's current frame. If an invalid frame is specified, the current frame will not change.
   
   :param frameIndex: New frame index for the sprite in the range [0, frameCount).

.. cpp:function:: void Sprite::SetMesh(MeshPtr mesh)
   
   Set the sprite's mesh.
   
   .. note:: This mesh may be textured or untextured and may contain any number of triangles.
   
   :param mesh: A pointer to a mesh.

.. cpp:function:: void Sprite::SetSpriteSource(ConstSpriteSourcePtr spriteSource)
   
   Set a new SpriteSource for the specified sprite.
   
   :param spriteSource: A new sprite source for the sprite.

.. cpp:function:: ConstSpriteSourcePtr Sprite::GetSpriteSource()
   
   Returns the current sprite source being used by the sprite.
   
   :return: A pointer to the current sprite source that is being used.

.. cpp:function:: void Sprite::SetColor(Color color)
   
   Set the blend color for the specified sprite.
   
   :param color: A new color for the sprite.

.. cpp:function:: const Color& Sprite::GetColor() const
   
   Retrieves the blend color for the sprite.
   
   :return: The RGBA blend color of the sprite.

.. cpp:function:: float Sprite::GetZDepth() const
   
   Retrieves the current depth of the sprite in the level.
   
   :return: The depth of the sprite in the level.

.. cpp:function:: void Sprite::SetZDepth(float depth)
   
   Sets the current depth of the sprite in the level.
   
   :param depth: The new depth of the sprite.

.. cpp:function:: void Sprite::Serialize(FileStream& stream) const
   
   Save sprite data to file.
   
   :param stream: The stream object used to save the sprite's data.

.. cpp:function:: void Sprite::Deserialize(FileStream& stream)
   
   Load sprite data from file.
   
   :param stream: The stream object used to load the sprite's data.
   
.. cpp:function:: static MeshManager& Sprite::GetMeshManager()

	Returns the mesh manager, which can be used to load new meshes
	or obtain existing mesh resources.
	
	.. note:: To simplify the process of finding mesh resources, use the *ResourceGetMesh* macro. This macro expands to *Sprite::GetMeshManager().GetResource(m)*, where *m* is the name of the mesh resource.
	
	:return: A reference to the mesh manager.
