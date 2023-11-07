Quadtree
========

.. cpp:namespace:: Beta

.. cpp:class:: Quadtree

Methods
-------

.. cpp:function:: Quadtree::Quadtree()
   
   Default constructor. Initializes all variables to 0.

.. cpp:function:: Quadtree::Quadtree(const BoundingRectangle& bounds, int level, int maxObjects)
   
   Constructor for quadtree.
   
   :param bounds: The area encompassed by this node.
   
   :param level: The level of this node in the tree. Child nodes will have a level of one less.
   
   :param maxObjectsPerNode: The maximum number of game objects in this node before it will be split.

.. cpp:function:: Quadtree::~Quadtree()
   
   Destructor for quadtree. Calls Clear function.

.. cpp:function:: void Quadtree::Draw() const
   
   Draws each of the nodes in the tree (for debug purposes only).

.. cpp:function:: void Quadtree::AddObject(GameObject* object)
   
   Adds an object to the tree
   
   :param object: The game object being added to the tree.

.. cpp:function:: void Quadtree::RetrieveNearbyObjects(GameObject* object, Array<GameObject*>& results)
   
   Retrieve all objects from the tree that could collide with a given object.
   
   :param object: The object for which we want to retrieve nearby objects.
   
   :param results: The list that stores the nearby objects.

.. cpp:function:: void Quadtree::CastRay(const LineSegment& ray, Array<CastResult>& results, const std::string& filter)
   
   Retrieve all objects from the tree that intersect with a given ray.
   
   :param ray: The ray for which we want to retrieve objects that it intersects.
   
   :param results: Intersecting objects and their distance from the ray's origin.
   
   :param filter: Objects with this name are excluded.
   
   .. :note:: The string is case sensitive and the filtering system may be reworked in the future.

.. cpp:function:: void Quadtree::Clear()
   
   Removes all objects from the tree.