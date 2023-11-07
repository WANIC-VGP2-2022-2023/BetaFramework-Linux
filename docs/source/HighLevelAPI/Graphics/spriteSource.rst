Sprite Source
=============

.. cpp:namespace:: Beta

.. cpp:class:: SpriteSource : public Serializable

Methods
-------

.. cpp:function:: SpriteSource::SpriteSource(ConstTexturePtr texture = nullptr, std::string name = "", unsigned numCols = 1, unsigned numRows = 1)
   
   Constructor for SpriteSource.
   
   :param texture: The texture that contains the frames that this sprite source will use.
   
   :param name: The name to use for this sprite source.
   
   :param numCols: The number of total columns in the sprite sheet texture.
   
   :param numRows: The number of total rows in the sprite sheet texture.

.. cpp:function:: void SpriteSource::UseTexture(unsigned frameIndex) const
   
   Activates the sprite source's texture and sets its UV offset based on the given frame index.
   
   :param frameIndex: The index of the frame of the texture that we want to use.

.. cpp:function:: unsigned SpriteSource::GetFrameCount() const
   
   Returns the maximum number of possible frames in the sprite source's texture (rows * columns).
   
   :return: The number of frames the texture could have.

.. cpp:function:: const std::string& SpriteSource::GetName() const
   
   Gets the name of the sprite source.
   
   :return: A reference to the name as a string.

.. cpp:function:: const Vector2D SpriteSource::GetTextureDimensions() const
   
   Gets the dimensions of the texture.
   
   :return: A 2D vector where the X value is the number of columns and the Y value is the number of rows.

.. cpp:function:: void SpriteSource::Serialize(FileStream& stream) const
   
   Save sprite source data to file.
   
   :param stream: The stream object used to save the sprite source's data.

.. cpp:function:: void SpriteSource::Deserialize(FileStream& stream)
   
   Load sprite source data from file.
   
   :param stream: The stream object used to load the sprite source's data.

.. cpp:function:: static SpriteSourceManager& SpriteSource::GetSpriteSourceManager()

	Returns the sprite source manager, which can be used to load new sprite sourcees
	or obtain existing sprite source resources.
	
	.. note:: To simplify the process of finding sprite source resources, use the *ResourceGetSpriteSource* macro. This macro expands to *SpriteSource::GetSpriteSourceManager().GetResource(s)*, where *s* is the name of the sprite source resource.
	
	:return: A reference to the sprite source manager.

.. cpp:function:: static TextureManager& SpriteSource::GetTextureManager()

	Returns the texture manager, which can be used to load new texturees
	or obtain existing texture resources.
	
	.. note:: To simplify the process of finding texture resources, use the *ResourceGetTexture* macro. This macro expands to *SpriteSource::GetTextureManager().GetResource(t)*, where *t* is the name of the texture resource.
	
	:return: A reference to the texture manager.
	