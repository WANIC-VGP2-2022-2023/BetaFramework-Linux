//------------------------------------------------------------------------------
//
// File Name:	MeshFactory.cpp
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
#include "MeshFactory.h"

#include <glad.h>
#include "../../glfw/src/glfw3.h"
#include "Vector2D.h"
#include "Vertex.h"			// constructors
#include "EngineCore.h"		// GetFilePath

//------------------------------------------------------------------------------

namespace Beta
{
	// The current relative path for loading textures
	std::string MeshFactory::meshPath = "Meshes/";

	namespace
	{
		Color ReadHexToColor(std::fstream& stream)
		{
			int hexValue = 0;
			stream >> std::hex >> hexValue;

			Color color;
			color.r = ((hexValue >> 24) & 0xFF) / 255.0f;  // Extract the RR byte
			color.g = ((hexValue >> 16) & 0xFF) / 255.0f;  // Extract the GG byte
			color.b = ((hexValue >> 8) & 0xFF) / 255.0f;   // Extract the BB byte
			color.a = ((hexValue) & 0xFF) / 255.0f;        // Extract the AA byte

			return color;
		}
	}

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	MeshFactory::MeshFactory()
		: floatsPerPosition(2), floatsPerColor(4), floatsPerTextureCoord(2),
		positionStride(floatsPerPosition * sizeof(float)), colorStride(floatsPerColor * sizeof(float)),
		textureCoordStride(floatsPerTextureCoord * sizeof(float))
	{
	}

	// Loads a mesh from a file
	Mesh* MeshFactory::CreateMeshFromFile(const std::string & filename)
	{
		Mesh* mesh = nullptr;

		std::string fullPath = EngineCore::GetInstance().GetFilePath() + meshPath + filename;
		std::fstream stream(fullPath, std::ios_base::in);

		if (stream.is_open())
		{
			MeshFactory& meshFactory = *EngineCore::GetInstance().GetModule <MeshFactory>();

			std::string token;
			stream >> token;

			if (token == "Mesh")
			{
				// Construct the mesh using data read from the stream.
				int vertexCount;
				stream >> vertexCount;
				for (int i = 0; i < vertexCount; ++i)
				{
					// Read the vertex position (in local space).
					Vector2D pos;
					stream >> pos.x;
					stream >> pos.y;

					// Read the vertex color.
					Color color = ReadHexToColor(stream);

					// Read the vertex UV values.
					Vector2D uv;
					stream >> uv.x;
					stream >> uv.y;

					// Add the vertex to the mesh.
					meshFactory.AddVertex(Vertex(pos, color, uv));
				}
				// Construct the mesh object.
				mesh = meshFactory.EndCreate();
			}
			else if (token == "Quad")
			{
				// Read the half size of the quad mesh.
				Vector2D halfSize;
				stream >> halfSize.x;
				stream >> halfSize.y;

				// Read the column and row sizes
				unsigned col, row;
				stream >> col;
				stream >> row;

				std::string name;
				stream >> name;

				// Construct the quad mesh object.
				mesh = meshFactory.CreateQuadMesh(Vector2D(1.0f / col, 1.0f / row), halfSize);
				mesh->SetName(name);
			}
		}

		return mesh;
	}

	// Create a textured quad mesh using the Beta Framework.
	// Params:
	//   textureSize = The UV size of the mesh, relative to texture coordinates, defaults to 1.0 for each.
	//	 halfSize = The XY distance of the vertices from the origin, defaults to 0.5 on each axis.
	// Returns:
	//	 If the memory allocation was successful, then return a pointer to the allocated memory,
	//	   else return nullptr.
	Mesh* MeshFactory::CreateQuadMesh(const Vector2D & textureSize, const Vector2D & halfSize)
	{
		AddTriangle(
			Vertex(-halfSize, Vector2D(0.0f, textureSize.y)),
			Vertex(Vector2D(halfSize.x, -halfSize.y), textureSize),
			Vertex(Vector2D(-halfSize.x, halfSize.y), Vector2D()));
		AddTriangle(
			Vertex(Vector2D(halfSize.x, -halfSize.y), textureSize),
			Vertex(halfSize, Vector2D(textureSize.x, 0.0f)),
			Vertex(Vector2D(-halfSize.x, halfSize.y), Vector2D()));

		Mesh * mesh = MeshFactory::EndCreate();

		return mesh;
	}

	// Create a colored quad mesh using the Beta Framework.
	// Params:
	//	 color = The color assigned to each vertex (for meshes not using textures).
	//	 halfSize = The XY distance of the vertices from the origin, defaults to 0.5 on each axis.
	// Returns:
	//	 If the memory allocation was successful, then return a pointer to the allocated memory,
	//	   else return nullptr.
	Mesh * MeshFactory::CreateQuadMesh(const Color & color, const Vector2D & halfSize)
	{
		AddTriangle(
			Vertex(-halfSize, color), Vertex(Vector2D(halfSize.x, -halfSize.y), color),
			Vertex(Vector2D(-halfSize.x, halfSize.y), color));
		AddTriangle(
			Vertex(Vector2D(halfSize.x, -halfSize.y), color), Vertex(halfSize, color),
			Vertex(Vector2D(-halfSize.x, halfSize.y), color));

		Mesh * mesh = MeshFactory::EndCreate();

		return mesh;
	}

	// Stops adding vertices and returns a dynamically allocated mesh.
	// Returns:
	//    A mesh pointer that contains all the vertices added since StartCreate was called.
	Mesh * MeshFactory::EndCreate(MeshDrawMode drawMode)
	{
		Mesh* mesh;

		// allocate triangle list
		mesh = new Mesh(static_cast<unsigned>(positions.Size()), drawMode);
		size_t numVertices = mesh->numVertices;

		// Create a buffer
		glGenBuffers(BT_Num, mesh->bufferIDs);

		// Create vertex array object to simplify future rendering
		glGenVertexArrays(1, &mesh->arrayObjectID);
		glBindVertexArray(mesh->arrayObjectID);

		// Bind buffer 0: Position
		glBindBuffer(GL_ARRAY_BUFFER, mesh->bufferIDs[BT_Position]);
		glBufferData(GL_ARRAY_BUFFER, numVertices * positionStride, positions.Data(), GL_STATIC_DRAW);
		glVertexAttribPointer(BT_Position, floatsPerPosition, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(BT_Position);

		// Bind buffer 1: Color
		glBindBuffer(GL_ARRAY_BUFFER, mesh->bufferIDs[BT_Color]);
		glBufferData(GL_ARRAY_BUFFER, numVertices * colorStride, colors.Data(), GL_STATIC_DRAW);
		glVertexAttribPointer(BT_Color, floatsPerColor, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(BT_Color);

		// Bind buffer 2: Texture Coordinate
		glBindBuffer(GL_ARRAY_BUFFER, mesh->bufferIDs[BT_TextureCoordinate]);
		glBufferData(GL_ARRAY_BUFFER, numVertices * textureCoordStride, textureCoords.Data(), GL_STATIC_DRAW);
		glVertexAttribPointer(BT_TextureCoordinate, floatsPerTextureCoord, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(BT_TextureCoordinate);

		// Clear vectors
		positions.Clear();
		colors.Clear();
		textureCoords.Clear();

		// Unbind indices
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		return mesh;
	}

	// Add a colored vertex to the mesh being created.
	void MeshFactory::AddVertex(const Vertex & vertex)
	{
		positions.PushBack(vertex.position);
		colors.PushBack(vertex.color);
		textureCoords.PushBack(vertex.textureCoords);
	}

	// Add a textured triangle (three vertices) to the mesh being created.
	void MeshFactory::AddTriangle(const Vertex & v0, const Vertex & v1, const Vertex & v2)
	{
		AddVertex(v0);
		AddVertex(v1);
		AddVertex(v2);
	}

	// Add a 2D colored triangle (three vertices) to the mesh being created.
	void MeshFactory::AddTriangle(const Vector2D & v0, const Vector2D & v1, const Vector2D & v2, const Color & color)
	{
		AddTriangle(Vertex(v0, color), Vertex(v1, color), Vertex(v2, color));
	}
}

//------------------------------------------------------------------------------
