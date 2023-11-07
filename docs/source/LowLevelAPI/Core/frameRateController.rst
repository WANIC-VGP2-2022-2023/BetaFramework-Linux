FrameRateController
===================

.. rst-class:: searchdescription

.. cpp:namespace:: Beta

The FrameRateController class is responsible for limiting the frame rate and determining the 
change in time between frames, or delta time (dt). This dt value is then used in *Update*
functions of many systems to ensure most behavior is time-based rather than frame-based.

Access to the FrameRateController class can be obtained using *EngineGetModule(FrameRateController)*, 
provided both FrameRateController.h and Engine.h (or something that includes them) are included.

.. cpp:class:: FrameRateController

Methods
-------

	.. rst-class:: collapsible
	
		.. cpp:function:: FrameRateController::FrameRateController()
		
			Construct an instance of the FrameRateController class. Initializes all time-related variables (usually to 0).
	
	.. rst-class:: collapsible
	
		.. cpp:function:: void FrameRateController::Initialize(unsigned framerateCap = 200)
		
			Grab the initial time stamp and set the maximum frame rate for the frame rate controller.
			
			:param framerateCap: The maximum number of frames to render per second.
	
	.. rst-class:: collapsible

		.. cpp:function:: void FrameRateController::FrameEnd() const

			End the current frame, update the delta time and frame count. Frame 
			rate controller will wait until enough time has passed if an individual
			frame does not take enough time.
	
	.. rst-class:: collapsible

		.. cpp:function:: float FrameRateController::GetFrameTime() const

			Get the change in time from the previous frame to the current. Use this 
			to make sure things such as movement and physics happen at a consistent speed 
			across multiple devices or framerates. It should be used to set a dt variable 
			for use in physics and other time-sensitive calculations and computations.
		
			:return: The time between the start of the previous frame and the start of the current frame.
			
	.. rst-class:: collapsible

		.. cpp:function:: unsigned long FrameRateController::GetFrameCount() const

			Gets the number of frames that have elapsed since the engine started running.
		
			:return: The total frames that have elapsed.
	