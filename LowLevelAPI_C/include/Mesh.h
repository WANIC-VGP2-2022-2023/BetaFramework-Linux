//------------------------------------------------------------------------------
//
// File Name:	Mesh.h
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

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

// Types of data found in vertex buffers for default mesh
typedef enum BufferType
{
	// DO NOT MODIFY - used by default vertices/meshes
	BT_Position,
	BT_Color,
	BT_TextureCoordinate,

	// Keep this one last
	BT_Num
}BufferType;

// Different methods for drawing meshes
typedef enum MeshDrawMode
{
	MDM_Points = 0,
	MDM_Lines,
	MDM_LinesStrip,
	MDM_LinesLoop,
	MDM_Triangles,

	// Keep this one last
	MDM_Num
}MeshDrawMode;

typedef struct Mesh Mesh;

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Destroy a mesh
BE_C_API void MeshDestroy(Mesh** mesh);

// Draw the mesh
BE_C_API void MeshDraw(Mesh* mesh);

// Get/Set
BE_C_API unsigned MeshGetNumVertices(const Mesh* mesh);
BE_C_API const char* MeshGetName(const Mesh* mesh);
BE_C_API void MeshSetName(Mesh* mesh, const char* name);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif
