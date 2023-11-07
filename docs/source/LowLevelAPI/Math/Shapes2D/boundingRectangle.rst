BoundingRectangle
=================

.. cpp:namespace:: Beta

The BoundingRectangle struct represents an axis-aligned rectangle
commonly used for debug drawing, as well as intersection calculations 
(using Intersection2D in the High-Level API).

.. cpp:struct:: BoundingRectangle

Methods
-------

	.. rst-class:: collapsible
	
		.. cpp:function:: BoundingRectangle::BoundingRectangle()
		
			Construct an instance of the BoundingRectangle struct. Calling the default constructor
			will result in a rectangle with a center at (0, 0) and extents of [0, 0].
	
	.. rst-class:: collapsible
	
		.. cpp:function:: BoundingRectangle::BoundingRectangle(const Vector2D& center, const Vector2D& extents)
		
			Construct an instance of the BoundingRectangle struct using the provided data. 
			This will also pre-compute the locations of the four sides of the rectangle.
			
			:param center: The center point of the rectangle.
			:param extents: Half the width and height of the rectangle.

Properties
----------

	.. rst-class:: collapsible
	
		.. cpp:member:: Vector2D BoundingRectangle::center
		
			The center point of the rectangle.
		
	.. rst-class:: collapsible
	
		.. cpp:member:: Vector2D BoundingRectangle::extents
		
			Half the width and height of the rectangle.
		
	.. rst-class:: collapsible
	
		.. cpp:member:: float BoundingRectangle::left
		
			The x-coordinate of the left side of the rectangle.
		
		.. cpp:member:: float BoundingRectangle::right
		
			The x-coordinate of the right side of the rectangle.
			
		.. cpp:member:: float BoundingRectangle::top
		
			The y-coordinate of the top side of the rectangle.
		
		.. cpp:member:: float BoundingRectangle::bottom
		
			The y-coordinate of the bottom side of the rectangle.
			