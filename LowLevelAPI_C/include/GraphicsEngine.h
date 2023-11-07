//------------------------------------------------------------------------------
//
// File Name:	GraphicsEngine.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		Beta Engine
// Course:		WANIC VGP2
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Color.h"
#include "Vector2D.h"

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

typedef struct Matrix2D Matrix2D;
typedef struct PostEffect PostEffect;
typedef struct ShaderProgram ShaderProgram;
typedef struct Camera Camera;
typedef struct Texture Texture;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

typedef enum BlendMode
{
	BM_None = 0,
	BM_Blend,
	BM_Add,
	BM_Multiply,
	BM_Screen,

	// Keep this one last
	BM_Num
}BlendMode;

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

/////////////////////
// COMMON SETTINGS //
/////////////////////

// Otherwise uncolored pixels will have this color
BE_C_API Color GraphicsGetBackgroundColor();
BE_C_API void GraphicsSetBackgroundColor(const Color* color);

// Color to blend with whole screen
BE_C_API Color GraphicsGetScreenTintColor();
BE_C_API void GraphicsSetScreenTintColor(const Color* color);

// Color to blend with the current sprite's color
BE_C_API void GraphicsSetSpriteBlendColor(const Color* color);

// Returns a 1x1 white texture.
BE_C_API const Texture* GraphicsGetDefaultTexture();

// Transform
BE_C_API void GraphicsSetTransform(const Vector2D* translation, const Vector2D* scale,
	float rotation);
BE_C_API void GraphicsSetTransformWithDepth(const Vector2D* translation, const Vector2D* scale,
	float rotation, float depth);
BE_C_API void GraphicsSetTransformMatrix(const Matrix2D* matrix);
BE_C_API void GraphicsSetTransformMatrixWithDepth(const Matrix2D* matrix, float depth);

// Camera
BE_C_API Camera* GraphicsGetDefaultCamera();

// Returns the default sprite shader (usually for the purpose of setting uniforms)
BE_C_API const ShaderProgram* GraphicsGetSpriteShader();

// Returns the font shader
BE_C_API const ShaderProgram* GraphicsGetFontShader();

///////////////////////
// ADVANCED SETTINGS //
///////////////////////

// Sets how sprites are blended
BE_C_API void GraphicsSetBlendMode(BlendMode mode);

// Adds a post-processing effect. Effects are applied sequentially,
// starting with the first that was added.
// Params:
//   effect =  The effect to add to the current list of effects.
BE_C_API void GraphicsPushEffect(PostEffect* effect);
// Removes the most recently added effect.
BE_C_API void GraphicsPopEffect();
// Removes a specific effect.
BE_C_API void GraphicsRemoveEffect(PostEffect* effect);
// Removes all effects that are currently active.
BE_C_API void GraphicsClearEffects();

// Test whether vertical sync is currently on
BE_C_API bool GraphicsGetUseVsync();
// Turn vertical sync on or off - will cause performance issues on some machines
BE_C_API void GraphicsSetUseVSync(bool useVsync);

// Get the dimensions of the viewport
BE_C_API Vector2D GraphicsGetViewport(void);
// Set the dimensions of the viewport.
// Params:
//   width = The new width of the viewport.
//   height = The new height of the viewport.
BE_C_API void GraphicsSetViewport(int width, int height);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif
