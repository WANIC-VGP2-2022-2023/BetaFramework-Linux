EngineCore
==========

.. rst-class:: searchdescription

.. cpp:namespace:: Beta

EngineCore is the main module used to run programs using the Beta framework. The primary responsibility 
of EngineCore is maintaining and providing access to the low level systems of Beta, including :cpp:class:`WindowSystem`, 
:cpp:class:`Input`, :cpp:class:`GraphicsEngine`, :cpp:class:`FrameRateController`, and :cpp:class:`DebugDraw`. EngineCore is a 
`singleton <https://sourcemaking.com/design_patterns/singleton>`_ class; users access it through its static 
:cpp:func:`Engine::GetInstance` function.

The EngineCore class also allows the user to provide their own modules via the AddModule function. System modules 
must inherit from the :cpp:class:`BetaObject` abstract class. Custom modules are added to the engine before it 
starts. Once all modules have been added, they can be initialized using the engine's Initialize function. After 
the engine starts, it calls each module's Update function every frame. When the engine shuts down, it shuts down 
each module in the reverse order of how they were added. This is done to reduce issues arising from dependencies between
modules.

.. cpp:class:: EngineCore

Methods
-------

	.. rst-class:: collapsible
	
		.. cpp:function:: void EngineCore::Start(const StartupSettings& settings = StartupSettings())
		
			Start the simulation loop and run until the quit state is reached.
			
			:param settings: The settings to use when starting the application.
	
	.. rst-class:: collapsible
	
		.. cpp:function:: void EngineCore::Stop()
		
			Stop the engine, shutting down all systems.
	
	.. rst-class:: collapsible

		.. cpp:function:: template<class ModuleType> ModuleType* EngineCore::AddModule()

			Add an additional module to the engine. This module will be initialized when the engine starts,
			updated every tick, and then shut down when the engine stops running.
			
			:type ModuleType: The class type of the module being added to the engine. Custom modules must implement the :cpp:class:`BetaObject` interface and have a default constructor.
			
			:return: A pointer to the module that was created and added to the engine. 
	
	.. rst-class:: collapsible

		.. cpp:function:: template<class ModuleType> ModuleType* EngineCore::GetModule()
		
			Retrieve the module with the given type if it exists. There are several specializations 
			of this function to ensure that retrieving built-in modules is done in an efficient manner.
			
			.. note:: Rather than calling this method directly, the use of the *EngineGetModule* macro is recommended. *EngineGetModule(ModuleType)* expands to *Beta::EngineCore::GetInstance().GetModule<ModuleType>()*.
			
			:type ModuleType: The class type of the module being retrieved.
			
			:return: A pointer to the specified module if it exists, otherwise *nullptr*.
			
	.. rst-class:: collapsible

		.. cpp:function:: std::string& EngineCore::GetFilePath() const

			Return the path to the assets directory. This directory, simply *Assets* by default, is 
			where the majority of systems will look when attempting to load or save files.
			
			:return: A string containing the relative path to the location of assets loaded by the engine.
			
	.. rst-class:: collapsible

		.. cpp:function:: void EngineCore::SetFilePath(const std::string& directory)

			Set the path used for loading and saving assets.
			
			:param directory:  The relative path to the master assets directory. Textures, objects, shaders, and sound files should be placed in subdirectories within this directory.
	
	.. rst-class:: collapsible
	
		.. cpp:function:: bool EngineCore::DoesCloseOnEscape() const
		
			Test whether the engine is currently set to close when escape is pressed.
			
			:return: True if the engine closes on escape, false otherwise.
			
	.. rst-class:: collapsible
	
		.. cpp:function:: void EngineCore::SetCloseOnEscape(bool closeOnEscape)
		
			Set whether pressing the escape key will close the program.
			
			:param closeOnEscape: A boolean specifying whether the engine should close on escape.
			
	.. rst-class:: collapsible
	
		.. cpp:function:: bool EngineCore::IsRunning() const
		
			Test whether the engine is currently running. This function is primarily for internal use; 
			there should be no need to check this anywhere other than within the engine itself once the engine has started.
			
			:return: True if the engine is currently running, false otherwise.
			
	.. rst-class:: collapsible
	
		.. cpp:function:: static EngineCore& EngineCore::GetInstance()
		
			Return the singular instance of the *EngineCore* class. *EngineCore* cannot be intantiated 
			by the user, so this function is the only way other methods of the class should be accessed.
			
			:return: A reference to the EngineCore instance.