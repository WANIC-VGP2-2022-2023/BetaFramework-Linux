Input
=====

.. rst-class:: searchdescription

.. cpp:namespace:: Beta

The Input class is responsible for processing keyboard and mouse input from the operating system. 
It stores data from the current frame as well as the previous frame, which users can query through 
various accessors.

Key codes for the various keyboard keys and mouse buttons can be found in Microsoft's `Windows API documentation <https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes>`_.

Access to the Input class can be obtained using *EngineGetModule(Input)*, provided both Input.h and
Engine.h (or something that includes them) are included.

.. cpp:class:: Input

Methods
-------

	.. rst-class:: collapsible
	
		.. cpp:function:: Input::Input()
		
			Construct an instance of the Input class. Initializes keyboard and cursor data
			to a clean state. Under normal circumstances, users should not need to call
			this function directly (or indirectly by creating an instance of the class).
	
	.. rst-class:: collapsible
	
		.. cpp:function:: void Input::Update()
		
			Update keyboard and cursor data. This function is called automatically 
			by the Engine once it starts.
	
	.. rst-class:: collapsible

		.. cpp:function:: bool Input::CheckHeld(unsigned key) const

			Test if a key or mouse button was down during the previous update and is still down this update.
			
			:param key: The character or virtual key code that is being checked. Examples: 'A' for the A key, VK_ENTER for the enter key, or VK_LBUTTON for the left mouse button.
			
			:return: True if the key is being held, false otherwise.
	
	.. rst-class:: collapsible

		.. cpp:function:: bool Input::CheckTriggered(unsigned key) const

			Test if a key or mouse button started being down this update, but was not down during
			the previous update.
			
			:param key: The character or virtual key code that is being checked. Examples: 'A' for the A key, VK_ENTER for the enter key, or VK_LBUTTON for the left mouse button.
			
			:return: True if the key is being triggered, false otherwise.
			
	.. rst-class:: collapsible

		.. cpp:function:: bool Input::CheckReleased(unsigned key) const

			Test if a key is no longer down this update, but was down during the previous update.
			
			:param key: The character or virtual key code that is being checked. Examples: 'A' for the A key, VK_ENTER for the enter key, or VK_LBUTTON for the left mouse button.
			
			:return: True if the key is being released, false otherwise.
			
	.. rst-class:: collapsible

		.. cpp:function:: bool Input::IsKeyDown(unsigned key) const

			Test if a key is down during this specific update. 
			
			.. note:: It is usually more useful to examine input over at least two frames. Under normal circumstances, you should use :cpp:func:`Input::CheckHeld` instead of this function.
			
			:param key: The character or virtual key code that is being checked. Examples: 'A' for the A key, VK_ENTER for the enter key, or VK_LBUTTON for the left mouse button.
			
			:return: True if the key is down this update, false otherwise.
	
	.. rst-class:: collapsible

		.. cpp:function:: bool Input::WasKeyDown(unsigned key, unsigned framesBefore = 1) const

			Test if a key was down during a specific update in the past.
			
			:param key: The character or virtual key code that is being checked. Examples: 'A' for the A key, VK_ENTER for the enter key, or VK_LBUTTON for the left mouse button.
			
			:param framesBefore: How far back into the past to go when checking the status of the key. The upper limit of this is determined by the input buffer size, which can be changed using :cpp:func:`Input::SetBufferSize`.
			
			:return: True if the key was down during the specified update, false otherwise.
	
	.. rst-class:: collapsible

		.. cpp:function:: Vector2D Input::GetCursorPosition() const

			Get the current location of the mouse in screen coordinates.
			
			:return: A Vector2D containing the position of the mouse.
	
	.. rst-class:: collapsible

		.. cpp:function:: void Input::SetCursorPosition(const Vector2D& position)

			Set the mouse's location to a new position.
			
			:param position: The new position for the mouse.
	
	.. rst-class:: collapsible

		.. cpp:function:: float Input::GetMouseScrollAmount() const

			Get the amount that the mouse has scrolled this frame.
			
			:return: A float representing the amount the mouse has scrolled.
	
	.. rst-class:: collapsible

		.. cpp:function:: void Input::SetMouseScrollAmount(float amount)

			Cause the mouse to scroll a specific amount this frame.
			
			:param amount: The amount that the mouse should scroll.
			
	.. rst-class:: collapsible

		.. cpp:function:: void Input::SetBufferSize(unsigned size)

			Set the number of frames of input to store. This affects how many updates in the past can be viewed using :cpp:func:`Input::WasKeyDown`.
			
			:param size: The number of frames to store (should be > 1).

Example Code
------------

Checking for keyboard and mouse input using the Input module::

	#pragma once
	
	#include <BetaObject.h>

	#include <Input.h> // CheckTriggered, CheckHeld, CheckReleased

	class InputTest : public Beta::BetaObject
	{
	public:
	   InputTest() : BetaObject("InputTest")
	   {
	   }

	   void Update(float dt) override
	   {
	      using namespace Beta;
	      
	      // Obtain necessary modules
	      Input& input = *EngineGetModule(Input);

	      // Check if the left mouse button starts being clicked
	      if(input.CheckTriggered(VK_LBUTTON))
	      {
	         std::cout << "Left mouse clicked" << std::endl;
	      }
	      
	      // Check if the right arrow key is held
	      if(input.CheckHeld(VK_RIGHT))
	      {
	         std::cout << "Right arrow held" << std::endl;
	      }
	      
	      // Check if the A key was released
	      if(input.CheckReleased('A'))
	      {
	         std::cout << "A key released" << std::endl;
	      }
	   }
	};
	