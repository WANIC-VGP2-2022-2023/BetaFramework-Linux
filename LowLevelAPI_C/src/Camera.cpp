//------------------------------------------------------------------------------
//
// File Name:	Camera.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		Project 7
// Course:		WANIC VGP2 2018-2019
//
// Copyright � 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Camera.h"

#include "../../LowLevelAPI/include/Camera.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

Camera* CameraCreate()
{
	Beta::Camera* camera = (Beta::Camera*)malloc(sizeof(Beta::Camera));
	return reinterpret_cast<Camera*>(camera);
}

void CameraDestroy(Camera** camera)
{
	Beta::Camera* c = reinterpret_cast<Beta::Camera*>(*camera);
	free(c);

	*camera = NULL;
}

// Set this camera as the active camera
void CameraUse(const Camera* camera)
{
	reinterpret_cast<const Beta::Camera*>(camera)->Use();
}

// Translation - use this to pan
const Vector2D* CameraGetTranslation(const Camera* camera)
{
	return reinterpret_cast<const Vector2D*>(
		&reinterpret_cast<const Beta::Camera*>(camera)->GetTranslation());
}

void CameraSetTranslation(Camera* camera, const Vector2D* translation)
{
	reinterpret_cast<Beta::Camera*>(camera)->SetTranslation(
		*reinterpret_cast<const Beta::Vector2D*>(translation));
}

// FOV - use this to zoom in Perspective mode
float CameraGetFOV(const Camera* camera)
{
	return reinterpret_cast<const Beta::Camera*>(camera)->GetFOV();
}

void CameraSetFOV(Camera* camera, float angle)
{
	reinterpret_cast<Beta::Camera*>(camera)->SetFOV(angle);
}

// Size - use this to zoom in Orthographic mode
float CameraGetVerticalSize(const Camera* camera)
{
	return reinterpret_cast<const Beta::Camera*>(camera)->GetVerticalSize();
}

void CameraSetVerticalSize(Camera* camera, float size)
{
	reinterpret_cast<Beta::Camera*>(camera)->SetVerticalSize(size);
}

// Projection mode used by this camera.
ProjectionMode CameraGetProjectionMode(const Camera* camera)
{
	return static_cast<ProjectionMode>(reinterpret_cast<const Beta::Camera*>(camera)->GetProjectionMode());
}

void CameraSetProjectionMode(Camera* camera, ProjectionMode mode)
{
	reinterpret_cast<Beta::Camera*>(camera)->SetProjectionMode(static_cast<Beta::ProjectionMode>(mode));
}

// Sets camera properties to default values
// (Translation of [0, 0], FOV of 90 degrees, size of 600)
void CameraReset(Camera* camera)
{
	reinterpret_cast<Beta::Camera*>(camera)->Reset();
}

// Gets the size of the viewport in world coordinates
const BoundingRectangle GetScreenWorldDimensions(const Camera* camera)
{
	Beta::BoundingRectangle rect = reinterpret_cast<const Beta::Camera*>(camera)->GetScreenWorldDimensions();
	return BoundingRectangleCreate(reinterpret_cast<const Vector2D*>(&rect.center), reinterpret_cast<const Vector2D*>(&rect.extents));
}

// Gets the world position of the specified screen position
Vector2D ScreenToWorldPosition(const Camera* camera, const Vector2D* screenPosition)
{
	Beta::Vector2D position = reinterpret_cast<const Beta::Camera*>(camera)->ScreenToWorldPosition(
		*reinterpret_cast<const Beta::Vector2D*>(screenPosition));
	return { position.x, position.y };
}

//------------------------------------------------------------------------------
