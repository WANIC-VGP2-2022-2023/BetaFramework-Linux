Tilemap
=======

.. cpp:namespace:: Beta

.. cpp:class:: Tilemap : public Serializable

Methods
-------

.. cpp:function:: Tilemap::Tilemap(unsigned width = 0, unsigned height = 0, const std::string & name = "", int** data = nullptr)
   
   Create a tilemap from the given parameters.
   
   :param width: The width of the map.
   
   :param height: The height of the map.
   
   :param name: The name of the map.
   
   :param data: The array containing the map data.

.. cpp:function:: Tilemap::~Tilemap()
   
   Cleans up allocated tilemap data.

.. cpp:function:: unsigned Tilemap::GetWidth() const
   
   Gets the width of the map (in tiles).
   
   :return: The width of the map.

.. cpp:function:: unsigned Tilemap::GetHeight() const
   
   Gets the height of the map (in tiles).
   
   :return: The height of the map.

.. cpp:function:: int Tilemap::GetMinIndexX() const
   
   Get the minimum index of the width of the tilemap.
   
   :return: The minimum index.

.. cpp:function:: int Tilemap::GetMinIndexY() const
   
   Get the minimum index of the height of the tilemap.
   
   :return: The minimum index.

.. cpp:function:: int Tilemap::GetMaxIndexX() const
   
   Get the maximum index of the width of the tilemap.
   
   :return: The maximum index.

.. cpp:function:: int Tilemap::GetMaxIndexY() const
   
   Get the maximum index of the height of the tilemap.
   
   :return: The maximum index.

.. cpp:function:: int Tilemap::GetCellValue(int column, int row) const
   
   Gets the value of the cell in the map with the given indices.
   
   :param column: The column of the cell.
   
   :param row: The row of the cell.
   
   :return: -1 if the indices are invalid, 0 if the cell is empty, or a positive integer otherwise.

.. cpp:function:: void Tilemap::SetCellValue(int column, int row, int value, bool resize = false, bool verbose = false)
   
   Sets the value of the cell in the map with the given indices.
   
   :param column: The column of the cell.
   
   :param row: The row of the cell.
   
   :param value: The new value for the cell.
   
   :param resize: Whether to automatically resize the map if the selected indices are out of bounds.
   
   :param verbose: Whether to print the tilemap to the console after changing the map.

.. cpp:function:: const std::string& Tilemap::GetName() const
   
   Return the name of the tilemap.
   
   :return: A reference to the string containing the name of the tilemap.

.. cpp:function:: void Tilemap::SetName(const std::string& name)
   
   Sets the name of the tilemap.
   
   :param name: A reference to the new name.

.. cpp:function:: void Tilemap::Resize(unsigned width, unsigned height, int xShift = 0, int yShift = 0)
   
   Resizes the map based on the given parameters.
   
   :param width: The new width of the map.
   
   :param height: The new height of the map.
   
   :param xShift: Amount to shift elements horizontally.
   
   :param yShift: Amount to shift elements vertically.

.. cpp:function:: void Tilemap::Clear()
   
   Resets all tiles to 0.
   
   .. warning:: This change is permanent and will clear all data that has been entered.

.. cpp:function:: void Tilemap::ShrinkToFit()
   
   Shrinks map so that any columns or rows on the edge of the map that are completely empty are removed.

.. cpp:function:: void Tilemap::AddObject(GameObject* object, int x, int y)
   
   Add a game object to the map.
   
   :param object: The game object being added to the map.
   
   :param x: The column in which to place the object.
   
   :param y: The row in which to place the object.

.. cpp:function:: GameObject* Tilemap::RemoveObject(int x, int y)
   
   Remove an object from the map.
   
   :param x: The column from which to remove the object.
   
   :param y: The row from which to remove the object.
   
   :return: A pointer to the object that was removed, or nullptr if no object was at that location.

.. cpp:function:: const Array<ObjectInMap>& Tilemap::GetObjects() const
   
   Retrieves the current list of objects in the map.
   
   :return: A reference to an array containing all the objects in the map.

.. cpp:function:: std::ostream& Tilemap::operator<<(std::ostream& stream, const Tilemap& map)
   
   A friend function to overload the output operator for a Tilemap.
   
   :param stream: The output stream reference.
   
   :param map: A reference to the map that will be output.
   
   :return: A reference to the output stream object so that the operator can be chained together with more output operations.

.. cpp:function:: void Tilemap::Serialize(FileStream& stream) const
   
   Save tilemap data to file.
   
   :param stream: The stream object used to save the tilemap's data.

.. cpp:function:: void Tilemap::Deserialize(FileStream& stream)
   
   Load tilemap data from file.
   
   :param stream: The stream object used to load the tilemap's data.
   
.. cpp:function:: static TilemapManager& Tilemap::GetTilemapManager()

	Returns the tilemap manager, which can be used to load new tilemaps
	or obtain existing tilemap resources.
	
	.. note:: To simplify the process of finding tilemap resources, use the *ResourceGetTilemap* macro. This macro expands to *Tilemap::GetTilemapManager().GetResource(t)*, where *t* is the name of the tilemap resource.
	
	:return: A reference to the tilemap manager.

Properties
----------

.. cpp:member:: static const int invalidIndex
   
   Check indices against this value to validate. If an index is invalid it will be this value.
