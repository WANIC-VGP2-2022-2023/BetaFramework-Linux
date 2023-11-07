//------------------------------------------------------------------------------
//
// File Name:	Transform.c
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 2
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BetaLow.h"
#include "Stream.h"
#include "Transform.h"
#include "Vector2D.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct Transform
{
	// The translation (or world position) of a game object.
	Vector2D	translation;

	// The rotation (or orientation) of a game object (in radians).
	float	rotation;

	// The scale (or size) of a game object.
	// (Hint: This should be initialized to (1, 1).)
	Vector2D	scale;

	// The transformation matrix resulting from concatenating the 
	//	 matrix = Translation*Rotation*Scale matrices.
	Matrix2D	matrix;

	// True if the transformation matrix needs to be recalculated.
	// (Hint: this should be initialized to true.)
	bool	isDirty;

} Transform;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

static float globalScale = 1.0f;
static bool isGlobalScaleEnabled = false;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new transform object.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// (Hint: Make sure to initialize all member variables properly.)
// (Hint: Failing to initialize the scale values to non-zero values will result in invisible sprites.)
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
TransformPtr TransformCreate(void)
{
	// Allocate the memory for the transform object.
	TransformPtr transform = calloc(1, sizeof(Transform));

	// Initialize any non-zero member variables.
	if (transform)
	{
		transform->scale.x = 1.0f;
		transform->scale.y = 1.0f;

		// Indicate that the transform matrix needs to be recalculated.
		transform->isDirty = true;
	}

	// Return the allocated memory (NULL if unsuccessful).
	return transform;
}

// Dynamically allocate a clone of an existing transform.
// Params:
//	 other = Pointer to the component to be cloned.
// Returns:
//	 If 'other' is valid and the memory allocation was successful,
//	   then return a pointer to the cloned component,
//	   else return NULL.
TransformPtr TransformClone(const TransformPtr other)
{
	// Validate the pointer.
	if (other)
	{
		// Allocate the memory for the transform object.
		TransformPtr transform = calloc(1, sizeof(Transform));

		// Initialize any non-zero member variables.
		if (transform)
		{
			// Perform a shallow copy of the translation, rotation, and scale variables.
			*transform = *other;

			// Indicate that the transform matrix needs to be recalculated.
			transform->isDirty = true;
		}

		// Return the allocated memory (NULL if unsuccessful).
		return transform;
	}
	return NULL;
}

// Free the memory associated with a sprite object.
// (Also, set the transform pointer to NULL.)
// Params:
//	 transform = Pointer to the transform pointer.
void TransformFree(TransformPtr * transform)
{
	// Verify that a valid transform was specified.
	if (transform)
	{
		// Free the allocated memory.
		free(*transform);

		// Set the transform pointer to NULL.
		*transform = NULL;
	}
}

// Read the properties of a Transform component from a file.
// [NOTE: Read the translation value using StreamReadVector2D.]
// [NOTE: Read the rotation value using StreamReadFloat.]
// [NOTE: Read the scale value using StreamReadVector2D.]
// Params:
//	 physics = Pointer to the Transform component.
//	 stream = Pointer to the data stream used for reading.
void TransformRead(TransformPtr transform, Stream stream)
{
	if (transform && stream)
	{
		StreamReadVector2D(stream, &(transform->translation));
		transform->rotation = StreamReadFloat(stream);
		StreamReadVector2D(stream, &(transform->scale));
	}
}

const Vector2D * TransformGetPosition(TransformPtr transform)
{
	// Verify that a valid transform was specified.
	if (transform)
	{
		return &(transform->translation);
	}
	return NULL;
}

// Get the transform matrix, based upon translation, rotation and scale settings.
// Params:
//	 transform = Pointer to the transform object.
// Returns:
//	 If the transform pointer is valid,
//		then return a pointer to the component's matrix structure,
//		else return a NULL pointer.
Matrix2D * TransformGetMatrix(const TransformPtr transform)
{
	// Verify that a valid transform was specified.
	if (transform)
	{
		if (transform->isDirty)
		{
			Matrix2D scale, rot, trans;

			scale = Matrix2DScaling(transform->scale.x, transform->scale.y);
			rot = Matrix2DRotationRadians(transform->rotation);
			trans = Matrix2DTranslation(transform->translation.x, transform->translation.y);

			transform->matrix = Matrix2DMultiply(&rot, &scale);
			transform->matrix = Matrix2DMultiply(&trans, &(transform->matrix));

			transform->isDirty = false;
		}

		return &(transform->matrix);
	}
	return NULL;
}

// Get the translation of a transform component.
// Params:
//	 transform = Pointer to the transform object.
// Returns:
//	 If the transform pointer is valid,
//		then return a pointer to the component's translation structure,
//		else return a NULL pointer.
const Vector2D * TransformGetTranslation(const TransformPtr transform)
{
	// Verify that a valid transform was specified.
	if (transform)
	{
		return &(transform->translation);
	}
	return NULL;
}

float TransformGetRotation(const TransformPtr transform)
{
	// Verify that a valid transform was specified.
	if (transform)
	{
		return transform->rotation;
	}
	return 0;
}

const Vector2D * TransformGetScale(const TransformPtr transform)
{
	// Verify that a valid transform was specified.
	if (transform)
	{
		return &(transform->scale);
	}
	return NULL;
}

void TransformSetTranslation(TransformPtr transform, const Vector2D * translation)
{
	// Verify that a valid transform was specified.
	if (transform)
	{
		transform->translation = *translation;
		transform->isDirty = true;
	}
}

void TransformSetRotation(TransformPtr transform, float rotation)
{
	// Verify that a valid transform was specified.
	if (transform)
	{
		transform->rotation = rotation;
		transform->isDirty = true;
	}
}

void TransformSetScale(TransformPtr transform, const Vector2D * scale)
{
	// Verify that a valid transform was specified.
	if (transform)
	{
		transform->scale = *scale;
		transform->isDirty = true;
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

