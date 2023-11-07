//------------------------------------------------------------------------------
//
// File Name:	ShaderProgram.h
// Author(s):	Jeremy Kings
// Project:		Beta Engine
// Course:		WANIC VGP2
//
// Copyright © 2018-2019 DigiPen (USA) Corporation.
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
// Forward Declarations:
//------------------------------------------------------------------------------

typedef struct Vector2D Vector2D;
typedef struct Matrix2D Matrix2D;
typedef struct ShaderProgram ShaderProgram;
typedef struct Color Color;

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Attempt to load a shader program from a file.
// Params:
//   vertexShaderFile = The file name of the vertex shader.
//   pixelShaderFile = The file name of the pixel shader.
// Returns:
//   A valid program if successful, nullptr if unsuccessful.
BE_C_API ShaderProgram* ShaderProgramCreate(const char* vertexShaderFile, const char* fragmentShaderFile);

// Destructor
BE_C_API void ShaderProgramDestroy(ShaderProgram** program);

// Set this as the active shader program
BE_C_API void ShaderProgramUse(const ShaderProgram* program);

// Set values of uniform variables
BE_C_API void ShaderProgramSetUniformInt(const ShaderProgram* program, const char* name, int value);
BE_C_API void ShaderProgramSetUniformFloat(const ShaderProgram* program, const char* name, float value);
BE_C_API void ShaderProgramSetUniformBool(const ShaderProgram* program, const char* name, bool value);
BE_C_API void ShaderProgramSetUniformVector(const ShaderProgram* program, const char* name, const Vector2D* value);
BE_C_API void ShaderProgramSetUniformColor(const ShaderProgram* program, const char* name, const Color* value);
BE_C_API void ShaderProgramSetUniformMatrix(const ShaderProgram* program, const char* name, const Matrix2D* value);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif
