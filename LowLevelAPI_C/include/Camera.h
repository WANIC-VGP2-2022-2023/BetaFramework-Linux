//------------------------------------------------------------------------------
//
// File Name:	Camera.h
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
#define BE_C_API

#include "Shapes2D.h" // BoundingRectangle, Vector2D

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

typedef struct Matrix3D Matrix3D;
typedef struct Camera Camera;
typedef struct BoundingRectangle BoundingRectangle;

//------------------------------------------------------------------------------
// Public Constants:
//------------------------------------------------------------------------------

typedef enum ProjectionMode
{
	PM_Orthographic,
	PM_Perspective,
}ProjectionMode;

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

BE_C_API Camera* CameraCreate();
BE_C_API void CameraDestroy(Camera** camera);

// Set this camera as the active camera
BE_C_API void CameraUse(const Camera* camera);

// Translation - use this to pan
BE_C_API const Vector2D* CameraGetTranslation(const Camera* camera);
BE_C_API void CameraSetTranslation(Camera* camera, const Vector2D* translation);

// FOV - use this to zoom in Perspective mode
BE_C_API float CameraGetFOV(const Camera* camera);
BE_C_API void CameraSetFOV(Camera* camera, float angle);

// Size - use this to zoom in Orthographic mode
BE_C_API float CameraGetVerticalSize(const Camera* camera);
BE_C_API void CameraSetVerticalSize(Camera* camera, float size);

// Projection mode used by this camera.
BE_C_API ProjectionMode CameraGetProjectionMode(const Camera* camera);
BE_C_API void CameraSetProjectionMode(Camera* camera, ProjectionMode mode);

// Sets camera properties to default values
// (Translation of [0, 0], FOV of 90 degrees, size of 600)
BE_C_API void CameraReset(Camera* camera);

// Gets the size of the viewport in world coordinates
BE_C_API const BoundingRectangle GetScreenWorldDimensions(const Camera* camera);

// Gets the world position of the specified screen position
BE_C_API Vector2D ScreenToWorldPosition(const Camera* camera, const Vector2D* screenPosition);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif
