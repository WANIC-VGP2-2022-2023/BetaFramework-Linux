//------------------------------------------------------------------------------
//
// File Name:	ShaderProgram.cpp
// Author(s):	Jeremy Kings
// Project:		Beta Engine
// Course:		WANIC VGP2
//
// Copyright � 2018-2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "ShaderProgram.h"

#include "../../LowLevelAPI/include/ShaderProgram.h"
#include "../../LowLevelAPI/include/Vector2D.h"
#include "../../LowLevelAPI/include/Matrix2D.h"
#include "../../LowLevelAPI/include/Color.h"
#include "Vector2D.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Attempt to load a shader program from a file.
// Params:
//   vertexShaderFile = The file name of the vertex shader.
//   pixelShaderFile = The file name of the pixel shader.
// Returns:
//   A valid program if successful, nullptr if unsuccessful.
ShaderProgram* ShaderProgramCreate(const char* vertexShaderFile, const char* fragmentShaderFile)
{
	Beta::ShaderProgram* program =
		Beta::ShaderProgram::CreateProgramFromFile(vertexShaderFile, fragmentShaderFile);
	return reinterpret_cast<ShaderProgram*>(program);
}

// Destructor
void ShaderProgramDestroy(ShaderProgram** program)
{
	delete reinterpret_cast<Beta::ShaderProgram*>(*program);
	*program = NULL;
}

// Set this as the active shader program
void ShaderProgramUse(const ShaderProgram* program)
{
	reinterpret_cast<const Beta::ShaderProgram*>(program)->Use();
}

// Set values of uniform variables
void ShaderProgramSetUniformInt(const ShaderProgram* program, const char* name, int value)
{
	reinterpret_cast<const Beta::ShaderProgram*>(program)->SetUniform(name, value);
}

void ShaderProgramSetUniformFloat(const ShaderProgram* program, const char* name, float value)
{
	reinterpret_cast<const Beta::ShaderProgram*>(program)->SetUniform(name, value);
}

void ShaderProgramSetUniformBool(const ShaderProgram* program, const char* name, bool value)
{
	reinterpret_cast<const Beta::ShaderProgram*>(program)->SetUniform(name, static_cast<bool>(value));
}

void ShaderProgramSetUniformVector(const ShaderProgram* program, const char* name, const Vector2D* value)
{
	reinterpret_cast<const Beta::ShaderProgram*>(program)->SetUniform(
		name, {value->x, value->y});
}

void ShaderProgramSetUniformColor(const ShaderProgram* program, const char* name, const Color* value)
{
	reinterpret_cast<const Beta::ShaderProgram*>(program)->SetUniform(
		name, *reinterpret_cast<const Beta::Color*>(value));
}

void ShaderProgramSetUniformMatrix(const ShaderProgram* program, const char* name, const Matrix2D* value)
{
	reinterpret_cast<const Beta::ShaderProgram*>(program)->SetUniform(name,
		*reinterpret_cast<const Beta::Matrix2D*>(value));
}

//------------------------------------------------------------------------------
