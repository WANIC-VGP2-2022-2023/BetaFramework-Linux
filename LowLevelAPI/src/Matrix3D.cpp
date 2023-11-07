//------------------------------------------------------------------------------
//
// File Name:	Matrix3D.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		Project 3 - Transformations
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Matrix3D.h"

#include "glm_include.h"

#include "Matrix2D.h"

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	Matrix3D::Matrix3D()
		: data(new glm::mat4)
	{
	}

	Matrix3D::Matrix3D(const Matrix3D& other)
		: data(new glm::mat4(*static_cast<glm::mat4*>(other.data)))
	{
	}

	// Conversion to GLM matrix
	Matrix3D::Matrix3D(const Matrix2D& m)
		: data(new glm::mat4)
	{
		glm::mat4& matrix = static_cast<glm::mat4&>(*static_cast<glm::mat4*>(data));

		// Start with GLM identity matrix
		matrix = glm::mat4(1.0f);

		// Copy over data
		matrix[0].x = m.m[0][0]; matrix[1].x = m.m[0][1]; matrix[3].x = m.m[0][2];
		matrix[0].y = m.m[1][0]; matrix[1].y = m.m[1][1]; matrix[3].y = m.m[1][2];
		matrix[0].z = m.m[2][0]; matrix[1].z = m.m[2][1]; matrix[3].z = 0.0f;
	}

	Matrix3D::~Matrix3D()
	{
		glm::mat4* matrix = static_cast<glm::mat4*>(data);
		delete matrix;
	}
}
