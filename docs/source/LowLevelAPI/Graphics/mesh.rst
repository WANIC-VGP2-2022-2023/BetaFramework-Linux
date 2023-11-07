Mesh
=======

.. cpp:namespace:: Beta

The Mesh class is responsible for determining which vertex data
is used when using meshes to draw sprites and text. Creation of
Mesh objects is handled by the :cpp:class:`MeshFactory` rather than
the Mesh class itself.

.. cpp:class:: Mesh

Methods
-------
	
	.. rst-class:: collapsible
	
		.. cpp:function:: Mesh::~Mesh()
		
			Destroy an instance of the Mesh class. This function deallocates all resources
			allocated directly by the mesh, which involves several calls to
			the internal rendering API (in this case, OpenGl).
			
	.. rst-class:: collapsible
	
		.. cpp:function:: void Mesh::Draw() const
		
			Draw the mesh using the current settings for transform, texture, camera, etc.
			
	.. rst-class:: collapsible
	
		.. cpp:function:: unsigned Mesh::GetNumVertices() const
		
			Get the number of vertices contained by this mesh.
			
			:return: The number of vertices in the mesh.
	
	.. rst-class:: collapsible
	
		.. cpp:function:: void Mesh::UpdatePositionBuffer(const Vector2D* positions)
		
			Update the positions of the vertices using the given array.
			Currently only used by fonts. Updating mesh vertices at runtime is
			generally not recommended for efficiency reasons.
			
			:param positions: An array containing the new vertex positions. The size of this array is assumed to be equal to the value returned by :cpp:func:`Mesh::GetNumVertices`.
			
	.. rst-class:: collapsible
	
		.. cpp:function:: std::string& Mesh::GetName() const
		
			Get the name of this mesh.
			
			:return: The name of the mesh.
			
	.. rst-class:: collapsible
	
		.. cpp:function:: void Mesh::SetName(const std::string& name)
		
			Set the name of this mesh.
			
			:param name: The new name of the mesh.
			
			
			