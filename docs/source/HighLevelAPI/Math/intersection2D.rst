Intersection2D
==============

The functions listed here are not members of a class but instead belong to the Intersection2D namespace. That is they can be accessed at any time and are here to aid in whatever you may be trying to accomplish.

.. cpp:namespace:: Beta::Intersection2D

Functions
---------

.. cpp:function:: bool PointRectangleIntersection(const Vector2D& point, const BoundingRectangle& rect)
   
   Checks whether a point and rectangle are intersecting.
   
   :param point: The point that may be intersecting the rectangle.
   
   :param rect: The rectangle to be checked.
   
   :return: True if the point is within or touching the rectangle, false otherwise.

.. cpp:function:: bool RectangleRectangleIntersection(const BoundingRectangle& rect1, const BoundingRectangle& rect2)
   
   Checks whether two rectangles are intersecting.
   
   :param rect1: The first rectangle.
   
   :param rect2: The second rectangle.
   
   :return: True if there is an intersection, false otherwise.

.. cpp:function:: bool RectangleCircleIntersection(const BoundingRectangle& rect, const Circle& circle)
   
   Checks whether a rectangle and a circle are intersecting.
   
   :param rect: The rectangle.
   
   :param circle: The circle.
   
   :return: True if there is an intersection, false otherwise.

.. cpp:function:: bool RectangleLineIntersection(const BoundingRectangle& first, const LineSegment& second, float& t)
   
   Checks whether a rectangle and line are intersecting.
   
   :param first: The rectangle.
   
   :param second: Start and end of second line segment.
   
   :param t: An out parameter where the T value of intersection (for checking closest intersection) will be stored.
   
   :return: True if the line is intersecting the rectangle and false otherwise.

.. cpp:function:: bool CircleCircleIntersection(const Circle& circle1, const Circle& circle2)
   
   Checks whether two circles are intersecting.
   
   :param circle1: The first circle.
   
   :param circle2: The second circle.
   
   :return: True if the two circles are intersecting, false otherwise.

.. cpp:function:: bool StaticCircleLineIntersection(const LineSegment& line, const Circle& circle, float& t)
   
   Checks whether a static circle and line are intersecting.
   
   :param line: Start and end of first line segment.
   
   :param circle: Start and end of second line segment (can be moving point or circle).
   
   :param t: T value of intersection (for checking closest intersection).
   
   :return: True if intersection, false otherwise.

.. cpp:function:: bool MovingCircleLineIntersection(const LineSegment& staticLine, const LineSegment& movingCircle, float radius, Vector2D& intersection, float& t)
   
   Checks whether a moving point/circle and line are intersecting.
   
   .. note:: A point is just a circle with a radius of 0.
   
   :param staticLine: Start and end of first line segment.
   
   :param movingCircle: Start and end of second line segment (can be moving point or circle).
   
   :param radius: Radius of the circle, you can use a value of 0 for a point.
   
   :param intersection: Out parameter for an intersection point, if there is an intersection.
   
   :param t: Out parameter for the T value of intersection (for checking closest intersection).
   
   :return: True if intersecting, false otherwise.

.. cpp:function:: void MovingCircleLineReflection(Core::Transform& transform, Physics::RigidBody& physics, const LineSegment& staticLine, const LineSegment& movingCircle, float radius, const Vector2D& intersection)
   
   Modifies object's position, velocity, and rotation using simple point-line reflection.
   
   :param transform: Transform of the object that is being reflected.
   
   :param physics: Physics of the object being reflected.
   
   :param staticLine: Start and end of first line segment.
   
   :param movingCircle: Start and end of second line segment (can be moving point or circle).
   
   :param radius: Radius of the circle. (Use 0 for points.)
   
   :param intersection: Intersection point of the line and circle. 

.. cpp:function:: bool LineLineIntersection(const LineSegment& first, const LineSegment& second, Vector2D& intersection, float& t)
   
   Check whether two line segments intersect.
   
   :param first: Start and end of first line segment.
   
   :param second: Start and end of second line segment.
   
   :param intersection: Intersection point, if any.
   
   :param t: T value of intersection (for checking closest intersection).
   
   :return: True if intersection, false otherwise.