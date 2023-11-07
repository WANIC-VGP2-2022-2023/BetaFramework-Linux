StartupSettings
===============

.. cpp:namespace:: Beta

The StartupSettings struct represents the initial settings that can be specified when starting
a program using the Beta API. This includes things such as window resolution, full screen mode,
frame limiting, and other common settings. Many of these can also be changed at runtime, but
this provides an easy way to specify these before the program starts.

.. cpp:struct:: StartupSettings

Methods
-------

	.. rst-class:: collapsible
	
		.. cpp:function:: StartupSettings::StartupSettings()
		
			Construct an instance of the StartupSettings struct. Default values for each property are
			listed below in the *Properties* section.

Properties
----------

	.. rst-class:: collapsible
	
		.. cpp:member:: unsigned StartupSettings::windowWidth
		
			Width of the window. Defaults to 800. Set to 0 to use native resolution.
		
	.. rst-class:: collapsible
	
		.. cpp:member:: unsigned StartupSettings::windowHeight
		
			Height of the window. Defaults to 600. Set to 0 to use native resolution.
		
	.. rst-class:: collapsible
	
		.. cpp:member:: unsigned StartupSettings::framerateCap
		
			Maximum framerate. Defaults to 60. Has no effect if vSync is enabled.
		
		.. cpp:member:: bool StartupSettings::showWindow
		
			Whether to show the window immediately when the engine starts. Defaults to true.
			
		.. cpp:member:: bool StartupSettings::debugConsole
		
			Whether to create a debug console in Debug mode. Defaults to true.
		
		.. cpp:member:: bool StartupSettings::fullscreen
		
			Whether to start in fullscreen mode. Defaults to false.
			
		.. cpp:member:: bool StartupSettings::allowMaximize
		
			Whether the user is allowed to maximize the window. Defaults to false.
			
		.. cpp:member:: bool StartupSettings::closeOnEscape
		
			Whether to close the application when escape is pressed. Defaults to true.
			
		.. cpp:member:: bool StartupSettings::vSync
			
			Whether to synchronize sync the frame rate with the refresh rate. Defaults to true.
			