Sprite Tilemap
==============

.. cpp:namespace:: Beta

.. cpp:class:: SpriteTilemap : public Sprite

Methods
-------

.. cpp:function:: SpriteTilemap::SpriteTilemap()
   
   Create a new sprite for use with a tilemap.

.. cpp:function:: void SpriteTilemap::Initialize() override
   
   Get component dependencies.

.. cpp:function:: void SpriteTilemap::Update(float dt) override
   
   Update area data.
   
   :param dt: Change in time (in seconds) since the last game loop.

.. cpp:function:: void SpriteTilemap::Draw() override
   
   Draw the tilemap using the SpriteTilemap's sprite source. 
   
   .. note:: If no sprite source is present, the default (white) will be used for all tiles.

.. cpp:function:: void SpriteTilemap::SetTilemap(Levels::ConstTilemapPtr map)
   
   Sets the tilemap data that will be used by the sprite.
   
   :param map: A pointer to the tilemap resource.

.. cpp:function:: void SpriteTilemap::Serialize(FileStream& stream) const
   
   Save sprite tilemap data to file.
   
   :param stream: The stream object used to save the sprite tilemap's data.

.. cpp:function:: void SpriteTilemap::Deserialize(FileStream& stream)
   
   Load sprite tilemap data from file.
   
   :param stream: The stream object used to load the sprite tilemap's data.