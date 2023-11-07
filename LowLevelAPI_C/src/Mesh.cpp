//------------------------------------------------------------------------------
//
// File Name:	Mesh.cpp
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
#include "Mesh.h"

#include "../../LowLevelAPI/include/Mesh.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

void MeshDestroy(Mesh** mesh)
{
	delete reinterpret_cast<Beta::Mesh*>(*mesh);
	*mesh = NULL;
}

// Draw the mesh
void MeshDraw(Mesh* mesh)
{
	reinterpret_cast<Beta::Mesh*>(mesh)->Draw();
}

unsigned MeshGetNumVertices(const Mesh* mesh)
{
	return reinterpret_cast<const Beta::Mesh*>(mesh)->GetNumVertices();
}

const char* MeshGetName(const Mesh* mesh)
{
	return reinterpret_cast<const Beta::Mesh*>(mesh)->GetName().c_str();
}

void MeshSetName(Mesh* mesh, const char* name)
{
	reinterpret_cast<Beta::Mesh*>(mesh)->SetName(name);
}

//------------------------------------------------------------------------------
