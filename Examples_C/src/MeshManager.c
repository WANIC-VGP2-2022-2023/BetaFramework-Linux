//------------------------------------------------------------------------------
//
// File Name:	MeshManager.c
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 4
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BetaLow.h"

#include "Mesh.h"
#include "MeshManager.h"
#include "Stream.h"
#include "Vector2D.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

#define meshListSize 10

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct MeshManager
{
	// The number of game objects currently in the list.
	unsigned int meshCount;

	// The maximum number of game objects ever in the list, at any given moment.
	//unsigned int meshMax;

	// A list of all currently loaded meshes.
	// This list can be a fixed-length array (minimum size of 10 entries)
	// or a dynamically-sized array, such as a linked list.
	Mesh*	meshList[meshListSize];

} MeshManager;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

static MeshManager meshes;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static void MeshManagerAdd(Mesh* mesh);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Initialize the Mesh Manager.
// (NOTE: Make sure to initialize all memory to zero.)
void MeshManagerInit()
{
	// Clear out the entire memory structure.
	memset(&meshes, 0, sizeof(meshes));
}

// Create a mesh and add it to the mesh manager.
// 1: Use sprintf_s() to construct a path name using meshName
//	   (HINT: The correct path name should be constructed using "Data/&s.txt".)
// 2: Call StreamOpen(), passing the pathname
// 3: If the stream was opened successfully,
//	  a: Call MeshRead() to construct a mesh using data read from the file
//	  b: Call MeshManagerAdd(), passing the created mesh
//	  c: Return the created mesh
// Params:
//	 meshName = The name of the mesh to be created.
// Returns:
//	 If the mesh was created successfully,
//	   then return a pointer to the created mesh,
//	   else return NULL.
Mesh* MeshManagerBuild(const char * meshName)
{
	Mesh* mesh = NULL;

	// Construct the file pathname.
	char pathName[FILENAME_MAX] = "";
	sprintf_s(pathName, _countof(pathName), "Data/%s.txt", meshName);

	// Read the contents of the mesh from the stream.
	mesh = MeshFactoryCreateFromFile(pathName);
	if (mesh != NULL)
	{
		MeshManagerAdd(mesh);
	}

	return mesh;
}

// Free all meshes in the Mesh Manager.
// (NOTE: Make sure to call AEGfxMeshFree() to free each mesh.)
// (HINT: The list should contain nothing but NULL pointers once this function is done.)
void MeshManagerFreeAll()
{
	for (int i = 0; i < meshListSize; i++)
	{
		if (meshes.meshList[i] != NULL)
		{
			MeshDestroy(&meshes.meshList[i]);
		}
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

void MeshManagerAdd(Mesh* mesh)
{
	if (mesh)
	{
		if (meshes.meshCount < _countof(meshes.meshList))
		{
			meshes.meshList[meshes.meshCount] = mesh;
			++meshes.meshCount;
		}
	}
}
