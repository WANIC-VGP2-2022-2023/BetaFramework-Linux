MeshFactory
===========

.. cpp:namespace:: Beta

The MeshFactory class is responsible for creating 2D meshes. Meshes created by the factory can consist of any number
of vertices. Typically, meshes are created from series of triangles rather than individual vertices. For simple
2D sprites, a quad can easily be created using :cpp:func:`MeshFactory::CreateQuadmesh`.

Access to the MeshFactory class can be obtained using *EngineGetModule(MeshFactory)*, provided both MeshFactory.h and
Engine.h (or something that includes them) are included.

.. cpp:class:: MeshFactory

Methods
-------

	.. rst-class:: collapsible
	
		.. cpp:function:: MeshFactory::MeshFactory()
		
			Construct an instance of the MeshFactory class. Initializes mesh factory variables
			to a clean state. Under normal circumstances, users should not need to call
			this function directly (or indirectly by creating an instance of the class).
	
	.. rst-class:: collapsible

		.. cpp:function:: void MeshFactory::CreateMeshFromFile(const std::string& filename)
		
			Create a mesh object from a text file.
			
			.. note:: Mesh files should follow the following format:
			
				| *MeshName*
				| *numberOfVertices*
				| *posX posY colorHex texU texV*
				| *posX posY colorHex texU texV*
				| *posX posY colorHex texU texV*
				| *(etc., for as many vertices as needed)*
			
			:param filename: The name of the file containing the mesh data.
	