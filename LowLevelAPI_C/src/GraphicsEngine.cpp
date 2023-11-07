//------------------------------------------------------------------------------
//
// File Name:	GraphicsEngine.h
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
#include "GraphicsEngine.h"

#include "EngineModulesGlobal.h"
#include "Vector2D.h"
#include "Matrix2D.h"
#include "../../LowLevelAPI/include/GraphicsEngine.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

/////////////////////
// COMMON SETTINGS //
/////////////////////

// Otherwise uncolored pixels will have this color
Color GraphicsGetBackgroundColor()
{
	Beta::Color bgColor =
		Beta::graphics->GetBackgroundColor();
	return { bgColor.r, bgColor.g, bgColor.b, bgColor.a };
}

void GraphicsSetBackgroundColor(const Color* color)
{
	Beta::graphics->SetBackgroundColor(
		reinterpret_cast<const Beta::Color&>(*color));
}

// Color to blend with whole screen
Color GraphicsGetScreenTintColor()
{
	Beta::Color tintColor =
		Beta::graphics->GetScreenTintColor();
	return { tintColor.r, tintColor.g, tintColor.b, tintColor.a };
}

void GraphicsSetScreenTintColor(const Color* color)
{
	Beta::graphics->SetScreenTintColor(
		reinterpret_cast<const Beta::Color&>(*color));
}

// Color to blend with the current sprite's color
void GraphicsSetSpriteBlendColor(const Color* color)
{
	Beta::Color blendColor =
		Beta::Color(color->r, color->g, color->b, color->a);
	Beta::graphics->SetSpriteBlendColor(blendColor);
}

// Returns a 1x1 white texture.
const Texture* GraphicsGetDefaultTexture()
{
	return reinterpret_cast<const Texture*>(&Beta::graphics->GetDefaultTexture());
}

// Transform
void GraphicsSetTransform(const Vector2D* translation, const Vector2D* scale,
	float rotation)
{
	GraphicsSetTransformWithDepth(translation, scale, rotation, 0.0f);
}

void GraphicsSetTransformWithDepth(const Vector2D* translation, const Vector2D* scale,
	float rotation, float depth)
{
	Beta::Vector2D t = { translation->x, translation->y };
	Beta::Vector2D s = { scale->x, scale->y };
	Beta::graphics->SetTransform(t, s, rotation, depth);
}

void GraphicsSetTransformMatrix(const Matrix2D* matrix)
{
	GraphicsSetTransformMatrixWithDepth(matrix, 0.0f);
}

void GraphicsSetTransformMatrixWithDepth(const Matrix2D* matrix, float depth)
{
	Beta::graphics->SetTransform(
		reinterpret_cast<const Beta::Matrix2D&>(*matrix), depth);
}

// Camera
Camera* GraphicsGetDefaultCamera()
{
	return reinterpret_cast<Camera*>(&Beta::graphics->GetDefaultCamera());
}

// Returns the default sprite shader (usually for the purpose of setting uniforms)
const ShaderProgram* GraphicsGetSpriteShader()
{
	return reinterpret_cast<const ShaderProgram*>(&Beta::graphics->GetSpriteShader());
}

// Returns the font shader
const ShaderProgram* GraphicsGetFontShader()
{
	return reinterpret_cast<const ShaderProgram*>(&Beta::graphics->GetFontShader());
}

///////////////////////
// ADVANCED SETTINGS //
///////////////////////

// Sets how sprites are blended
void GraphicsSetBlendMode(BlendMode mode)
{
	Beta::graphics->SetBlendMode(static_cast<Beta::BlendMode>(mode));
}

// Adds a post-processing effect. Effects are applied sequentially,
// starting with the first that was added.
// Params:
//   effect =  The effect to add to the current list of effects.
void GraphicsPushEffect(PostEffect* effect)
{
	Beta::graphics->PushEffect(reinterpret_cast<Beta::PostEffect&>(*effect));
}

// Removes the most recently added effect.
void GraphicsPopEffect()
{
	Beta::graphics->PopEffect();
}

// Removes a specific effect.
void GraphicsRemoveEffect(PostEffect* effect)
{
	Beta::graphics->RemoveEffect(reinterpret_cast<Beta::PostEffect*>(effect));
}

// Removes all effects that are currently active.
void GraphicsClearEffects()
{
	Beta::graphics->ClearEffects();
}

// Test whether vertical sync is currently on
bool GraphicsGetUseVsync()
{
	return Beta::graphics->GetUseVsync();
}

// Turn vertical sync on or off - will cause performance issues on some machines
void GraphicsSetUseVSync(bool useVsync)
{
	Beta::graphics->SetUseVSync(useVsync);
}

// Get the dimensions of the viewport
Vector2D GraphicsGetViewport(void)
{
	Beta::Vector2D viewportSize = Beta::graphics->GetViewport();
	return { viewportSize.x, viewportSize.y };
}

// Set the dimensions of the viewport.
// Params:
//   width = The new width of the viewport.
//   height = The new height of the viewport.
void GraphicsSetViewport(int width, int height)
{
	Beta::graphics->SetViewport(width, height);
}

//------------------------------------------------------------------------------
