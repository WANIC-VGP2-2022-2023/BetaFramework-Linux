Camera
=======

.. cpp:namespace:: Beta

The Camera class represents a specific view of the world space.
Changing properties of a camera, such as translation and field-of-view,
affects how objects are drawn when using that camera. The Camera class
supports both orthographic and perspective projection, but does not
expose camera rotation, as the Beta framework is only intended to be
used to make 2D games.

.. cpp:class:: Camera

Methods
-------

	.. rst-class:: collapsible
	
		.. cpp:function:: Camera::Camera()
		
			Construct an instance of the Camera class. Cameras use an orthographic 
			projection by default, have a translation of (0, 0) and a vertical size of 5 units.
	
	.. rst-class:: collapsible
	
		.. cpp:function:: Camera::~Camera()
		
			Destroy an instance of the Camera class.
			
	.. rst-class:: collapsible
	
		.. cpp:function:: void Camera::Use() const
		
			Set this camera as the active camera used when rendering.
			
	.. rst-class:: collapsible
	
		.. cpp:function:: const Vector2D& Camera::GetTranslation() const
		
			Return the current translation of the camera.
			
			:return: A vector containing the translation of the camera.
	
	.. rst-class:: collapsible
	
		.. cpp:function:: void Camera::SetTranslation(const Vector2D& translation)
		
			Set the translation of the camera.
			
			:param translation: A vector representing the new translation of the camera.
			
	.. rst-class:: collapsible
	
		.. cpp:function:: float Camera::GetFOV() const
		
			Return the current field of view angle of the camera. 
			
			.. note:: This data is only relevant if the camera is in perspective projection mode.
			
			:return: The field of view of the camera in degrees.
	
	.. rst-class:: collapsible
	
		.. cpp:function:: void Camera::SetFOV(float angle)
		
			Set the field of view angle of the camera.
			
			.. note:: This will not result in a change unless the camera is in perspective projection mode.
			
			:param angle: The angle to use for the camera's field of view, specified in degrees.
		
	.. rst-class:: collapsible
	
		.. cpp:function:: float Camera::GetVerticalSize() const
		
			Return the current height of the camera in world units. 
			
			.. note:: This data is only relevant if the camera is in orthographic projection mode.
			
			:return: The height of the camera in world units.
	
	.. rst-class:: collapsible
	
		.. cpp:function:: void Camera::SetVerticalSize(float angle)
		
			Set the height of the camera. This allows the user to "zoom" in and out when the camera is in orthographic projection mode. A higher vertical size means more of the world will be displayed when using this camera, while a lower size means less of the world will be visible.
			
			.. note:: This will not result in a change unless the camera is in orthographic projection mode.
			
			:param angle: The new height of the camera in world units.
	
	.. rst-class:: collapsible
	
		.. cpp:function:: void Camera::Reset()
		
			Reset this camera's properties to their default values. Translation will be (0, 0),
			FOV is set to 90 degrees, and vertical size is set to 5. This function does *not* change
			the current projection mode of the camera.
			
	.. rst-class:: collapsible
	
		.. cpp:function:: const BoundingRectangle Camera::GetScreenWorldDimensions() const
		
			Get the width and height of the viewport in world coordinates.
			
			:return: A vector containing the size of the viewport in world coordinates.
	
	.. rst-class:: collapsible
	
		.. cpp:function:: Vector2D Camera::ScreenToWorldPosition(const Vector2D& screenPosition) const
			
			Convert a position in screen coordinates to world coordinates.
		
			:return: A vector containing the world coordinates of the given screen position.