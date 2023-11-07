Game Object Manager
===================

.. cpp:namespace:: Beta

.. cpp:class:: GameObjectManager : public BetaObject

Methods
-------

.. cpp:function:: GameObjectManager::GameObjectManager(Space* space)
   
   Constructor of the Game Object Manager. Reserves space for game objects.
   
   :param space: A pointer to the space where this object manager will be managing objects.

.. cpp:function:: GameObjectManager::~GameObjectManager()
   
   Clears out the Quadtree of the space and frees up object resources.

.. cpp:function:: void GameObjectManager::Update(float dt) override
   
   Update all objects in the active game objects list.
   
   :param  dt: Change in time (in seconds) since the last game loop.

.. cpp:function:: void GameObjectManager::Draw(void) override
   
   Draw all game objects in the active game object list.

.. cpp:function:: void GameObjectManager::Shutdown(void) override
   
   Shutdown the game object manager, destroying all active objects.

.. cpp:function:: void GameObjectManager::AddObject(GameObject& gameObject)
   
   Add a game object to the active game object list.
   
   :param gameObject: A reference to the game object to be added to the list.

.. cpp:function:: GameObject* GameObjectManager::GetObjectByName(const std::string& name) const
   
   Returns a pointer to the first game object matching the specified name.
   
   .. note:: This search does not include objects that are marked for destruction but have not yet been destroyed.
   
   :param name: The name of the object to be returned, if found.
   
   :return: The pointer to the object that was found or null if the object was not found.

.. cpp:function:: unsigned GameObjectManager::GetObjectCount(const std::string& objectName) const
   
   Returns the number of objects with the given name.
   
   .. note:: If no name is specified (objectName = ""), then the count of all objects will be given.
   
   :param objectName: The name of the objects that should be counted.
   
   :return: The number of objects matching the specified name.

.. cpp:function:: template<typename ComponentType> void GameObjectManager::GetAllObjectsWithComponent(Array<GameObject*>& results)
   
   Retrieves all objects that contain the given component.
   
   :type ComponentType: The type of component to search for.
   
   :param results: A reference to an array where the results of the search will be stored.

.. cpp:function:: bool GameObjectManager::IsQuadtreeEnabled() const
   
   Test whether the quadtree is currently enabled for this object manager.
   
   :return: True if the quadtree is enabled and false otherwise.

.. cpp:function:: void GameObjectManager::SetQuadtreeEnabled(bool enabled)
   
   Enable or disable use of the quadtree data structure for collision detection.
   
   :param enabled: True to enable and false to disable.

.. cpp:function:: void GameObjectManager::CastRay(const Vector2D& start, const Vector2D& direction, float distance, Array<Physics::CastResult>& results, const std::string& filter = "")
   
   Perform a raycast and return all objects that intersect with the ray.
   
   :param start: The starting point of the ray in world coordinates.
   
   :param direction: The direction of the ray.
   
   :param distance: How far to check in the given direction. Distance must be positive.
   
   :param results: The vector in which the results will be stored.
   
   :param filter: Objects with this name are excluded. This is case sensitive.

.. cpp:function:: GameObject* GameObjectManager::CastRayClosest(const Vector2D& start, const Vector2D& direction, float distance, const std::string& filter = "")
   
   Perform a raycast and return the first object hit.
   
   :param start: The starting point of the ray in world coordinates.
   
   :param direction: The direction of the ray.
   
   :param distance: How far to check in the given direction. Distance must be positive.
   
   :param filter: Objects with this name are excluded. This is case sensitive.
   
   :return: A pointer to the first object hit by the ray.