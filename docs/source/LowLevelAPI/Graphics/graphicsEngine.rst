GraphicsEngine
==============

.. cpp:namespace:: Beta

The GraphicsEngine class is responsible for starting and ending the rendering process, allowing the user to set general graphics 
properties such as transform and background color, as well as providing access to the default shaders and cameras used for rendering.

Access to the GraphicsEngine class can be obtained using *EngineGetModule(GraphicsEngine)*, provided both GraphicsEngine.h and
Engine.h (or something that includes them) are included.

.. cpp:class:: GraphicsEngine

Methods
-------

	.. rst-class:: collapsible
	
		.. cpp:function:: GraphicsEngine::GraphicsEngine()
		
			Construct an instance of the GraphicsEngine class. Initializes graphics variables
			to a clean state. Under normal circumstances, users should not need to call
			this function directly (or indirectly by creating an instance of the class).
	
	.. rst-class:: collapsible
	
		.. cpp:function:: GraphicsEngine::~GraphicsEngine()
		
			Destroy an instance of the GraphicsEngine class. This function deallocates all resources
			allocated directly by the graphics engine, such as default textures, and shuts down the 
			text rendering system.
	
	.. rst-class:: collapsible

		.. cpp:function:: void GraphicsEngine::Initialize(unsigned width, unsigned height)

			Initialize the graphics engine. This involves creating the default texture, initializing 
			the OpenGl renderer, and setting initial graphics properties (such as the viewport) 
			to their default values. No graphics operations should be requested until after this
			function has been called.
	
	.. rst-class:: collapsible

		.. cpp:function:: void GraphicsEngine::FrameStart()
		
			Inform the renderer that a new frame is about to start.
			
			.. note:: At the start of a frame, the camera used for rendering is reset to the default camera. In addition, if the camera is using a perspective projection, depth testing is re-enabled.
			
	.. rst-class:: collapsible
		
		.. cpp:function:: void GraphicsEngine::FrameEnd()
		
			Inform the renderer that the frame is over. Anything drawn after the last call to FrameStart
			will be rendered to the window.
	
	.. rst-class:: collapsible
	
		.. cpp:function:: const Color& GraphicsEngine::GetBackgroundColor() const
			
			Retrieve the current background color. Any pixels within the viewport that would otherwise be
			uncolored will have this color.
			
			:return: A color representing the current background color.

	.. rst-class:: collapsible
	
		.. cpp:function:: void GraphicsEngine::SetBackgroundColor(const Color& color = Colors::Black)
			
			Set the current background color. Any pixels within the viewport that would otherwise be
			uncolored will have this color.
			
			:param color: The color to use for the new background color. Defaults to black.
	
	.. rst-class:: collapsible
	
		.. cpp:function:: const Color& GraphicsEngine::GetScreenTintColor() const
			
			Retrieve the current tint color. All pixels within the viewport will be blended with this color.
			
			:return: A color representing the current tint color.

	.. rst-class:: collapsible
	
		.. cpp:function:: void GraphicsEngine::SetScreenTintColor(const Color& color = Colors::White)
			
			Set the current tint color. All pixels within the viewport will be blended with this color.
			
			:param color: The color to use for the new tint color. Defaults to white.
	
	.. rst-class:: collapsible
	
		.. cpp:function:: void GraphicsEngine::SetSpriteBlendColor(const Color& color = Colors::White)
			
			Set the current blend color. All subsequent draw calls using default shaders will
			have their color (typically from a texture or vertex data) blended with this color.
			
			:param color: The color to blend with when drawing sprites. Defaults to white.
	
	.. rst-class:: collapsible
	
		.. cpp:function:: const Texture& GraphicsEngine::GetDefaultTexture() const
			
			Return the default texture, typically used for drawing when no other texture is present.
			
			:return: A 1x1 white texture.
	
	.. rst-class:: collapsible
	
		.. cpp:function:: void GraphicsEngine::SetTransform(const Matrix2D& matrix, float depth = 0.0f)
			
			Set the world transform used for subsequent draw calls.
			
			:param matrix: The matrix representing the world transform.
			:param depth: The depth at which to draw when using a perspective projection. Defaults to 0.0f.
			
	.. rst-class:: collapsible
	
		.. cpp:function:: void GraphicsEngine::SetTransform(const Vector2D& translation, const Vector2D& scale = Vector2D(1.0f,1.0f), float rotation = 0.0f, float depth = 0.0f)
			
			Set the world transform used for subsequent draw calls.
			
			:param translation: The world translation that will be used when drawing.
			:param scale: The world scale that will be used when drawing.
			:param rotation: The world rotation (around the z-axis) that will be used when drawing, in radians. Defaults to 0.0f.
			:param depth: The depth at which to draw when using a perspective projection. Defaults to 0.0f.
			
	.. rst-class:: collapsible
	
		.. cpp:function:: Camera& GraphicsEngine::GetDefaultCamera() const
			
			Retrieve the camera that is used by default when rendering.
		
			:return: A reference to the default camera.
			
	.. rst-class:: collapsible
	
		.. cpp:function:: ShaderProgram& GraphicsEngine::GetSpriteShader() const
		
			Retrieve the shader that is used by default when rendering sprites.
			
			:return: A reference to the default sprite shader.
			
	.. rst-class:: collapsible
	
		.. cpp:function:: ShaderProgram& GraphicsEngine::GetFontShader() const
		
			Retrieve the shader that is used when rendering text.
			
			:return: A reference to the text shader.
	
	.. rst-class:: collapsible
	
		.. cpp:function:: void GraphicsEngine::SetBlendMode(BlendMode mode, bool forceSet = false)
			
			Set the algorithm used for alpha-blending when drawing sprites and text.
			
			:param mode: The blending mode. Can be *BM_None*, *BM_Blend* (default behavior), *BM_Add*, *BM_Multiply*, or *BM_Screen*.
			:param forceSet: Forces the engine to set the blend mode even if the current mode and the new mode are the same. Defaults to false.
	
	.. rst-class:: collapsible
	
		.. cpp:function:: void GraphicsEngine::PushEffect(PostEffect& effect)
		
			Add a post-processing effect to the rendering pipeline. Effects are applied sequentially, 
			starting with the first effect that was added.
			
			:param effect: An effect that will be added to the end of the current list of effects.
	
	.. rst-class:: collapsible
	
		.. cpp:function:: void GraphicsEngine::PopEffect()
		
			Remove the effect most recently added using *PushEffect* from the rendering pipeline.
			
	.. rst-class:: collapsible
	
		.. cpp:function:: void GraphicsEngine::RemoveEffect(PostEffect& effect)
		
			Search for an effect matching the given effect and remove it from the rendering pipeline. 
			If no such effect exists, no change is made to the current pipeline.
			
			:param effect: An effect that matches the effect to be removed.
	
	.. rst-class:: collapsible
	
		.. cpp:function:: void GraphicsEngine::ClearEffects()
		
			Remove all active post-processing effects from the rendering pipeline. 
			Subsequent frames will have no longer have effects applied to them.
			
	.. rst-class:: collapsible
	
		.. cpp:function:: bool GraphicsEngine::GetUseVsync() const
		
			Test whether vertical sync is currently on. Vertical sync reduces the likelihood of 
			screen tearing at the expense of capping framerates to multiples of the refresh rate.
			
			:return: True if vertical sync is on, false otherwise.
			
	.. rst-class:: collapsible
	
		.. cpp:function:: void GraphicsEngine::SetUseVsync(bool useVsync)
		
			Turn vertical sync on or off. Vertical sync reduces the likelihood of 
			screen tearing at the expense of capping framerates to multiples of the refresh rate.
			
			:param useVsync: A boolean that indicates whether vertical sync should be on (true) or off (false).
			
	.. rst-class:: collapsible
	
		.. cpp:function:: Vector2D GraphicsEngine::GetViewport() const
		
			Retrieve the current dimensions of the viewport.
			
			:return: A vector containing the dimensions of the viewport, in pixels.
			
	.. rst-class:: collapsible
	
		.. cpp:function:: void GraphicsEngine::SetViewport(unsigned width, unsigned height)
		
			Set the dimensions of the viewport to the specified width and height.
			
			:param width: The desired width of the viewport, in pixels.
			:param height: The desired height of the viewport, in pixels.
	
	.. rst-class:: collapsible
	
		.. cpp:function:: FontSystem* GraphicsEngine::GetFontSystem() const
		
			Retrieve the font system.
			
			.. note:: This function is only used internally by other text rendering classes.
			
			:return: A pointer to the font system.
		