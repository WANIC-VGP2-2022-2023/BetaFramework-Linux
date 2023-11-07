//------------------------------------------------------------------------------
//
// File Name:	MeshFactory.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		Beta Engine
// Course:		WANIC VGP2
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Vector2D.h"
#include "Color.h"
#include "Mesh.h" // MDM_Triangles

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

typedef struct Vertex Vertex;

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load a mesh from a text file
// Example mesh format:
//   MeshName
//   numberOfVertices
//   posX posY colorHex texU texV
//   posX posY colorHex texU texV
//   posX posY colorHex texU texV
//   (etc., for as many vertices as needed)
BE_C_API Mesh* MeshFactoryCreateFromFile(const char* filename);

// Create a textured quad mesh using the Beta Framework.
// Params:
//	 halfSize = The XY distance of the vertices from the origin, defaults to 0.5 on each axis.
//   textureSize = The UV size of the mesh, relative to texture coordinates, defaults to 1.0 for each.
// Returns:
//	 If the memory allocation was successful, then return a pointer to the allocated memory,
//	   else return nullptr.
BE_C_API Mesh* MeshFactoryCreateQuadTextured(const Vector2D* textureSize, const Vector2D* halfSize);

// Create a colored quad mesh using the Beta Framework.
// Params:
//	 halfSize = The XY distance of the vertices from the origin, defaults to 0.5 on each axis.
//	 color = The color assigned to each vertex (for meshes not using textures).
// Returns:
//	 If the memory allocation was successful, then return a pointer to the allocated memory,
//	   else return nullptr.
BE_C_API Mesh* MeshFactoryCreateQuadColored(const Color* color, const Vector2D* halfSize);

// Stops adding vertices and returns a dynamically allocated mesh.
// Params:
//   drawMode = How to interpret the vertices of the mesh (usually as triangles).
// Returns:
//   A mesh pointer that contains all the vertices added since StartCreate was called.  
BE_C_API Mesh* MeshFactoryEndCreate(MeshDrawMode drawMode);

// Add a colored vertex to the mesh being created.
BE_C_API void MeshFactoryAddVertex(const Vertex* vertex);

// Add a textured triangle (three vertices) to the mesh being created.
BE_C_API void MeshFactoryAddTriangle(const Vertex* v0, const Vertex* v1, const Vertex* v2);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif
