#//------------------------------------------------------------------------------
//
// File Name:	Vertex.cpp
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
#include "Vertex.h"

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	Vertex::Vertex()
	{
	}

	// Colored vertex 2D
	Vertex::Vertex(const Vector2D& position, const Color& color)
		: position(position), color(color)
	{
	}

	// Textured vertex 2D
	Vertex::Vertex(const Vector2D& position, const Vector2D& textureCoords)
		: position(position), color(Colors::White), textureCoords(textureCoords)
	{
	}

	// Complete vertex 3D
	Vertex::Vertex(const Vector2D& position, const Color& color, const Vector2D& textureCoords)
		: position(position), color(color), textureCoords(textureCoords)
	{
	}
}

//------------------------------------------------------------------------------
