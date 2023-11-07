//------------------------------------------------------------------------------
//
// File Name:	glm_include.h
// Author(s):	Jeremy Kings
// Project:		Beta Engine
// Course:		WANIC VGP2
//
// Copyright � 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once

#pragma warning(push)

// Non-standard extension
#pragma warning(disable : 4201)
// Conditional expression is constant
#pragma warning(disable : 4127)

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Matrix3D.h"

inline glm::mat4& GlmMatrix(const Beta::Matrix3D& m)
{
	return static_cast<glm::mat4&>(*static_cast<glm::mat4*>(m.data));
}

#pragma warning(pop)

//------------------------------------------------------------------------------
