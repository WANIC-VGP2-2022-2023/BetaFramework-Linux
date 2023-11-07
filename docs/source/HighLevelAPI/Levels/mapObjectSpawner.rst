Map Object Spawner
==================

.. cpp:namespace:: Beta

.. cpp:class:: MapObjectSpawner : public Component

Methods
-------

.. cpp:function:: MapObjectSpawner::MapObjectSpawner()
   
   Constructs a new map spawner component.

.. cpp:function:: void MapObjectSpawner::Initialize() override
   
   Initialize this component.

.. cpp:function:: void MapObjectSpawner::SetTilemap(ConstTilemapPtr map)
   
   Sets the tilemap data that will be used by this component.
   
   :param map: A pointer to the tilemap resource.

.. cpp:function:: void MapObjectSpawner::Serialize(FileStream& stream) const
   
   Save map object spawner data to file.
   
   :param stream: The stream object used to save the map object spawner's data.

.. cpp:function:: void MapObjectSpawner::Deserialize(FileStream& stream)
   
   Load map object spawner data from file.
   
   :param stream: The stream object used to load the map object spawner's data.