//------------------------------------------------------------------------------
//
// File Name:	MeshFactory.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		Beta Engine
// Course:		WANIC VGP2
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "MeshFactory.h"

#include "EngineModulesGlobal.h"
#include "Vertex.h"
#include "../../LowLevelAPI/include/MeshFactory.h"
#include "../..//LowLevelAPI/include/Vertex.h"

//------------------------------------------------------------------------------

using Beta::MeshFactory;

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Loads a mesh from a file
Mesh* MeshFactoryCreateFromFile(const char* filename)
{
	return reinterpret_cast<Mesh*>(Beta::meshFactory->CreateMeshFromFile(filename));
}

// Create a textured quad mesh using the Beta Framework.
// Params:
//	 halfSize = The XY distance of the vertices from the origin, defaults to 0.5 on each axis.
//   textureSize = The UV size of the mesh, relative to texture coordinates, defaults to 1.0 for each.
// Returns:
//	 If the memory allocation was successful, then return a pointer to the allocated memory,
//	   else return nullptr.
Mesh* MeshFactoryCreateQuadTextured(const Vector2D* textureSize, const Vector2D* halfSize)
{
	return reinterpret_cast<Mesh*>(Beta::meshFactory->CreateQuadMesh({ textureSize->x, textureSize->y },
		{ halfSize->x, halfSize->y }));
}

// Create a colored quad mesh using the Beta Framework.
// Params:
//	 halfSize = The XY distance of the vertices from the origin, defaults to 0.5 on each axis.
//	 color = The color assigned to each vertex (for meshes not using textures).
// Returns:
//	 If the memory allocation was successful, then return a pointer to the allocated memory,
//	   else return nullptr.
Mesh* MeshFactoryCreateQuadColored(const Color* color, const Vector2D* halfSize)
{
	return reinterpret_cast<Mesh*>(Beta::meshFactory->CreateQuadMesh({ color->r, color->g, color->b, color->a },
		{ halfSize->x, halfSize->y }));
}

// Stops adding vertices and returns a dynamically allocated mesh.
// Params:
//   drawMode = How to interpret the vertices of the mesh (usually as triangles).
// Returns:
//   A mesh pointer that contains all the vertices added since StartCreate was called.  
Mesh* MeshFactoryEndCreate(MeshDrawMode drawMode)
{
	return reinterpret_cast<Mesh*>(Beta::meshFactory->EndCreate(static_cast<Beta::MeshDrawMode>(drawMode)));
}

// Add a colored vertex to the mesh being created.
void MeshFactoryAddVertex(const Vertex* vertex)
{
	Beta::meshFactory->AddVertex(*reinterpret_cast<const Beta::Vertex*>(vertex));
}

// Add a textured triangle (three vertices) to the mesh being created.
void MeshFactoryAddTriangle(const Vertex* v0, const Vertex* v1, const Vertex* v2)
{
	Beta::meshFactory->AddTriangle(*reinterpret_cast<const Beta::Vertex*>(v0),
		*reinterpret_cast<const Beta::Vertex*>(v1), *reinterpret_cast<const Beta::Vertex*>(v2));
}

//------------------------------------------------------------------------------
