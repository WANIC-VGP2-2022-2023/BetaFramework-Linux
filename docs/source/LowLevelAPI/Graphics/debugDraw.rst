DebugDraw
==============

.. cpp:namespace:: Beta

The DebugDraw class is responsible for drawing simple primitives used for debugging purposes. These primitives are only rendered when the program is built and launched in debug mode. In release mode, any requests for debug drawing are safely ignored.

Access to the DebugDraw class can be obtained using *EngineGetModule(DebugDraw)*, provided both DebugDraw.h and
Engine.h (or something that includes them) are included.

.. cpp:class:: DebugDraw

Methods
-------

	.. rst-class:: collapsible
	
		.. cpp:function:: DebugDraw::DebugDraw()
		
			Construct an instance of the DebugDraw class. This initializes debug draw variables
			to a clean state.
	
	.. rst-class:: collapsible
	
		.. cpp:function:: DebugDraw::~DebugDraw()
		
			Destroy an instance of the DebugDraw class. This function deallocates all resources
			allocated directly by the debug draw system, such as primitive meshes.
	
	.. rst-class:: collapsible

		.. cpp:function:: void DebugDraw::Initialize(unsigned width, unsigned height)

			Initialize the graphics engine. This involves creating the meshes used by
			debug draw primitives.
	
	.. rst-class:: collapsible

		.. cpp:function:: void DebugDraw::Draw()
		
			Draw all debug draw primitives requested during this frame. This function is used
			internally by the engine near the end of each frame.

	.. rst-class:: collapsible
	
		.. cpp:function:: void DebugDraw::AddLineToList(const Vector2D& start, const Vector2D& end, const Color& color = Colors::White)
			
			Add a line segment to a list of lines to be drawn this frame. 
			
			.. note:: Actual drawing of this line will not occur unless *EndLineList* is also called.
			
			:param start: The starting point of the line segment.
			:param end: The end point of the line segment.
			:param color: The color to use when drawing the line. Defaults to white.
			
	.. rst-class:: collapsible
	
		.. cpp:function:: void DebugDraw::EndLineList(float zDepth = 0.0f)
			
			Create a line list primitive out of line segments previously added using *AddLineToList*. 
			This line list is then drawn along with other primitives toward the end of the frame.
			
			:param zDepth: The depth at which to draw the line list when using a perspective projection. Defaults to 0.0f.
			
	.. rst-class:: collapsible
	
		.. cpp:function:: void DebugDraw::AddCircle(const Vector2D& center, float radius, const Color& color = Colors::White, float zDepth = 0.0f)
			
			Add a circle primitive to the list of circles to be drawn this frame.
			
			:param center: The center point of the circle.
			:param radius: The radius of the circle.
			:param color: The color to use when drawing this circle. Defaults to white.
			:param zDepth: The depth at which to draw the circle when using a perspective projection. Defaults to 0.0f.
	
	.. rst-class:: collapsible
	
		.. cpp:function:: void DebugDraw::AddRectangle(const Vector2D& center, const Vector2D& extents, const Color& color = Colors::White, float zDepth = 0.0f)
			
			Add a rectangle primitive to the list of rectangles to be drawn this frame.
			
			:param center: The center point of the rectangle.
			:param extents: The extents of the rectangle, i.e. half the width and height of the rectangle.
			:param color: The color to use when drawing this rectangle. Defaults to white.
			:param zDepth: The depth at which to draw the rectangle when using a perspective projection. Defaults to 0.0f.
			
	.. rst-class:: collapsible
	
		.. cpp:function:: void DebugDraw::SetCamera(Camera& camera)
		
			Set the camera used for debug drawing. Any shapes added before the next
			call to SetCamera will use the provided camera.
			
			:param camera: The camera used to determine where debug objects will be drawn.
		
	.. rst-class:: collapsible
	
		.. cpp:function:: void DebugDraw::SetEnabled(bool enabled)
			
			Enable or disable the debug draw system. If disabled, calls to debug draw functions will be ignored, even in debug mode.
			
			:param enabled: A boolean indicating whether to enable (true) or disable (false) debug draw.
			
	.. rst-class:: collapsible
	
		.. cpp:function:: bool DebugDraw::IsEnabled()
		
			Test whether the debug draw system is currently enabled.
			
			:return: True if debug drawing is enabled, false otherwise.
			
Example Code
------------

Drawing a circle and a line using DebugDraw::

	#pragma once
	
	#include <BetaObject.h>

	#include <DebugDraw.h> // AddCircle, AddLineToList

	class DebugDrawTest : public Beta::BetaObject
	{
	public:
	  DebugDrawTest() : BetaObject("DebugDrawTest")
	  {
	  }

	  void Update(float dt) override
	  {
	    using namespace Beta;
		
		// Obtain necessary modules, etc.
	    DebugDraw& debug = *EngineGetModule(DebugDraw);

	    // Draw an aqua circle at 0,0.5 with radius 2
	    Vector2D center = Vector2D(0.0f, 0.5f);
	    float radius = 2.0f;
	    debug.AddCircle(center, radius,
		  Colors::Aqua);

	    // Draw a red line from -1,0 to 2,1.
	    Vector2D start = Vector2D(-1, 0);
	    Vector2D end = Vector2D(2, 1);
	    debug.AddLineToList(start, end, Colors::Red);
	    debug.EndLineList();
	  }
	};
	