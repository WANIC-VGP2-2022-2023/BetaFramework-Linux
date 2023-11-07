WindowSystem
============

.. rst-class:: searchdescription

.. cpp:namespace:: Beta

The WindowSystem class is responsible for creating and maintaining the main application window and, in Debug builds, the debug console. Access to the WindowSystem class can be obtained using *EngineGetModule(WindowSystem)*, 
provided both WindowSystem.h and Engine.h (or something that includes them) are included.

.. cpp:class:: WindowSystem

Methods
-------

	.. rst-class:: collapsible
	
		.. cpp:function:: WindowSystem::WindowSystem()
		
			Construct an instance of the WindowSystem class. This initializes GLFW and other window-related functionality.
	
	.. rst-class:: collapsible
	
		.. cpp:function:: WindowSystem::~WindowSystem()
		
			Destroy an instance of the WindowSystem class. This destroys the window and terminates GLFW.
	
	.. rst-class:: collapsible
	
		.. cpp:function:: void WindowSystem::Initialize(const StartupSettings& settings)
		
			Initialize the WindowSystem module. This creates the window and, in Debug builds, creates the debug console.
			
			:param settings: The settings to use when creating the window.
	
	.. rst-class:: collapsible
	
		.. cpp:function:: void WindowSystem::Draw()
			
			End the frame and draw.
		
	.. rst-class:: collapsible
	
		.. cpp:function:: void WindowSystem::SetWindowTitle(const std::string& text)
		
			Set the title of the window used by the Beta application.
			
			:param text: The string that the window title will be set to.

	.. rst-class:: collapsible
	
		.. cpp:function:: bool WindowSystem::DoesWindowExist() const
		
			Test whether the window for this program still exists.
			
			:return: True if the window exists, false otherwise.
			
	.. rst-class:: collapsible
	
		.. cpp:function:: bool WindowSystem::IsFullScreen() const
		
			Test whether the application is in fullscreen mode.
			
			:return: True if the application is in fullscreen mode, false if it is in windowed mode.
	
	.. rst-class:: collapsible
	
		.. cpp:function:: void WindowSystem::SetFullScreen(bool fullscreen)
		
			Set the fullscreen mode of the application.
			
			:param fullscreen: A boolean indicating whether the application should be fullscreen (true) or windowed (false).
	
	.. rst-class:: collapsible
	
		.. cpp:function:: void WindowSystem::SetResolution(unsigned width, unsigned height)

			Set the resolution of the window.
		
			:param width: The new width of the window.
			:param height: The new height of the window.
			
	.. rst-class:: collapsible
	
		.. cpp:function:: Vector2D WindowSystem::GetResolution() const

			Get the current resolution of the window.
			
			:return: A Vector2D containing the current resoluton of the window.
	