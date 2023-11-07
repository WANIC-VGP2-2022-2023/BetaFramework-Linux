//------------------------------------------------------------------------------
//
// File Name:	DebugDraw.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaEngine
// Course:		VGP2
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "DebugDraw.h"

#include "MeshFactory.h"
#include "EngineCore.h" // GetModule
#include "GraphicsEngine.h"
#include "Vertex.h"
#include "Camera.h"
#include "ShaderProgram.h" // Use

// Ignore unused param warning
#pragma warning(push)
#pragma warning(disable : 4100)

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	DebugDraw::DebugDraw()
		: circleMesh(nullptr), rectangleMesh(nullptr), enabled(true), camera(nullptr)
	{
	}

	DebugDraw::~DebugDraw()
	{
#if _DEBUG
		// Free circle and rectangle mesh
		delete circleMesh;
		delete rectangleMesh;
#endif
	}

	void DebugDraw::Initialize()
	{
#if _DEBUG
		// Create circle mesh
		MeshFactory& meshFactory = *EngineGetModule(MeshFactory);
		unsigned numVerts = 64;

		for (unsigned i = 0; i < numVerts; ++i)
		{
			float theta = (2.0f * (float)M_PI) / numVerts * i;
			// There are more efficient ways to do this, but this only happens once, so it's not a major concern
			meshFactory.AddVertex(Vertex(Vector2D(cos(theta) / 2.0f, sin(theta) / 2.0f), Colors::White));
		}
		circleMesh = meshFactory.EndCreate(MDM_Lines_Loop);

		// Create rectangle mesh (4 lines)
		meshFactory.AddVertex(Vertex(Vector2D(-0.5f, 0.5f), Colors::White));
		meshFactory.AddVertex(Vertex(Vector2D(0.5f, 0.5f), Colors::White));
		meshFactory.AddVertex(Vertex(Vector2D(0.5f, -0.5f), Colors::White));
		meshFactory.AddVertex(Vertex(Vector2D(-0.5f, -0.5f), Colors::White));
		rectangleMesh = meshFactory.EndCreate(MDM_Lines_Loop);

		// Set default camera
		camera = &EngineGetModule(GraphicsEngine)->GetDefaultCamera();
#endif
	}

	void DebugDraw::Draw()
	{
#if _DEBUG
		if (!enabled)
			return;

		GraphicsEngine& graphics = *EngineGetModule(GraphicsEngine);

		// Use sprite shader and default texture
		graphics.GetSpriteShader().Use();
		graphics.GetDefaultTexture().Use();

		// Draw circles
		for (auto it = circles.Begin(); it != circles.End(); ++it)
		{
			it->camera->Use();
			graphics.SetSpriteBlendColor(it->color);
			graphics.SetTransform(it->shape.center, Vector2D(it->shape.radius * 2.0f, it->shape.radius * 2.0f), 0.0f, it->zDepth);
			circleMesh->Draw();
		}

		// Draw rectangles
		for (auto it = rectangles.Begin(); it != rectangles.End(); ++it)
		{
			it->camera->Use();
			graphics.SetSpriteBlendColor(it->color);
			graphics.SetTransform(it->shape.center, it->shape.extents * 2.0f, 0.0f, it->zDepth);
			rectangleMesh->Draw();
		}

		// Draw lines
		for (auto it = lineLists.Begin(); it != lineLists.End(); ++it)
		{
			it->camera->Use();
			graphics.SetSpriteBlendColor(Colors::White);
			graphics.SetTransform(Vector2D(), Vector2D(1, 1), 0.0, it->zDepth);
			it->mesh->Draw();
			delete it->mesh;
		}

		// Clear vectors
		lineLists.Clear();
		circles.Clear();
		rectangles.Clear();
#endif
	}

	void DebugDraw::AddLineToList(const Vector2D & start, const Vector2D & end, const Color & color)
	{
#if _DEBUG
		if (enabled)
			tempLines.PushBack(DebugLineSegment(start, end, color));
#endif
	}

	void DebugDraw::EndLineList(float zDepth)
	{
#if _DEBUG
		if (enabled)
		{
			lineLists.PushBack(DebugLineList(tempLines, camera, zDepth));
			tempLines.Clear();
		}
#endif
	}

	void DebugDraw::AddCircle(const Vector2D & center, float radius,
		const Color & color, float zDepth)
	{
#if _DEBUG
		if (enabled)
			circles.PushBack(DebugCircle(center, radius, color, camera, zDepth));
#endif
	}

	void DebugDraw::AddRectangle(const Vector2D & center, const Vector2D & extents,
		const Color & color, float zDepth)
	{
#if _DEBUG
		if (enabled)
			rectangles.PushBack(DebugRectangle(center, extents, color, camera, zDepth));
#endif
	}

	void DebugDraw::SetCamera(Camera& camera_)
	{
		camera = &camera_;
	}

	// Enables or disables debug drawing. 
	// Note that debug drawing is only possible in debug mode.
	void DebugDraw::SetEnabled(bool value)
	{
		enabled = value;
	}

	// Returns a boolean value that indicates whether debug drawing is enabled.
	bool DebugDraw::IsEnabled() const
	{
		return enabled;
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	DebugDraw::DebugLineSegment::DebugLineSegment(const Vector2D & start, const Vector2D & end,
		const Color & color)
		: start(start), end(end), color(color)
	{
	}

	// Creates the mesh for the lines added via AddLine
	DebugDraw::DebugLineList::DebugLineList(const Array<DebugLineSegment> & segments,
		Camera * camera, float zDepth)
		: camera(camera), zDepth(zDepth)
	{
#if _DEBUG
		MeshFactory& meshFactory = *EngineGetModule(MeshFactory);

		for (auto it = segments.Begin(); it != segments.End(); ++it)
		{
			meshFactory.AddVertex(Vertex(it->start, it->color));
			meshFactory.AddVertex(Vertex(it->end, it->color));
		}

		mesh = meshFactory.EndCreate(MDM_Lines);
#endif
	}

	DebugDraw::DebugCircle::DebugCircle(const Vector2D & center, float radius,
		const Color & color, Camera * camera, float zDepth)
		: shape(center, radius), color(color), camera(camera), zDepth(zDepth)
	{
	}

	DebugDraw::DebugRectangle::DebugRectangle(const Vector2D & center,
		const Vector2D & extents, const Color & color, Camera * camera, float zDepth)
		: shape(center, extents), color(color), camera(camera), zDepth(zDepth)
	{
	}
}

#pragma warning(pop)
