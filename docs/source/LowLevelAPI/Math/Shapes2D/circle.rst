Circle
======

.. cpp:namespace:: Beta

The Circle struct represents a circle. It is most commonly used for
debug drawing, as well as intersection calculations (using Intersection2D 
in the High-Level API).

.. cpp:struct:: Circle

Methods
-------

	.. rst-class:: collapsible
	
		.. cpp:function:: Circle::Circle()
		
			Construct an instance of the Circle struct. Calling the default constructor
			will result in a circle centered at (0, 0) with a radius of 0.
	
	.. rst-class:: collapsible
	
		.. cpp:function:: Circle::Circle(const Vector2D& center, float radius)
		
			Construct an instance of the Circle struct using the provided data.
			
			:param center: The center point of the circle.
			:param radius: The radius of the circle.

Properties
----------

	.. rst-class:: collapsible
	
		.. cpp:member:: Vector2D Circle::center
		
		The center point of the circle.
		
	.. rst-class:: collapsible
	
		.. cpp:member:: float Circle::radius
		
		The radius of the circle.
		