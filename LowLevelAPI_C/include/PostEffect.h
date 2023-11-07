//------------------------------------------------------------------------------
//
// File Name:	PostEffect.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		Beta Engine
// Course:		WANIC VGP2
//
// Copyright © 2019 DigiPen (USA) Corporation.
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

typedef struct ShaderProgram ShaderProgram;
typedef struct PostEffect PostEffect;

typedef void(*EffectFunction)(PostEffect*);
typedef void(*EffectFunctionDt)(PostEffect*, float);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Constructor
// Params:
//   fragmentShader = The filename of the fragment shader for this effect.
//   vertexShader = The filename of the vertex shader for this effect. Defaults to simple vertex shader.
//   init = Pass in a function to reset variables as needed.
//   update = Pass in a function to modify variables before they are sent to the shader.
//   draw = Pass in a function to send custom data to the shader.
//   data = Pass in dynamically allocated extra data if needed. Will be freed when the effect is destroyed.
BE_C_API PostEffect* PostEffectCreate(const char* fragmentShader, const char* vertexShader,
	EffectFunction init, EffectFunctionDt update, EffectFunction draw, void* data);

// Destructor
BE_C_API void PostEffectDestroy(PostEffect** effect);

// Test whether this effect is currently being used by the renderer.
BE_C_API bool PostEffectIsActive(const PostEffect* effect);

// Get the shader program used by this effect.
BE_C_API const ShaderProgram* PostEffectGetProgram(const PostEffect* effect);

// Get any extra data that was passed in when the effect was initialized.
BE_C_API void* PostEffectGetData(const PostEffect* effect);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif
