Texture
=======

.. cpp:namespace:: Beta

The Texture class is responsible for determining which texture data
is used when using meshes to draw sprites and text. Textures are
immutable, meaning they cannot be changed once they are created.

.. cpp:class:: Texture

Methods
-------

	.. rst-class:: collapsible
	
		.. cpp:function:: Texture::Texture()
		
			Construct an instance of the Texture class. Calling the default constructor
			will result in a white texture that is 1 pixel wide and 1 pixel tall.
	
	.. rst-class:: collapsible
	
		.. cpp:function:: Texture::Texture(const Array<Color>& colors, size_t width, size_t height, const std::string& name = "")
		
			Construct an instance of an RGBA texture using the provided data.
			
			.. note:: If you want to create a texture from a file, use the static :cpp:func:`Texture::CreateTextureFromFile` function instead of this one.
			
			:param colors: A dynamic array of colors in RGBA float format.
			:param width: The width of the texture, in pixels.
			:param height: The height of the texture, in pixels.
			:param name: The name to use for this texture, defaults to an empty string.
			
	.. rst-class:: collapsible
	
		.. cpp:function:: Texture::Texture(unsigned char* buffer, size_t width, size_t height, const std::string& name = "")
		
			Construct an instance of a red-channel texture using the provided data. 
			This is typically used internally when loading fonts.
			
			:param buffer: An array of unsigned characters representing a texture containing only the red channel.
			:param width: The width of the texture, in pixels.
			:param height: The height of the texture, in pixels.
			:param name: The name to use for this texture, defaults to an empty string.
	
	.. rst-class:: collapsible
	
		.. cpp:function:: Texture::~Texture()
		
			Destroy an instance of the Texture class. This function deallocates all resources
			allocated directly by the texture, which involves several calls to
			the internal rendering API (in this case, OpenGl).
			
	.. rst-class:: collapsible
	
		.. cpp:function:: void Texture::Use(const Vector2D& uvOffset = Vector2D(), const ShaderProgram* shader = nullptr) const
		
			Set this texture as the active texture used when rendering.
			
			:param uvOffset: A vector containing the offset to add to the texture coordinates used when rendering.
			:param shader: The shader that will use this texture. If no argument is given (or nullptr is given as an argument), the default sprite shader will use this texture.
	
	.. rst-class:: collapsible
	
		.. cpp:function:: const std::string& Texture::GetName() const
		
			Return the name of the texture.
			
			:return: A string containing the name of the texture.
		
	.. rst-class:: collapsible
	
		.. cpp:function:: unsigned Texture::GetBufferID() const
		
			Return the ID of the texture buffer in OpenGL.

			:return: The texture's ID, used by OpenGL to identify the texture.
	
	.. rst-class:: collapsible
	
		.. cpp:function:: static Texture* Texture::CreateTextureFromFile(const std::string& filename, TextureFilterMode mode = TM_Nearest)
		
			Load a texture from a file.
			
			:param filename: The filename of the texture that should be loaded.
			
			:param mode: Which mode to use for texture filtering. Defaults to nearest neighbor (TM_Nearest), but can also be bilinear (TM_Bilinear) or trilinear (TM_Trilinear).
			
			:return: A pointer to the newly created texture, or nullptr if the texture could not be loaded.