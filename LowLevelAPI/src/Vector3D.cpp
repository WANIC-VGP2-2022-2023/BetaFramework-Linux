//------------------------------------------------------------------------------
//
// File Name:	Vector3D.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Vector3D.h"

#include "glm_include.h"

#include "Vector2D.h"

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	Vector3D::Vector3D()
		: data(new glm::vec3)
	{

	}

	Vector3D::Vector3D(const Vector3D& other)
		: data(new glm::vec3(*static_cast<glm::vec3*>(other.data)))
	{
	}

	Vector3D::Vector3D(float value)
		: data(new glm::vec3(value))
	{
	}

	Vector3D::Vector3D(float x, float y, float z)
		: data(new glm::vec3(x, y, z))
	{
	}

	Vector3D::Vector3D(const Vector2D& v, float z)
		: data(new glm::vec3(v.x, v.y, z))
	{
	}

	Vector3D::~Vector3D()
	{
		glm::vec3* v = static_cast<glm::vec3*>(data);
		delete v;
	}
}

//------------------------------------------------------------------------------
