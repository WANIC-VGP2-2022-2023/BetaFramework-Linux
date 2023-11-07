//------------------------------------------------------------------------------
//
// File Name:	Mesh.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		Beta Engine
// Course:		WANIC VGP2
//
// Copyright � 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Mesh.h"

#include <glad.h>
#include "../../glfw/src/glfw3.h"
#include "Vector2D.h"
#include "GraphicsEngine.h" // GetShader

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	Mesh::Mesh(unsigned numVertices, MeshDrawMode mode, unsigned numBuffers)
		: numVertices(numVertices), numBuffers(numBuffers), drawMode(GL_TRIANGLES), arrayObjectID(0)
	{
		bufferIDs = new unsigned[numBuffers];
		SetDrawMode(mode);
	}

	Mesh::~Mesh()
	{
		glDeleteVertexArrays(1, &arrayObjectID);
		glDeleteBuffers(numBuffers, bufferIDs);
		delete[] bufferIDs;
	}

	void Mesh::Draw() const
	{
		// Bind the vertex array object
		glBindVertexArray(arrayObjectID);
		// Draw all the things
		glDrawArrays(drawMode, 0, numVertices);
		// Unbind the VAO
		glBindVertexArray(0);
	}

	unsigned Mesh::GetNumVertices() const
	{
		return numVertices;
	}

	void Mesh::UpdatePositionBuffer(const Vector2D* positions)
	{
		// Update content of VBO memory
		const GLsizei bufferSize = sizeof(Vector2D) * static_cast<GLsizei>(numVertices);
		//glBindVertexArray(arrayObjectID);
		glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[BT_Position]);
		glBufferSubData(GL_ARRAY_BUFFER, 0,
			bufferSize, positions);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//glBindVertexArray(0);
	}

	const std::string& Mesh::GetName() const
	{
		return name;
	}

	void Mesh::SetName(const std::string& _name)
	{
		name = _name;
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	void Mesh::SetDrawMode(MeshDrawMode mode)
	{
		switch (mode)
		{
		case MDM_Points:
			drawMode = GL_POINTS;
			break;

		case MDM_Lines:
			drawMode = GL_LINES;
			break;

		case MDM_Lines_Strip:
			drawMode = GL_LINE_STRIP;
			break;

		case MDM_Lines_Loop:
			drawMode = GL_LINE_LOOP;
			break;

		case MDM_Triangles:
			drawMode = GL_TRIANGLES;
			break;

		default:
			std::cout << "Mesh Draw Mode " << mode << " is invalid!" << std::endl;
			return;
		}
	}
}

//------------------------------------------------------------------------------
